cmake_minimum_required(VERSION 3.10.0)

project(Log)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_FLAGS "-std=c++20 -Wextra -O2")

set(MAIN_SOURCE main.cpp)
set(GEN_SOURCE temp_generator.cpp)
set(HEADERS my_serial.hpp)

add_executable(gen ${GEN_SOURCE} ${HEADERS})
add_executable(${PROJECT_NAME} ${MAIN_SOURCE} ${HEADERS})