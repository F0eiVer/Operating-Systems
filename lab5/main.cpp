#include <iostream>
#include <fstream>
#include <sqlite3.h>
#include "my_serial.hpp"
#include "my_thread.hpp"
#include "help_programm.hpp"

using std::string;
#ifdef _WIN32
string INPUT_PORT = "COM5";
#else
string INPUT_PORT = "/dev/pts/2";
#endif

string TMP_FILE_NAME = "tmp.txt";
string CUR_FILE = "cur.txt";
string AVG_HOUR = "hour.txt";
string AVG_DAY = "day.txt";
char* DATABASE_NAME = "temperature.db";

time_t SEC_HOUR = 3600;
time_t SEC_DAY = 86400;
time_t SEC_MONTH = 2592000;
time_t SEC_YEAR = 31536000;

char* SQL_CREATE_TABLES = "CREATE TABLE IF NOT EXISTS cur_temp(id INTEGER PRIMARY KEY AUTOINCREMENT, temp REAL, time INTEGER);"
                          "CREATE TABLE IF NOT EXISTS hour_temp(id INTEGER PRIMARY KEY AUTOINCREMENT, temp REAL, time INTEGER);"
                          "CREATE TABLE IF NOT EXISTS day_temp(id INTEGER PRIMARY KEY AUTOINCREMENT, temp REAL, time INTEGER);";

double get_avg_temp(string file_name, time_t time_now, time_t interval_time){
  string line;
  std::ifstream in(file_name);
  double sum_d = 0;
  double count_d = 0;

  if (in.is_open())
  {
    while (std::getline(in, line))
    {
      if(time_now - help_P::get_time(line) <= interval_time){
        sum_d += help_P::get_time(line);
        count_d++;
      }
    }
  }
  in.close();
  return sum_d / count_d;
}

int db_delete(sqlite3* db, time_t time_now){
  char *err_msg = 0;
  char *sql = (char*)("DELETE FROM cur_temp WHERE " + std::to_string(time_now) + " - time > " + std::to_string(SEC_DAY)).c_str();
  int rc = sqlite3_exec(db, sql, 0, 0, &err_msg); 

  if(time_now % SEC_HOUR == 0){
    sql = (char*)("DELETE FROM hour_temp WHERE " + std::to_string(time_now) + " - time > " + std::to_string(SEC_MONTH)).c_str();
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg); 
  }else if (time_now % SEC_DAY == 0) {
    sql = (char*)("DELETE FROM day_temp WHERE " + std::to_string(time_now) + " - time > " + std::to_string(SEC_YEAR)).c_str();
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg); 
  }
  return rc;
}

int db_add(sqlite3* db, string str){
  time_t time_now = help_P::my_time();

  int rc = db_delete(db, time_now);
  if(rc != SQLITE_OK){
    std::cout << "delete problems\n";
    return rc;
  }
  char *err_msg = 0;

  size_t indx = str.find('.');
  str = str.substr(0, indx + 6);
  char *sql = (char *)("INSERT INTO cur_temp (temp, time) VALUES (" + str + "," + std::to_string(time_now) + ");").c_str();
  rc = sqlite3_exec(db, sql, 0, 0, &err_msg); 

  if(time_now % SEC_HOUR == 0){
    sql = (char *)("INSERT INTO hour_temp (temp, time) VALUES (" + str + "," + std::to_string(time_now) + ");").c_str();
    str = std::to_string(get_avg_temp(CUR_FILE, time_now, SEC_HOUR));
    indx = str.find('.');
    str = str.substr(0, indx + 6);

    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg); 
  } else if (time_now % SEC_DAY == 0) {
    sql = (char *)("INSERT INTO day_temp (temp, time) VALUES (" + str + "," + std::to_string(time_now) + ");").c_str();
    str = std::to_string(get_avg_temp(AVG_HOUR, time_now, SEC_DAY));
    indx = str.find('.');
    str = str.substr(0, indx + 6);
    
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg); 
  }
  return rc;
}

class LogThread : public cplib_thread::Thread {
public:
	LogThread(string port_name, sqlite3* db): _port(port_name), _db(db) {}
	virtual void Main() {
    cplib::SerialPort smport(_port, cplib::SerialPort::BAUDRATE_115200);
    if (!smport.IsOpen()) {
      std::cout << "Failed to open port \n";
      return;
    }
    char *err_msg = 0;
    // открываем подключение к базе данных
    int result  = sqlite3_open(DATABASE_NAME, &_db);
    // если подключение успешно установлено
    if(result == SQLITE_OK) {
      std::cout << "Connection established\n";
    } else {
      sqlite3_close(_db);
      std::cout << "Can't connect to database\n";
    }
    result = sqlite3_exec(_db, SQL_CREATE_TABLES, 0, 0, &err_msg);
    if (result != SQLITE_OK){
      std::cout << "SQL error: %s\n" << err_msg;
      sqlite3_free(err_msg);
      return;
    }
    string temp;

    smport >> temp;
		while (true) {
      smport >> temp;;
      db_add(_db, temp);
			CancelPoint();
		}
	}
private:
  std::string _port;
  sqlite3* _db;
};

int main(int argc, char** argv){
  sqlite3* db;
  LogThread th_log(INPUT_PORT, db);
  th_log.Start();
  th_log.WaitStartup();
  std::string cmd = "";

  while(true){
    std::cin >> cmd;
    if (cmd == "e" || cmd == "q" || cmd == "exit" || cmd == "quite") {
      break;
    } else{
      std::cout << "Possible commands: e\n";
    }
  }

  th_log.Stop();
  th_log.Join();
  sqlite3_close(db);
  return 0;
}