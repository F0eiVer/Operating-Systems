cmake_minimum_required(VERSION 3.10.0)

project(LibProject)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_FLAGS "-std=c++20 -Wall -Wextra -O2")
set(SOURCE main.cpp)
set(LIB_SOURCE my_lib.cpp)
set(LIB_HEADERS my_lib.hpp)

add_library(mylib SHARED ${LIB_HEADERS} ${LIB_SOURCE})
add_executable(${PROJECT_NAME} ${LIB_HEADERS} ${SOURCE})
target_link_libraries(${PROJECT_NAME} mylib)