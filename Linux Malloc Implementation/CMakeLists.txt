cmake_minimum_required(VERSION 2.8)
project('assignment-4')

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall")
add_executable (main main.c memory.c)
