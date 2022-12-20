cmake_minimum_required(VERSION 2.8)
project('assignment-8')

enable_testing()
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall")

add_executable (shell main.c shell.c)

# Check for correct execution
add_test(NAME correctExecution
COMMAND ${PROJECT_SOURCE_DIR}/tests/test-correct-execution.sh
WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tests)

# check that the program correctly resports that exec failed if the program does not exist
add_test(NAME incorrectProgram
COMMAND ${PROJECT_SOURCE_DIR}/tests/test-nonexistent-program.sh
WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tests)

#check if the program terminated correctly 
add_test(NAME correctExitCode
COMMAND ${PROJECT_SOURCE_DIR}/tests/test-correct-exit-code.sh
WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tests)
