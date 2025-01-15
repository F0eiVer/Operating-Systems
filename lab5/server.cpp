// Простенький сервер, публикующий сообщения устройства на HTML-страничке

#include <iostream>         /* std::cout */
#include <string>           /* std::string */
#include <sstream>          /* std::stringbuf */  

#include <stdlib.h>         /* atoi */
#include <string.h>         /* memset */

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
    void ProcessClient(const char* device_str)
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
        struct pollfd polstr;
        memset(&polstr, 0, sizeof(polstr));
        polstr.fd = client_socket;
        polstr.events |= POLLIN;
#ifdef _WIN32
        int ret = WSAPoll(&polstr, 1, READ_WAIT_MS);
#else
        int ret = poll(&polstr, 1, READ_WAIT_MS);
#endif
		// Не хочет - закрываем сокет
        if (ret <= 0) {
            CloseSocket(client_socket);
            return;
        }
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
        std::cout << m_input_buf << '\n';
        // Сюда запишем полный ответ клиенту
        std::stringstream response;
        // Сюда запишем HTML-страницу с ответом
        std::stringstream response_body; // тело ответа
        // TODO: хороший сервер должен анализировать заголовки запроса (m_input_buf)
        // и на их основе создавать ответ. Но это - плохой сервер )
        response_body << "<html>\n<head>"
                      << "\t<meta http-equiv=\"Refresh\" content=\"1\" />\n"
                      << "\t<title>Temperature Device Server</title>\n"
                      << "</head>\n\t<body>\n"
                      << "\t\t<h1>Temperature Device Server</h1>\n"
                      << "\t\t<p>Current device data: <b>"
                      << (strlen(device_str) ? device_str : "No data provided!")
                      << "</b></p>\n"
                      << "\t</body>\n</html>";
        // Формируем весь ответ вместе с заголовками
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
};

// Основная программа
int main (int argc, char** argv)
{
    if (argc < 3) {
        std::cout << "Usage: ipv4srv_addr srv_port" << std::endl;
        return -1;
    }
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
    HTTPServer srv;
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

    // Буфер для получения данных от железки
    char server_buf[255];
    memset(server_buf, 0, sizeof(server_buf));
    // Цикл ожидания событий на сокетах
    for (;;) {
        int ret = 0;
#ifdef _WIN32
        ret = WSAPoll(&polstr, 1, -1);
#else
        ret = poll(polstr, 1, -1);
#endif
        // Ошибка сокета
        if (ret <= 0) {
            std::cout << "Error on poll: " << SocketWorker::ErrorCode() << std::endl;
            continue;
        }
        if (polstr.revents & POLLIN) {
            // Есть HTTP-клиент - возвращаем ему страницу
            srv.ProcessClient(server_buf);
        }
    }
    // Сюда мы никогда не попадем, но в цикле можно предусмотреть условие выхода, например по ошибке Poll
    // Деинициализируем библиотеку сокетов (под Windows) 
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}