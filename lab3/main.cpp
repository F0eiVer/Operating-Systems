#include <iostream>
#include <fstream>
#include "programm.hpp"
#include "my_shmem.hpp"
#include "my_thread.hpp"

using std::cin;
using std::cout;
using std::to_string;

void write(std::string str, bool next = true) {
  std::ofstream out("log.txt", std::ios::app);
  if (out.is_open()){
    out << str;
    next ? out << "\n" : out << " ";
  }
  out.close();  
}

struct Counter{
  int count = 0;
};

class IncrementThread : public cplib_thread::Thread {
public:
	IncrementThread(cplib_shared::SharedMem<Counter>* sh_mem) :_mem(sh_mem) {}
	virtual void Main() {
		while (true) {
      increment();
			this->Sleep(0.3);
			CancelPoint();
		}
	}

  void increment(){
    _mem->Lock();
    _mem->Data()->count += 1;
    _mem->Unlock();
  }

private:
	cplib_shared::SharedMem<Counter>* _mem;
};

class DataThread : public cplib_thread::Thread {
public:
  DataThread(cplib_shared::SharedMem<Counter>* sh_mem) :_mem(sh_mem) {}
	virtual void Main() {
    int count = 0;
		while (true) {
      _mem->Lock();
      count = _mem->Data()->count;
      _mem->Unlock();
      write("my pid: " + to_string(TimerProgramm::get_pid()), false);
      write("count: " + to_string(count), false);
      write("data: " + TimerProgramm::get_cur_data(), false);
      write(TimerProgramm::get_cur_time());
			this->Sleep(1);
			CancelPoint();
		}
	}

private:
	cplib_shared::SharedMem<Counter>* _mem;
};

int main(int argc, char** argv) {
  auto shmem = cplib_shared::SharedMem<Counter>("shmem");
  std::string cmd = "";
  int iparam = 0;

  write("my pid: " + to_string(TimerProgramm::get_pid()), false);
  write(TimerProgramm::get_cur_time());

  IncrementThread th_increment(&shmem);
  DataThread th_data(&shmem);

  th_increment.Start();
  th_data.Start();
	th_increment.WaitStartup();
  th_data.WaitStartup();

  while(true){
    std::cin >> cmd;
		if (cmd == "i" || cmd == "int") {
			std::cin >> iparam;
      shmem.Lock();
      shmem.Data()->count = iparam;
      iparam = shmem.Data()->count;
      shmem.Unlock();
		} else if (cmd == "s" || cmd == "show"){
      shmem.Lock();
      iparam = shmem.Data()->count;
      shmem.Unlock();
      cout << "now count is: " << iparam << "\n";
    } else if (cmd == "e" || cmd == "q" || cmd == "exit" || cmd == "quite") {
			break;
		} else{
			std::cout << "Possible commands: i | s | q\n";
    }
  }

  th_increment.Stop();
  th_data.Stop();
	th_increment.Join();
  th_data.Join();
  return 0;
}

