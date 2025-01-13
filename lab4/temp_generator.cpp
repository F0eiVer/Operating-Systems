#include "my_serial.hpp"
// #include <sstream>              // std::stringstream
#include <iostream>             // std::cout
#include <ctime>

#if !defined (WIN32)
#	include <unistd.h>          // pause()
#	include <time.h>            // nanosleep()
#endif

std::string OUT_PORT = "COM4";

// // Сконвертировать любой базовый тип в строку
// template<class T> std::string to_string(const T& v)
// {
//     std::ostringstream ss;
//     ss << v;
//     return ss.str();
// }

void csleep(double timeout) {
#if defined (WIN32)
	if (timeout <= 0.0)
        ::Sleep(INFINITE);
    else
        ::Sleep((DWORD)(timeout * 1e3));
#else
    if (timeout <= 0.0)
        pause();
    else {
        struct timespec t;
        t.tv_sec = (int)timeout;
        t.tv_nsec = (int)((timeout - t.tv_sec)*1e9);
        nanosleep(&t, NULL);
    }
#endif
}

double f_rand(double fMin, double fMax){
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

int main(int argc, char** argv)
{	
    srand(time(0));
    cplib::SerialPort smport(OUT_PORT, cplib::SerialPort::BAUDRATE_115200);
    if (!smport.IsOpen()) {
        std::cout << "Failed to open port \n";
        return -2;
    }

    std::string mystr;
    while(true) {
        smport << std::to_string(f_rand(-30.0, 30.0));
        csleep(1.0);
    }
    return 0;
}