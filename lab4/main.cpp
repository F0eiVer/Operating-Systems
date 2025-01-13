#include <iostream>
#include <fstream>
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

time_t SEC_HOUR = 3600;
time_t SEC_DAY = 86400;
time_t SEC_MONTH = 2592000;
time_t SEC_YEAR = 31536000;


time_t get_time(string line){
  size_t indx1 = line.find('*');
  size_t indx2 = line.find('*', indx1 + 1);
  return std::stoll(line.substr(indx1 + 1, indx2 - indx1));
}

double get_temp(string line){
  size_t indx1 = line.find('*');
  return std::stod(line.substr(0, indx1 + 1));
}

void copy_file(string file_name){
  std::ifstream in(file_name, std::ios::binary);
  std::ofstream out(TMP_FILE_NAME, std::ios::binary);

  out << in.rdbuf();

  in.close();
  out.close();
}

double get_avg_temp(string file_name, time_t time_now, time_t interval_time){
  string line;
  std::ifstream in(file_name);
  double sum_d = 0;
  double count_d = 0;

  if (in.is_open())
  {
    while (std::getline(in, line))
    {
      if(time_now - get_time(line) <= interval_time){
        sum_d += get_time(line);
        count_d++;
      }
    }
  }
  in.close();
  return sum_d / count_d;
}

void write_valid(string file_name, time_t time_now, time_t max_time){
  string line;
  copy_file(file_name);
  std::ifstream in(TMP_FILE_NAME);
  std::ofstream out(file_name);

  if (in.is_open())
  {
    while (std::getline(in, line))
    {
      if(time_now - get_time(line) < max_time){
        out << line << '\n';
      }
    }
  }
  out.close();
  in.close();
}



void write(string str) {
  time_t time_now = help_P::my_time();
  write_valid(CUR_FILE, time_now, SEC_DAY);

  std::ofstream out(CUR_FILE, std::ios::app);
  
  size_t indx = str.find('.');
  str = str.substr(0, indx + 6);
  str = str + '*' + std::to_string(time_now) + '*';
  erase(str, ' ');

  if (out.is_open() && str[0] != '*'){
    out << str << "\n";
  }

  out.close();  

  if(time_now % SEC_HOUR == 0){
    write_valid(AVG_HOUR, time_now, SEC_MONTH);

    str = std::to_string(get_avg_temp(CUR_FILE, time_now, SEC_HOUR));
    indx = str.find('.');
    str = str.substr(0, indx + 6);
    str = str + '*' + std::to_string(time_now) + '*';
    erase(str, ' ');

    std::ofstream out(AVG_HOUR, std::ios::app);
    if (out.is_open()){
      out << str << "\n";
    }
    out.close();  
  } else if (time_now % SEC_DAY == 0) {
    write_valid(AVG_DAY, time_now, SEC_YEAR);

    str = std::to_string(get_avg_temp(AVG_HOUR, time_now, SEC_DAY));
    indx = str.find('.');
    str = str.substr(0, indx + 6);
    str = str + '*' + std::to_string(time_now) + '*';
    erase(str, ' ');

    std::ofstream out(AVG_DAY, std::ios::app);
    if (out.is_open()){
      out << str << "\n";
    }
    out.close(); 
  }
}

class SecondLogThread : public cplib_thread::Thread {
public:
	SecondLogThread(string port_name): _port(port_name) {}
	virtual void Main() {
    cplib::SerialPort smport(_port, cplib::SerialPort::BAUDRATE_115200);
    if (!smport.IsOpen()) {
      std::cout << "Failed to open port \n";
      return;
    }
    string temp;

    smport >> temp;
		while (true) {
      smport >> temp;
      write(temp);
			CancelPoint();
		}
	}
private:
  std::string _port;
};

int main(int argc, char** argv){
  SecondLogThread th_log(INPUT_PORT);
  th_log.Start();
  th_log.WaitStartup();
  std::string cmd = "";

  while(true){
    std::cin >> cmd;
    if (cmd == "i" || cmd == "int") {
    } else if (cmd == "s" || cmd == "show"){
    } else if (cmd == "e" || cmd == "q" || cmd == "exit" || cmd == "quite") {
      break;
    } else{
      std::cout << "Possible commands: i | s | q\n";
    }
  }

  th_log.Stop();
  th_log.Join();
  return 0;
}