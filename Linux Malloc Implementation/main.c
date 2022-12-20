#define _GNU_SOURCE
#include "memory.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void test1()
{
    printf("TEST 1 STARTED\n");

    size_t len1 = memory_avail();
    char* ptr = memory_alloc(len1);
    printf("allocated %ld memory at %p\n", len1, ptr);

    size_t len2 = memory_avail();
    printf("available memory after alloc is %ld\n", len2);

    if (ptr != NULL) {
        printf("overwriting allocated memory\n");
        memset(ptr, 0x11, len1);
    }
    memory_free(ptr);
    printf("memory is freed at %p\n", ptr);

    size_t len3 = memory_avail();
    printf("available memory after free is %ld\n", len3);

    if (ptr != NULL && len2 == 0 && len3 == len1)
        printf("TEST 1 PASSED\n");
    else
        printf("TEST 1 FAILED\n");
}

void test2()
{
    printf("TEST 2 STARTED\n");

    size_t len1 = memory_avail();
    size_t num = len1 / FRAME_SIZE;

    printf("allocating %ld many 1-byte blocks\n", num);
    size_t count = 0;
    void** blocks = malloc(num * sizeof(void*));
    for (size_t i = 0; i < num; i++) {
        blocks[i] = memory_alloc(1);
        if (blocks[i] != NULL) {
            memset(blocks[i], 0x22, FRAME_SIZE); // should be rounded up
            count += 1;
        }
    }
    printf("succesfully allocated %ld blocks\n", count);

    size_t len2 = memory_avail();
    printf("available memory after alloc is %ld\n", len2);

    printf("freeing all allocated blocks\n");
    for (size_t i = 0; i < num; i++)
        memory_free(blocks[i]);

    size_t len3 = memory_avail();
    printf("available memory after free is %ld\n", len3);

    if (count == num && len2 == 0 && len3 == len1)
        printf("TEST 2 PASSED\n");
    else
        printf("TEST 2 FAILED\n");
}

int main(int argc, char* argv[])
{
    size_t len;
    if (argc == 2 && strcmp(argv[1], "rand") == 0) {
        srand(time(NULL));
        len = (rand() % 100000) * FRAME_SIZE;
    } else
        len = 11 * FRAME_SIZE;

    void* ptr = memalign(FRAME_SIZE, len);
    if (ptr == NULL) {
        printf("not enough memory to allocate %ld bytes\n", len);
        exit(1);
    }

    printf("initializing at %p with %ld bytes\n", ptr, len);
    memory_init(ptr, len);

    size_t avail = memory_avail();
    printf("total available memory is %ld, reserved is %ld\n", avail, len - avail);

    test1();
    test2();
    test3();
	
    free(ptr);
}
