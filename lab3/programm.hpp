#pragma once
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <spawn.h>
#endif

namespace TimerProgramm {
  using std::string;
  using std::to_string;

  string get_cur_time();
  string get_cur_data();
  int get_pid();
  void create_process(int argc, char* argv[], char* copy_num);
  void sleep(int sec);
}
