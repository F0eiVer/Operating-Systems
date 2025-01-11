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

string TimerProgramm::get_cur_data() {
  SYSTEMTIME lt;
  GetLocalTime(&lt);
  string str_data = "";

  str_data += to_string(lt.wDay) + ".";
  str_data += to_string(lt.wMonth) + ".";
  str_data += to_string(lt.wYear);
  return str_data;
}

int TimerProgramm::get_pid(){
  return GetCurrentProcessId();
}

void TimerProgramm::create_process(int argc, char* argv[], char* copy_num){
  string cmd = "";
  for (int i = 0; i < argc; ++i){
    cmd += argv[i];
    cmd += " ";
  }
  cmd += copy_num;

  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));

  if(!CreateProcess(
    NULL,
    (char *)cmd.c_str(),
    NULL,
    NULL,
    false,
    0,
    NULL,
    NULL,
    &si,
    &pi))
  {
    std::cout << "Process creation failed\n";
    return;
  }

  WaitForSingleObject(pi.hProcess, INFINITE);

  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
}

void TimerProgramm::sleep(int sec) {
  Sleep(sec * 1000);
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

string TimerProgramm::get_cur_data(){
  string str_data = "";

  time_t t = time(NULL);
  struct tm *now = localtime(&t);

  str_data += to_string(now->tm_day) + ".";
  str_data += to_string(now->tm_mon + 1) + ".";
  str_data += to_string(now->tm_year + 1900);

  return str_data;
}

int TimerProgramm::get_pid(){
  return getpid();
}

void TimerProgramm::create_process(int argc, char* argv[], char* copy_num){
  string cmd = "";
  for (int i = 0; i < argc; ++i){
    cmd += argv[i];
    cmd += " ";
  }
  cmd += copy_num;

  pid_t pid;
  int status;

  switch(pid = fork()){
    case -1:{
      perror("fork");
      exit(1);
    }
    case 0: {
      execv(argv[0], (char *)cmd.c_str());
    }
    default:{
      wait(&status);
    }
  }
}

void TimerProgramm::sleep(int sec) {
  sleep(sec);
}

#endif
