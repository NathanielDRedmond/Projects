#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>

static const int FRAME_SIZE = 4096;

// Initializes the module to use this memory block for frames
void memory_init(void* buffer, size_t size);

// Returns the amount of free memory (must be a multiple of FRAME_SIZE)
size_t memory_avail();

// Allocates memory, returned pointer must be FRAME_SIZE aligned
void* memory_alloc(size_t size);

// Frees a previously allocated block of memory
void memory_free(void* ptr);

// Demonstrate fragmentation (has available memory but alloc fails)
void test3();

#endif //MEMORY_H
