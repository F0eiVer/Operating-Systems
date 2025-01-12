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
  int cur_writer = -1;
  int cur_copies = 0;
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
    int cur_writer = 0;
		while (true) {
      _mem->Lock();
      count = _mem->Data()->count;
      cur_writer = _mem->Data()->cur_writer;
      if(cur_writer < 0){
        _mem->Data()->cur_writer = TimerProgramm::get_pid();
      }
      _mem->Unlock();

      if(cur_writer == TimerProgramm::get_pid()){
        write("my pid: " + to_string(TimerProgramm::get_pid()), false);
        write("count: " + to_string(count), false);
        write("data: " + TimerProgramm::get_cur_data(), false);
        write(TimerProgramm::get_cur_time());
      }
			this->Sleep(1.0);
			CancelPoint();
		}
	}

private:
	cplib_shared::SharedMem<Counter>* _mem;
};

class CopyThread : public cplib_thread::Thread{
public:
	CopyThread(cplib_shared::SharedMem<Counter>* sh_mem, int argc, char* argv[]) 
  :_mem(sh_mem), _argc(argc), _argv(argv) {}
	virtual void Main() {
    int cur_writer = 0;
    int copies = 0;
		while (true) {
      _mem->Lock();
      cur_writer = _mem->Data()->cur_writer;
      if(cur_writer < 0){
        _mem->Data()->cur_writer = TimerProgramm::get_pid();
      }
      _mem->Unlock();

      if(cur_writer == TimerProgramm::get_pid()) {
        _mem->Lock();
        copies = _mem->Data()->cur_copies;
        _mem->Unlock();
        if(!copies){
          TimerProgramm::create_process(_argc, _argv, (char *)"1");
          _mem->Lock();
          _mem->Data()->cur_copies += 1;
          _mem->Unlock();
          TimerProgramm::create_process(_argc, _argv, (char *)"2");
          _mem->Lock();
          _mem->Data()->cur_copies += 1;
          _mem->Unlock();
        } else {
          write("copies are still working");
        }
      }

			this->Sleep(3.0);
			CancelPoint();
		}
	}

private:
	cplib_shared::SharedMem<Counter>* _mem;
  int _argc;
  char** _argv;
};

int main(int argc, char** argv) {
  auto shmem = cplib_shared::SharedMem<Counter>("shmem");
  
  if(argc < 2){
    int cur_writer = 0;
    shmem.Lock();
    if(shmem.Data()->cur_writer < 0){
      shmem.Data()->cur_writer = TimerProgramm::get_pid();
    }
    cur_writer = shmem.Data()->cur_writer;
    shmem.Unlock();
    std::string cmd = "";
    int iparam = 0;

    if(cur_writer == TimerProgramm::get_pid()){
      write("my pid: " + to_string(TimerProgramm::get_pid()), false);
      write(TimerProgramm::get_cur_time());
    }
    
    IncrementThread th_increment(&shmem);
    DataThread th_data(&shmem);
    CopyThread th_copy(&shmem, argc, argv);

    th_increment.Start();
    th_data.Start();
    th_copy.Start();
    th_increment.WaitStartup();
    th_data.WaitStartup();
    th_copy.WaitStartup();

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
    th_copy.Stop();
    th_increment.Join();
    th_data.Join();
    th_copy.Join();

    shmem.Lock();
    if(shmem.Data()->cur_writer == TimerProgramm::get_pid()){
      shmem.Data()->cur_writer = -1;
    }
    shmem.Unlock();
  } else if (argv[1][0] == '1'){
    write("copy1 pid: " + to_string(TimerProgramm::get_pid()), false);
    write("copy1 start time: " + TimerProgramm::get_cur_time());
    shmem.Lock();
    shmem.Data()->count += 10;
    shmem.Unlock();
    write("copy1 end time: " + TimerProgramm::get_cur_time());

    shmem.Lock();
    shmem.Data()->cur_copies -= 1;
    shmem.Unlock();
  } else if (argv[1][0] == '2') {
    write("copy2 pid: " + to_string(TimerProgramm::get_pid()), false);
    write("copy2 start time: " + TimerProgramm::get_cur_time());
    shmem.Lock();
    shmem.Data()->count *= 2;
    shmem.Unlock();

    TimerProgramm::sleep(2);

    shmem.Lock();
    shmem.Data()->count /= 2;
    shmem.Unlock();
    write("copy2 end time: " + TimerProgramm::get_cur_time());

    shmem.Lock();
    shmem.Data()->cur_copies -= 1;
    shmem.Unlock();
  }

  return 0;
}

