#include <iostream>
#include <fstream>
#include "programm.hpp"
#include "my_shmem.hpp"
#include "my_thread.hpp"

using std::cin;

struct Counter{
  int count = 0;
};

void write(std::string str) {
  std::ofstream out("log.txt", std::ios::app);
  if (out.is_open()){
    out << str << "\n";
  }
  out.close();  
}

int main(int argc, char** argv) {
  auto shmem = cplib::SharedMem<Counter>("shmem");
  std::string cmd = "";
  int iparam = 0;

  while(true){
    std::cin >> cmd;
		if (cmd == "i" || cmd == "int") {
			std::cin >> iparam;
		} else if (cmd == "e" || cmd == "q" || cmd == "exit" || cmd == "quite") {
			break;
		} else{
			std::cout << "! Possible commands: i | q" << std::endl;
    }
  }

  write(TimerProgramm::get_cur_time());
  return 0;
}

