Assignment 1
============

**Due: September 5, 2022 by 11:59 PM US Central Time**. Push to GitHub to submit the assignment.

This is an introductory assignment to help you get comfortable with various tools we'll be using throughout the semester, including git, CMake, make, gcc, objdump, and strace. A small program is provided for you in main.c. You're going to compile, run, disassemble, and examine the system calls this program makes.

To build and run this program, do the following.

1. Clone your repo
2. From the root directory of your repo, create a build directory: `mkdir build`
3. Go to the build directory: `cd build`
4. Generate the build files by running CMake: `cmake ..`. The `..` tells CMake to look for a CMakeLists.txt file in the parent directory.
5. Generate the executable by running make: `make`
6. Run the program: `./main`

## Tasks

* Add a file named `report.md` to your repo.
* Use the strace command (`strace ./main`) to examine the system calls that the program makes when it runs. Pick three system calls that the program makes, including `execve`, and describe their arguments and what they do in your `report.md`. You can use the `man` pages to find information about system calls (for example, `man 2 execve`).
* Use `objdump` to examine the assembly language instructions for the program. Look for the disassembly of the function `f` and explain how (1) the loop iteration works, (2) how the increment of the variable x works, and (3) how arguments are passed to the ``printf()`` function. For example, use the command `objdump -d -M intel main` to get the disassembly of your program on an x86 OS.

## Evaluation

Your assignment will be graded according to the following criteria:

- **50 points** For your explanation of the system calls.
- **50 points** For your explanation of the assembly language instructions.
