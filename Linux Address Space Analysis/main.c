#include "segments.h"
#include <stdio.h>
#include <string.h>

charfunc_t text_ptr_1 = NULL;
charfunc_t text_ptr_2 = NULL;

char* data_ptr_1 = NULL;
char* data_ptr_2 = NULL;

char* bss_ptr_1 = NULL;
char* bss_ptr_2 = NULL;

char* heap_ptr_1 = NULL;
char* heap_ptr_2 = NULL;

char* stack_ptr_1 = NULL;
char* stack_ptr_2 = NULL;

int main(int argc, char* argv[])
{
    init_pointers();

    // copy the text from the stack pointers b/c they will be overwritten
    char temp1[100];
    if (stack_ptr_1 != NULL)
        memcpy(temp1, stack_ptr_1, 100);

    char temp2[100];
    if (stack_ptr_2 != NULL)
        memcpy(temp2, stack_ptr_2, 100);

    printf("text ptr 1 pointer: %p, return value: %s\n", (void*)text_ptr_1, text_ptr_1 != NULL ? (*text_ptr_1)() : "(null)");
    printf("text ptr 2 pointer: %p, return value: %s\n", (void*)text_ptr_2, text_ptr_2 != NULL ? (*text_ptr_2)() : "(null)");
    printf("data ptr 1 pointer: %p, string value: %s\n", data_ptr_1, data_ptr_1);
    printf("data ptr 2 pointer: %p, string value: %s\n", data_ptr_2, data_ptr_2);
    printf("bss ptr 1 pointer: %p, string value: %s\n", bss_ptr_1, bss_ptr_1);
    printf("bss ptr 2 pointer: %p, string value: %s\n", bss_ptr_2, bss_ptr_2);
    printf("heap ptr 1 pointer: %p, string value: %s\n", heap_ptr_1, heap_ptr_1);
    printf("heap ptr 2 pointer: %p, string value: %s\n", heap_ptr_2, heap_ptr_2);
    printf("stack ptr 1 pointer: %p, string value: %s\n", stack_ptr_1, stack_ptr_1 != NULL ? temp1 : "(null)");
    printf("stack ptr 2 pointer: %p, string value: %s\n", stack_ptr_2, stack_ptr_2 != NULL ? temp2 : "(null)");

    free_pointers();
}
