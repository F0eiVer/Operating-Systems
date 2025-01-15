#pragma once
#include <iostream>
#include <ctime>
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

namespace help_P {
  using std::string;
  using std::to_string;

  char* SQL_CREATE = "CREATE TABLE IF NOT EXISTS cur_temp(id INTEGER PRIMARY KEY AUTOINCREMENT, temp REAL, time INTEGER);"
                     "CREATE TABLE IF NOT EXISTS hour_temp(id INTEGER PRIMARY KEY AUTOINCREMENT, temp REAL, time INTEGER);"
                     "CREATE TABLE IF NOT EXISTS day_temp(id INTEGER PRIMARY KEY AUTOINCREMENT, temp REAL, time INTEGER);";


  time_t my_time();
  time_t get_time(string line);
  double get_temp(string line);
  void db_add();
}