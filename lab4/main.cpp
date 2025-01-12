#include <iostream>
#include "my_serial.hpp"

std::string INPUT_PORT = "COM5";

int main(int argc, char** argv){
  cplib::SerialPort smport(INPUT_PORT, cplib::SerialPort::BAUDRATE_115200);
	if (!smport.IsOpen()) {
		std::cout << "Failed to open port \n";
		return -2;
	}

  std::string mystr;

  smport.SetTimeout(1.0);
  while(true){
    smport >> mystr;
    std::cout << mystr << "\n";
  }

  return 0;
}