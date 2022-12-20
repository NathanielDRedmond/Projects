cmake_minimum_required(VERSION 2.8)
project('assignment-2')

enable_testing()
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall")

add_executable (shell main.c shell.c)
