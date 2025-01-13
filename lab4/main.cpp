#include <iostream>
#include <fstream>
#include "my_serial.hpp"
#include "my_thread.hpp"
#include "help_programm.hpp"

std::string INPUT_PORT = "COM5";
time_t SEC_HOUR = 3600;
time_t SEC_DAY = 86400;
time_t SEC_MONTH = 2592000;
time_t SEC_YEAR = 31536000;

void write(std::string str, std::string file_name, bool next = true) {
  std::ofstream out(file_name, std::ios::app);
  if (out.is_open()){
    out << str;
    next ? out << "\n" : out << " ";
  }
  out.close();  
}

class SecondLogThread : public cplib_thread::Thread {
public:
	SecondLogThread(std::string port_name): _port(port_name) {}
	virtual void Main() {
    cplib::SerialPort smport(_port, cplib::SerialPort::BAUDRATE_115200);
    if (!smport.IsOpen()) {
      std::cout << "Failed to open port \n";
      return;
    }
    std::string temp;
    temp.erase();

    smport >> temp;
		while (true) {
      smport >> temp;
      size_t indx = temp.find('.');
      temp = temp.substr(0, indx + 6);
      temp = temp + '*' + std::to_string(help_P::my_time()) + '*';
      erase(temp, ' ');
      if(temp[0] != '*'){
        write(temp, "cur_log.txt");
      }
			// this->Sleep(1.0);
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