#pragma once
#include <iostream>
#include <ctime>

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

namespace help_P {
  using std::string;
  using std::to_string;

  time_t my_time();
}