#include "my_lib.hpp"

using namespace BackgroundProgramm;

#ifdef _WIN32
void BackgroundProgramm::run(char* argv[]){
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));

  if(!CreateProcess(
    NULL,
    argv[1],
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

#else
void BackgroundProgramm::run(char* argv[]){
  pid_t pid;
  int status;

  switch(pid = fork()){
    case -1:{
      perror("fork");
      exit(1);
    }
    case 0: {
      execv(argv[1], NULL);
    }
    default:{
      wait(&status);
    }
  }
}
#endif