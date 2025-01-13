#include "help_programm.hpp"

time_t help_P::my_time(){
  time_t rawtime;
  time(&rawtime);

  return rawtime;   
}
