cmake_minimum_required(VERSION 2.8)
project(assignment-3)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c99")
add_executable(main main.c segments.c)
