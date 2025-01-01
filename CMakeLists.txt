cmake_minimum_required(VERSION 3.10.0)

project(hello)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_FLAGS "-std=c++20 -Wall -Wextra -O2")

add_executable(${PROJECT_NAME} main.cpp)