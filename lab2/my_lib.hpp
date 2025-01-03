#pragma once
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

namespace BackgroundProgramm {
  void run(char**);
}