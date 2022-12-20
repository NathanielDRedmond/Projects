Assignment 4
============

**Due: September 26, 2022 11:59 PM US Central Time**. Push to GitHub to submit the assignment.

In this assignment you are creating a simple memory allocator that might be used in an OS kernel to allocate physical memory. 
We assume that frames are 4096 bytes long and their addresses are always aligned to 4096 bytes (the lower 3 hexadecimal digits of their addresses are always zero). All allocation sizes must be rounded up to a multiple of 4096 bytes, so allocating 4097 bytes should result in the allocation of 8192 bytes.

Your allocator will be initialized using `memory_init` with a reserved memory area. Since we are simulating how physical memory works (and is mapped into user space) you are not allowed to store any control data (such as block headers commonly used in the implementation of malloc) in or near the memory area you return from `memory_alloc`. Instead, we divide up the reserved memory in `memory_init` into two areas: one for book keeping, and the other for actually servicing the allocations from.

A test harness is already written for you in `main.c`. You need to complete the functions in `memory.c` to actually allocate and free memory as directed by the tests and report the available amount of free memory. A correct execution of the test program will look something like this:

```
daniel@hawkins:~/cs3281/assignment-4/build$ ./main
initializing at 0x556bd3cdf000 with 45056 bytes
total available memory is 40960, reserved is 4096
TEST 1 STARTED
allocated 40960 memory at 0x556bd3ce0000
available memory after alloc is 0
overwriting allocated memory
memory is freed at 0x556bd3ce0000
available memory after free is 40960
TEST 1 PASSED
TEST 2 STARTED
allocating 10 many 1-byte blocks
succesfully allocated 10 blocks
available memory after alloc is 0
freeing all allocated blocks
available memory after free is 40960
TEST 2 PASSED
```

## Tasks

* Finish the implementation `memory_init`. The logic that divides up the reserved memory area is already done for you, you need to specify the amount of extra memory you need for book keeping for each frame, and actually do the initialization.
* Write the implementation of `memory_alloc`, `memory_free` and `memory_avail`. All addresses must be 4096 byte aligned and all memory allocation sizes must be rounded up. Make sure that `memory_alloc(0)` returns `NULL` and `memory_free(NULL)` does not crash.
* Implement the `test3` test which should demonstrate memory fragmentation (you cannot allocate memory of the size returned by `memory_avail`) by allocating and freeing memory in some pattern. This is not implemented in the sample output above.

## Evaluation

Your assignment will be graded according to the following criteria:

- **20 points** for the design of the control structure used for book keeping the usage of frames and for finishing the implementation of `memory_init`.
- **20 points** for a correct implementaton of `memory_alloc`.
- **20 points** for a correct implementaton of `memory_free`.
- **20 points** for a correct implementaton of `memory_avail`.
- **20 points** for a correct implementaton of `test3` for your allocator.
- **10 extra points** if you write a buddy allocator.
