#include <iostream>
#include <fstream>
#include "programm.hpp"
#include "my_shmem.hpp"

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

  write(TimerProgramm::get_cur_time());
  return 0;
}

