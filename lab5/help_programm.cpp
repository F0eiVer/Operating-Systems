#include "help_programm.hpp"

time_t help_P::my_time(){
  time_t rawtime;
  time(&rawtime);

  return rawtime;   
}

time_t help_P::get_time(string line){
  size_t indx1 = line.find('*');
  size_t indx2 = line.find('*', indx1 + 1);
  return std::stoll(line.substr(indx1 + 1, indx2 - indx1));
}

double help_P::get_temp(string line){
  size_t indx1 = line.find('*');
  return std::stod(line.substr(0, indx1 + 1));
}

void help_P::db_add(string line){

}