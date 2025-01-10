#include <iostream>
#include "programm.hpp"

int main(int argc, char** argv) {
  std::cout << TimerProgramm::get_cur_time();
  return 0;
}