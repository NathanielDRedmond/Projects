cmake_minimum_required(VERSION 2.8)
project('assignment-10')

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99 -Wall -g")

add_executable(p2pchat p2pchat.c ui.c)

target_link_libraries(p2pchat pthread)
target_link_libraries(p2pchat form)
target_link_libraries(p2pchat ncurses)
