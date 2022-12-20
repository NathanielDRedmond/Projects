System Call Analysis

1. execve (const char *pathname, char *const argv[], char *const envp[])
   type - int

Context: execve("./main", ["./main"], 0x7fffeb6f2d0 /* 59 vars */) = 0

- execve is a function that is called to replace the calling program in the current process with a new program whose filepath is indicated in execve's first argument. The first argument is also a string, which is represented by an array of characters in C. In order to access the array, a pointer holds the memory address of the first element of the array. 

- The second argument in execve is an array of pointers to strings and represents command-line arguments for the program. By convention, the first element of the array always points to a string that is equivalent to the one passed to the first argument of the system call. The following elements of the array are all command line arguments. The array must be terminated by a NULL pointer

- The third argument of the system call is also an array of pointers to strings. Once again the array must be terminated by a NULL pointer. The strings in the array represent environmental variables which essentially provide information regarding the directory associated with the program, type of terminal, etc.

- Note that upon success, the system call does NOT return a value. If the system call fails, it returns -1.

- In the case of the example call to execve above, the first passed argument represents the filepath of the program to be executed. In this case the program we want to execute is main.c. The second argument by convention stores the aforementioned filepath as a string which is pointed to by the first element in the array argv. The array does not contain any pointers to command-line arguments, as there is no need for any external values to be passed to such a simple program. The third argument contains the memory address of the first pointer in the array envp. The array contains 59 variables that are used in preparing the environment in which the program will be run.



2. write(int fd, const void *buf, size_t count)
   type - ssize_t

Example from main.c: write(1, "Value of x: 10\n", 15Value of x: 10
) = 15

- write is a system call that writes values to the file (or some other form of I/O device) specified in the first argument fd (file descriptor). Note that fd is an integer and is simply an arbitrary reference to a file in the directory. The bytes to be written to the file are stored in buf, and the number of bytes to be written are stored in count. Note that count begins at 0. 

- If the write system command is successful, the number of bytes that were written to the file referenced by fd are returned. Otherwise, -1 is returned and the cause of failure is indicated in a separate variable. 

- Note that write commands may be successful even when less than count bytes are written to the referenced file. This can happen for various reasons, such as a scenario in which the disk to which the bytes are being written runs out of storage. 

- In the specific example outlined above, the first argument (1) describes where the bytes will be written to (in this case the terminal). The second argument contains the string that shall be written to the terminal, in this case "Value of x: 10\n". The third argument contains the number of bytes that shall be written from the second argument to the terminal. In order for the full string to be written to the terminal, count must be (length of string - 1). In this case the length of the string is 16, and count is 15 so we should expect the full string to be written. The statement following the integer 15 displays the entirety of the written value for visual purposes.



3. mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
   type - void*
   
- The mmap system call is used to map files into memory. The memory address at which mapping will begin is indicated in the first argument addr. If the memory address is NULL the kernel will map the file to a memory address anywhere it sees fit. If the address is not NULL, the kernel will attempt to map the file to a nearby page boundary. If a mapping already exists at the nearby page boundary, the kernel will pick a new address that may or may not be related to the address specified in the first argument. 
The system call returns a pointer to the mapped area upon success.

- The second argument, length, is used to specify the number of bytes that will be mapped to memory from the file. The last argument in the system call specifies an offset that indicates where in the file the mapping should start. The offset must be a multiple of the page size which can be found via the return value of the system call sysconf(_SC_PAGE_SIZE).

- The prot argument essentially provides certain protections to the new mapping. PROT_NONE indicates that the contents of the new mapping may not be accessed at all, although there are certain other protections that allow for the contents to be executed, read, or written. 

- The flags argument specifies whether or not the user wants updates to the mapping to be visible to other processes mapping to the same addresses, and whether updates are seen by the underlying file. For example, the flag MAP_PRIVATE prevents any updates to the mapping from being seen by other processes mapping to the same area of memory, and indicates that these updates will not be accessible by the underlying file. 




Program Disassembly:
- Analyzed using x86 OS disassembly (

Instructions held at memory addresses 11b3-11b7 set up the stack that will contain values to be manipulated in f(). The instruction at 11bb, "mov DWORD PTR [rbp-0x4], 0x0", is used to initialize the variable x of type int.

Prior to this call, rbp (the base pointer register) is pushed to this instance of f()'s stack and is then assigned to point to the memory address of the stack pointer. rbp now represents the top of f()'s stack. Then, 16 is subtracted from the stack pointer to make space on the stack for variables. 

In terms of the initialization of x, the DWORD PTR is a pointer to the memory address stored at [rbp-0x4], which is the second byte address on the stack. The pointer is dereferenced and assigned the value 0. 

Using the same command, the variable i of type int is also assigned the value 0. The value is stored at the third byte address on the stack. 

From here, the conditional statement of the for loop is entered. In assembly this is seen as a jump to the compare statement stored at memory address 188d. The compare statement checks to see if the value held at the third byte address of the stack (variable i) is less than or equal to 9. If this condition is met, then we jump to the body of the for loop, whose first instruction is contained at address 1185. 

At this point we simply add 1 to the variable stored at the second byte address of the stack (x). Then 1 is added to i (variable stored at third byte address in stack). At this point, the value of i is once again compared with 9. If it is indeed less than or equal to 9, the process repeats. Else, the for loop is exited and the jump statement at 1191 is skipped. Note that this same jump statement is also skipped if the initial value of i is greater than 9. In that case, the body of the for loop would never be executed. 

Upon exit of the for loop, the value of x is stored in the register eax. This is necessary if the value of x is to be accessed outside of the function f(), as it was previously only stored in f()'s stack. The value of x is also assigned to a call-preserved register esi. This ensures that the value of x will persist across C function calls, specifically printf() in this case. The value at the memory address (address of rip + 0xe65] is then loaded to the rax register. Rdi is also loaded with this value. Then the printf function is called. When the print function is called, a string is retrieved from some address in memory (address of rip + 0x2f55). The string is outputted, followed by the value held in rax, the variable x. After the function f exits, we return to main and rax is loaded with some point in memory that has no value. A different print function is called that retrieves and outputs a string from some point in memory using the bnd jmp command.




