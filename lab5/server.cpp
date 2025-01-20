// Простенький сервер, публикующий сообщения устройства на HTML-страничке

#include <iostream>         /* std::cout */
#include <string>           /* std::string */
#include <sstream>          /* std::stringbuf */  
#include <fstream>
#include <sqlite3.h>
#include <stdlib.h>         /* atoi */
#include <string.h>         /* memset */
#include <string>
#include <time.h>
#include <vector>

#ifdef _WIN32
#   include <winsock2.h>    /* socket */
#   include <ws2tcpip.h>    /* ipv6 */
#else
#   include <sys/socket.h>  /* socket */
#   include <netinet/in.h>  /* socket */
#   include <arpa/inet.h>   /* socket */
#   include <unistd.h>      
#   define SOCKET int
#   define INVALID_SOCKET -1
#   define SOCKET_ERROR -1
#endif

// Ожидание запроса от браузера в мс
#define READ_WAIT_MS 50

// Немного полезных функций по работе с сокетами, вынесенных в отдельный класс
class SocketWorker
{
public:
    SocketWorker():m_socket(INVALID_SOCKET) {}
    ~SocketWorker() { CloseMySocket(); }
    /// Возвращает код ошибки
    static int ErrorCode()
    {
#if defined (WIN32)
        return WSAGetLastError();
#else
        return errno;
#endif
    }
protected:
    void CloseMySocket()
    {
        CloseSocket(m_socket);
        m_socket = INVALID_SOCKET;
    }
    void CloseSocket(SOCKET sock)
    {
        if (sock == INVALID_SOCKET)
            return;
#if defined (WIN32)
        shutdown(sock, SD_SEND);
        closesocket(sock);
#else
        shutdown(sock, SHUT_WR);
        close(sock);
#endif
    }
    SOCKET m_socket;
};

// Сервер, отдающий HTML-страницу с данными от железки
class HTTPServer: public SocketWorker
{
public:
    HTTPServer(sqlite3* db): _db(db) {};
    ~HTTPServer(){
        sqlite3_close(_db);
    }
	// Регистрируем сокет на прослушку подключения
    SOCKET Listen(const std::string& interface_ip, short int port)
    {
        // Создаем сокет ipv4
        m_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (m_socket == INVALID_SOCKET) {
            std::cout << "Cant open socket: " << ErrorCode() << std::endl;
            return INVALID_SOCKET;
        }
        // Биндим сокет на адрес и порт
        sockaddr_in local_addr;
        memset(&local_addr, 0, sizeof(local_addr));
        local_addr.sin_family = AF_INET;
        local_addr.sin_addr.s_addr = inet_addr(interface_ip.c_str());
        local_addr.sin_port = htons(port);
        if (bind(m_socket, (struct sockaddr*)&local_addr, sizeof(local_addr))) {
            std::cout << "Failed to bind: " << ErrorCode() << std::endl;
            CloseMySocket();
            return INVALID_SOCKET;
        }
        // Запускаем прослушку на сокете
        if (listen(m_socket, SOMAXCONN)) {
            std::cout << "Failed to start listen: " << ErrorCode() << std::endl;
            CloseMySocket();
            return INVALID_SOCKET;
        }
        return m_socket;
    }
	// Обработаем подключение клиента (браузера), возвращая ему строку с железки
    void ProcessClient()
    {
        // Принимаем входящие соединения
        SOCKET client_socket = accept(m_socket, NULL, NULL);
        if (client_socket == INVALID_SOCKET) {
            std::cout << "Error accepting client: " << ErrorCode() << std::endl;
            CloseSocket(client_socket);
            return;
        }
        // Хочет ли клиент с нами говорить?
		// (современные браузеры могу открыть два подключения сразу)
//         struct pollfd polstr;
//         memset(&polstr, 0, sizeof(polstr));
//         polstr.fd = client_socket;
//         polstr.events |= POLLIN;
// #ifdef _WIN32
//         int ret = WSAPoll(&polstr, 1, READ_WAIT_MS);
// #else
//         int ret = poll(&polstr, 1, READ_WAIT_MS);
// #endif
//         std::cout << ret << "\n";
		// Не хочет - закрываем сокет
        // if (ret <= 0) {
        //     CloseSocket(client_socket);
        //     return;
        // }
        // Прочитаем, что клиент нам сказал (блокирующий вызов!!)
        int result = recv(client_socket, m_input_buf, sizeof(m_input_buf), 0);
        if (result == SOCKET_ERROR) {
            std::cout << "Error on client receive: " << result << std::endl;
            CloseSocket(client_socket);
            return;
        } else if (result == 0) {
            std::cout << "Client closed connection before getting any data!" << std::endl;
            CloseSocket(client_socket);
            return;
        }
        std::string input = m_input_buf;
        std::cout << m_input_buf << "\n";
        std::string type = input.substr(input.find("GET") + 4, 4);
        bool table = (input.find("Graph") == std::string::npos);
        bool clearData = (input.find("Clear") != std::string::npos);
        std::stringstream response;
        std::stringstream response_body;
        char* sql_stmt = "";
        sqlite3_stmt* stmt;
        int rc = sqlite3_open("temperature.db", &_db);

        if(type == "/sec"){
            sql_stmt = "SELECT * FROM cur_temp";
        }else if(type == "/hou"){
            sql_stmt = "SELECT * FROM hour_temp";
        }else if(type == "/day"){
            sql_stmt = "SELECT * FROM day_temp";
        }

        if(sql_stmt == ""){
            response_body << readFile("../index.html", NULL);
        }else {
            rc = sqlite3_prepare_v2(_db, sql_stmt, -1, &stmt, 0);
            if (rc != SQLITE_OK) {
                printf("\nUnable to fetch data");
                sqlite3_close(_db);
                return;
            }
            if(table){
                response_body << readFile("../data_table.html", stmt, table, clearData);
            } else {
                response_body << readFile("../data_graphic.html", stmt, table, clearData);
            }
            
        }
        
        response << "HTTP/1.0 200 OK\r\n"
                 << "Version: HTTP/1.1\r\n"
                 << "Content-Type: text/html; charset=utf-8\r\n"
                 << "Content-Length: " << response_body.str().length()
                 << "\r\n\r\n"
                 << response_body.str();
        
        // Отправляем ответ клиенту
        result = send(client_socket, response.str().c_str(), (int)response.str().length(), 0);
        if (result == SOCKET_ERROR) {
            // произошла ошибка при отправке данных
            std::cout << "Failed to send responce to client: " << ErrorCode() << std::endl;
        }
        // Закрываем соединение к клиентом
        CloseSocket(client_socket);
        std::cout << "Answered to client!" << std::endl;
    }
private:
	// Буфер для чтения запроса браузера
    char   m_input_buf[1024];
    sqlite3* _db;

    std::string readFile(std::string fileName, sqlite3_stmt* table_data, bool table = true, bool clearData = false){
        std::ifstream f(fileName);
        std::stringstream ss;
        std::string first_part = "";
        ss << f.rdbuf();
        std::string file_body = ss.str();
        if(!table_data){
            return file_body;
        }
        if(table){
            first_part = file_body.substr(0, file_body.find("<tbody>") + 7);
            std::string second_part = file_body.substr(file_body.find("<tbody>") + 7);
            while (sqlite3_step(table_data) == SQLITE_ROW) {
                time_t temp_time = std::stoll((char*)sqlite3_column_text(table_data, 2));
                std::string time = (clearData) ? std::to_string(temp_time) : std::string(ctime(&temp_time));
                first_part += "<tr><td>" + std::string(((char*)sqlite3_column_text(table_data, 1))) + "</td><td>" + time + "</td></tr>";
            }
            first_part += second_part;
        } else {
            std::vector<double> temp = {};
            size_t size = 0;
            first_part = file_body.substr(0, file_body.find("data: [") + 7);
            std::string second_part = file_body.substr(file_body.find("data: [") + 7);
            while (sqlite3_step(table_data) == SQLITE_ROW) {
                size++;
                temp.push_back(std::stod(((char*)sqlite3_column_text(table_data, 1))));
            }
            for(size_t i = 0; i < size; ++i){
                first_part += "{x:" + std::to_string((double)i / (double)size) + ",y:"+ std::to_string(temp[i]) + "},";
            }
            first_part += second_part;
        }
        // {x:-10,y:0},
        return first_part;
    }
};

// Основная программа
int main (int argc, char** argv)
{   
    if (argc < 3) {
        std::cout << "Usage: ipv4srv_addr srv_port" << std::endl;
        return -1;
    }
    sqlite3* db;
    // Инициализируем библиотеку сокетов (под Windows)
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);
#else
    // Игнорируем SIGPIPE сигнал
    // чтобы программа не терминировалась при попытке записи в закрытый сокет
    signal(SIGPIPE, SIG_IGN);
#endif
	// Создаем клиент и сервер и инициализируем их
    HTTPServer srv(db);
    SOCKET server_socket = srv.Listen(argv[1], atoi(argv[2]));
    if (server_socket == INVALID_SOCKET) {
        std::cout << "Terminating..." << std::endl;
#ifdef _WIN32
        WSACleanup();
#endif
        return -1;
    }
    // Будем ждать событий на двух сокетах - клиента UDP и сервера TCP
    struct pollfd polstr;
    memset(&polstr, 0, sizeof(polstr));
    polstr.fd = server_socket;
    polstr.events |= POLLIN;

    // Цикл ожидания событий на сокетах
    for (;;) {
        int ret = 0;
#ifdef _WIN32
        ret = WSAPoll(&polstr, 1, -1);
#else
        ret = poll(polstr, 1, -1);
#endif
        std::cout << ret << "\n";
        // Ошибка сокета
        if (ret <= 0) {
            std::cout << "Error on poll: " << SocketWorker::ErrorCode() << std::endl;
            continue;
        }
        if (polstr.revents & POLLIN) {
            // Есть HTTP-клиент - возвращаем ему страницу
            srv.ProcessClient();
        }
    }
    // Сюда мы никогда не попадем, но в цикле можно предусмотреть условие выхода, например по ошибке Poll
    // Деинициализируем библиотеку сокетов (под Windows) 
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}