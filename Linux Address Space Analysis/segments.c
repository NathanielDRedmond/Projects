#include "segments.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


static char* j;
static char* k;
static char dataseg1[8] = "data 1\0";
static char dataseg2[8] = "data 2\0";

char* textSeg1() {
	return "text 1";
}

char* textSeg2() {
	return "text 2";
}

void stackFunc2() {
	char* tempStack2;
	stack_ptr_2 = (char*) &tempStack2;
	strcpy(stack_ptr_2, "stack 2");
}

void stackFunc1() {
	char* tempStack1;
	stack_ptr_1 = (char*) &tempStack1;
	strcpy(stack_ptr_1, "stack 1");
	stackFunc2();
}	

void init_pointers()
{
	text_ptr_1 = &textSeg1;
	text_ptr_2 = &textSeg2;

	// Allocate space for bss (unitialized data) pointers then initialize
	bss_ptr_1 = (char*) &j;
	bss_ptr_2 = (char*) &k;
	strcpy(bss_ptr_1, "bss 1");
	strcpy(bss_ptr_2, "bss 2");

	// Allocate space for and intialize initialized data pointers
	data_ptr_1 = (char*)&dataseg1;
	data_ptr_2 = (char*)&dataseg2;
	
	// Allocate memory for and initialize heap pointers
	heap_ptr_1 = (char*) malloc(6 * sizeof(char));
	heap_ptr_2 = (char*) malloc(6 * sizeof(char));
	char* tempHeap1 [7];
	char* tempHeap2 [7];
	strcpy((char*) tempHeap1, "heap 1");
	strcpy((char*) tempHeap2, "heap 2");
	memcpy(heap_ptr_1, (char*) tempHeap1, 8 * sizeof(char));
	memcpy(heap_ptr_2, (char*) tempHeap2, 8 * sizeof(char));

	// Initialize stack pointers
	stackFunc1();	
}

void free_pointers()
{
	// Note that only heap ptrs need to be free as they are the only ones
	// assigned to memory that was dynamically allocated.
	free(heap_ptr_1);
	free(heap_ptr_2);
	heap_ptr_1 = NULL;
	heap_ptr_2 = NULL;

	bss_ptr_1 = NULL;
	bss_ptr_2 = NULL;

	text_ptr_1 = NULL;
	text_ptr_2 = NULL;
	
	data_ptr_1 = NULL;
	data_ptr_2 = NULL;
	
	stack_ptr_1 = NULL;
	stack_ptr_2 = NULL;
}
