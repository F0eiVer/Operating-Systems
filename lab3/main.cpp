#include <iostream>
#include <fstream>
#include "programm.hpp"
#include "my_shmem.hpp"

void write(std::string str) {
  std::ofstream out("log.txt", std::ios::app);
  if (out.is_open()){
    out << str << "\n";
  }
  out.close();  
}

int main(int argc, char** argv) {
  write(TimerProgramm::get_cur_time());
  return 0;
}

