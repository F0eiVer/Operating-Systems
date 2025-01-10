#pragma once
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

namespace TimerProgramm {
  using std::string;
  using std::to_string;

  string get_cur_time();

}