#ifndef __SEGMENTS_H
#define __SEGMENTS_H

typedef char* (*charfunc_t)();
extern charfunc_t text_ptr_1;
extern charfunc_t text_ptr_2;

extern char* data_ptr_1;
extern char* data_ptr_2;

extern char* bss_ptr_1;
extern char* bss_ptr_2;

extern char* heap_ptr_1;
extern char* heap_ptr_2;

extern char* stack_ptr_1;
extern char* stack_ptr_2;

void init_pointers();
void free_pointers();

#endif //__SEGMENTS_H
