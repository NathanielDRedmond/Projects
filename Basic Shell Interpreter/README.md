Assignment 2
============

**Due: September 12, 2022 11:59 PM US Central Time**. Push to GitHub to submit the assignment.

In this assignment you are building a basic shell interpreter. It should read a command to be executed along with the command line arguments. To finish this assignment you will need to understand the usage of following systems calls:

- fork
- exec
- wait
- exit

Reading the input is already written for you in `main.c`, you need to complete the `execute` function in `shell.c` to actually execute the command entered. You should create the `build` directory and compile there with `cmake` as usual. A correct execution of the shell program will look something like this:

```
daniel@upsidedown:~/work/assignment-2/build$ ./shell
*** Welcome to the CS3281 Shell
CS3281$ ls -l
*** Entered: ls -l (2 words)
total 56
-rw-r--r-- 1 daniel daniel 13304 Sep  5 21:07 CMakeCache.txt
drwxr-xr-x 5 daniel daniel  4096 Sep  5 21:07 CMakeFiles
-rw-r--r-- 1 daniel daniel  1512 Sep  5 21:07 cmake_install.cmake
-rw-r--r-- 1 daniel daniel   879 Sep  5 21:07 CTestTestfile.cmake
-rw-r--r-- 1 daniel daniel  5608 Sep  5 21:07 Makefile
-rwxr-xr-x 1 daniel daniel 17536 Sep  5 21:07 shell
*** Child exited sucessfully
CS3281$ something
*** Entered: something (1 words)
*** Command not found: something
*** Child exited with 1
CS3281$ ls something
*** Entered: ls something (2 words)
ls: cannot access 'something': No such file or directory
*** Child exited with 2
CS3281$ exit
*** Closing the CS3281 Shell
```

## Tasks

* Finish the implementation of the `execute` function in `shell.c`. Do not modify, change or free the `args` array of input words.
* Select the appropriate `exec` function from the possibilities (see `man exec`). Use the manuals for the required system calls to find the appropriate include files. You might need `#include <sys/types.h>` to access the definition of `pid_t`.
* Make sure you print "Fork failed" if the fork function failed.
* Make sure you print "Command not found" if the exec function failed.
* Make sure you print "Command exited successfully" if the command terminated with a zero status code.
* Make sure you print "Command exited with" if the command terminated with a non-zero status code.

## Evaluation

Your assignment will be graded according to the following criteria:

- **25 points** for the correct usage of and error handling for the `fork` system call.
- **25 points** for the correct usage of and error handling for the `exec` system call.
- **25 points** for the correct usage of and error handling for the `wait` system call.
- **25 points** for the correct and appropriate usage of the `exit` system call.
