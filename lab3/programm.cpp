#include "programm.hpp"

using namespace TimerProgramm;
#ifdef _WIN32

string TimerProgramm::get_cur_time(){
  SYSTEMTIME lt;
  GetLocalTime(&lt);
  string str_time = "";

  str_time += to_string(lt.wHour) + ":";
  str_time += to_string(lt.wMinute) + ":";
  str_time += to_string(lt.wSecond) + ":";
  str_time += to_string(lt.wMilliseconds);
  return str_time;
}

int TimerProgramm::get_pid(){
  return GetCurrentProcessId();
}

#else

string TimerProgramm::get_cur_time(){
  string str_time = "";

  time_t t = time(NULL);
  struct tm *now = localtime(&t);
  struct timeval tv;
  gettimeofday(&tv, 0);

  str_time += to_string(now->tm_hour) + ":";
  str_time += to_string(now->tm_min) + ":";
  str_time += to_string(now->tm_sec) + ":";
  str_time += to_string(tv.tv_usec % 1000);

  return str_time;
}

int TimerProgramm::get_pid(){
  return getpid();
}


#endif
