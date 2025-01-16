cmake_minimum_required(VERSION 3.10.0)

project(Log)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_FLAGS "-std=c++20 -Wextra -O2")

set(MAIN_SOURCE main.cpp help_programm.cpp sqlite3.c)
set(GEN_SOURCE temp_generator.cpp)
set(SERVER_SOURCE server.cpp sqlite3.c)
set(HEADERS my_serial.hpp my_thread.hpp help_programm.hpp sqlite3.h)

add_executable(gen ${GEN_SOURCE} ${HEADERS})
add_executable(${PROJECT_NAME} ${MAIN_SOURCE} ${HEADERS})
add_executable(server ${SERVER_SOURCE} ${HEADERS})

IF (WIN32)
    TARGET_LINK_LIBRARIES(server ws2_32)
ENDIF(WIN32)