#include <iostream>
#include "my_lib.hpp"

int main(int argc, char* argv[]){
  // child programms will have the same flags
  if(argc != 2){
    std::cout << "incorrect use of" << argv[0] << '\n';
    return 0;
  }
  std::cout << "This is main process\n";
  BackgroundProgramm::run(argv);

  return 0;
}