cmake_minimum_required(VERSION 3.10.0)

project(TimerProject)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_FLAGS "-std=c++20 -Wall -Wextra -O2")
set(SOURCE main.cpp programm.cpp)
set(HEADERS programm.hpp my_shmem.hpp)

add_executable(${PROJECT_NAME} ${SOURCE} ${HEADERS})

IF (NOT WIN32)
    # В POSIX нужно линковаться к rt для shm_open() и к pthread для sem_open()
    TARGET_LINK_LIBRARIES(my_shmem rt pthread)
ENDIF (NOT WIN32)