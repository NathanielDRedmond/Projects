# Assignment 3

**Due: September 19, 2022 by 11:59 PM US Central Time**. Push to GitHub to submit the assignment.

In this assignment you have to verify the layout of the address space of a process on the x86.

## Tasks

* Define variables and functions in the `segments.c` file such that your compiler will store (allocate space for) these variables and functions in the text, data, bss, heap, and stack segments. The allocated space must contain a string (for the text segment it must contain a function that returns a string) that describes the type of the segment (see the sample output).

* Implement the `init_pointers` function and initialize the pointers declared in the `segments.h` file to point to the allocated space. You need to initialize two pointers for each segment type to verify how that segment grows. In particular, make sure that you have nested function calls for the initialization of the stack segment pointers.

* Implement the `free_pointers` function to free memory that was allocated in `init_pointers` and needs to be freed.

* Run you program several times and verify that the heap start address (start_brk) is random and that the heap grows upwards, the start stack address is also random and it grows downwards.

## Testing your code

Create the build directory, compile and run your program

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./main
```

You should see an output that looks something like this:

```
$ ./main
text ptr 1 pointer: 0x400f57, return value: text 1
text ptr 2 pointer: 0x400f62, return value: text 2
data ptr 1 pointer: 0x602058, string value: data 1
data ptr 2 pointer: 0x60205f, string value: data 2
bss ptr 1 pointer: 0x6020c6, string value: bss 1
bss ptr 2 pointer: 0x6020c0, string value: bss 2
heap ptr 1 pointer: 0x18b3010, string value: heap 1
heap ptr 2 pointer: 0x18b3030, string value: heap 2
stack ptr 1 pointer: 0x7ffc4a49ea10, string value: stack 1
stack ptr 2 pointer: 0x7ffc4a49e9f0, string value: stack 2
``` 

## Evaluation

Your assignment will be graded according to the following criteria:

- **100 points**: 20 points for the correct definition/ allocation/ initialization/ deallocation of string buffers for each of the five segment types.
- **5 extra points**: if your can arrange that your second heap pointer is always the same number (or at least the last 4 hexadecimal digits are).
- **5 extra points**: if you can arrange that your second stack pointer is always the same number (or at least the last 4 hexadecimal digits are).
