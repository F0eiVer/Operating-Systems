#include "programm.hpp"

using namespace TimerProgramm;
#ifdef _WIN32

string TimerProgramm::get_cur_time(){
  SYSTEMTIME lt;
  GetLocalTime(&lt);
  string str_time = "";

  str_time += to_string(lt.wDay) + ".";
  str_time += to_string(lt.wMonth) + ".";
  str_time += to_string(lt.wYear) + " ";
  str_time += to_string(lt.wHour) + ":";
  str_time += to_string(lt.wMinute) + ":";
  str_time += to_string(lt.wSecond);
  return str_time;
}

#else

string TimerProgramm::get_cur_time(){
  // from example https://en.cppreference.com/w/c/chrono/localtime

  time_t t = time(NULL);
  return string(asctime(localtime(&t)));
}


#endif
