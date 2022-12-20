# CMake generated Testfile for 
# Source directory: /home/nathanr/assignment-8-nathanredmond
# Build directory: /home/nathanr/assignment-8-nathanredmond/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(correctExecution "/home/nathanr/assignment-8-nathanredmond/tests/test-correct-execution.sh")
set_tests_properties(correctExecution PROPERTIES  WORKING_DIRECTORY "/home/nathanr/assignment-8-nathanredmond/tests" _BACKTRACE_TRIPLES "/home/nathanr/assignment-8-nathanredmond/CMakeLists.txt;10;add_test;/home/nathanr/assignment-8-nathanredmond/CMakeLists.txt;0;")
add_test(incorrectProgram "/home/nathanr/assignment-8-nathanredmond/tests/test-nonexistent-program.sh")
set_tests_properties(incorrectProgram PROPERTIES  WORKING_DIRECTORY "/home/nathanr/assignment-8-nathanredmond/tests" _BACKTRACE_TRIPLES "/home/nathanr/assignment-8-nathanredmond/CMakeLists.txt;15;add_test;/home/nathanr/assignment-8-nathanredmond/CMakeLists.txt;0;")
add_test(correctExitCode "/home/nathanr/assignment-8-nathanredmond/tests/test-correct-exit-code.sh")
set_tests_properties(correctExitCode PROPERTIES  WORKING_DIRECTORY "/home/nathanr/assignment-8-nathanredmond/tests" _BACKTRACE_TRIPLES "/home/nathanr/assignment-8-nathanredmond/CMakeLists.txt;20;add_test;/home/nathanr/assignment-8-nathanredmond/CMakeLists.txt;0;")
