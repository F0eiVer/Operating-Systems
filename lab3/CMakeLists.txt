cmake_minimum_required(VERSION 3.10.0)

project(TimerProject)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_FLAGS "-std=c++20 -Wall -Wextra -O2")
set(SOURCE main.cpp my_lib.cpp)
set(HEADERS my_lib.hpp)

add_executable(${PROJECT_NAME} ${SOURCE} ${HEADERS})