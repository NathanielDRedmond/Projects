#include "memory.h"
#include <stdio.h>
#include <sys/mman.h>

// My implementation uses a free list based on the one described in chapter 17
// of the TLPI textbook.

// While this may not be the most efficient solution to this problem,
// it is my understanding that physical memory is represented
// virtually in most operating systems with a linked list
// structure called a Free List. I wanted to challenge myself
// to complete this assignment in a way consistent with what I
// learned from the textbook.


// Struct for free list from TLPI textbook
typedef struct __node_t {
	int isFree;
	int allocFlag;
	struct __node_t *next;
} node_t;
size_t round_up(size_t size)
{
    return (size + FRAME_SIZE - 1) / FRAME_SIZE * FRAME_SIZE;
}

// Declaration/initialization of global vars
size_t num_frames = 0;
char* info_ptr = NULL;
void* data_ptr = NULL;
node_t** memoryTracker = NULL;
node_t* block1;

void memory_init(void* buffer, size_t size)
{
    
    size_t info_per_frame = sizeof(node_t*); // used to be = sizeof(char)

    num_frames = size / FRAME_SIZE; // initial estimate
    info_ptr = buffer;
    data_ptr = (void*)info_ptr + round_up(info_per_frame * num_frames * 16);
    num_frames = (buffer + size - data_ptr) / FRAME_SIZE; // real value
    printf("num of frames: %ld\n", num_frames);
    

    memoryTracker = (void*) info_ptr;
    
    // Set up linked list (bookkeeping structure)
    block1 = (((void*) memoryTracker) + sizeof(memoryTracker));
    (*memoryTracker) = block1;
    node_t* cur = block1; 
    block1->isFree = 1;
    
    for (int i = 1; i < num_frames; ++i) {
    	node_t* temp;
    	temp = cur + info_per_frame;
    	cur->next = temp;
    	temp->isFree = 1;
    	temp->next = NULL;
    	cur = temp;
    }
}

// Determines how much (non-contiguous) memory is available
size_t memory_avail()
{
	node_t* tempNode = block1;
    int free_count = 0;
    for (int i=0; i<num_frames; i++) {
    	if (tempNode->isFree) free_count++;
    	tempNode = tempNode->next;
    }
    tempNode = NULL;
    return (FRAME_SIZE * free_count);
}

// Allocates the amount of memory requested by user
// Note that memory is allocated in groups of 4096 bytes
// If memory requested is not a multiple of 4096, memory requested is rounded up
// If memory requested is GT memory available, returns null
// If memory requested can not be represented contiguously, returns null
// Uses nodes in a linked list to keep track of which blocks of memory
// are free and which blocks are not free
void* memory_alloc(size_t size)
{
	if (size > memory_avail()) {
		printf("Not enough memory to process request\n");
		return NULL;
	}
	else if (size == 0) return NULL;
	
	// Initializtion for following constructs
	node_t* tempNode = block1;
	node_t* startNode;
    size_t required_frames = size / FRAME_SIZE;
    size_t contiguous_frames = 0;
    size_t count = 0;
    if ((size % FRAME_SIZE) != 0) required_frames++;
    
    // Keeps a count of how many blocks are contiguous at any given time
    // to determine whether a contiguous block of memory of size can be 
    // allocated
    // Also keeps track of which block represents the first block of
    // contiguous memory so that this address can be returned later
    while (tempNode != NULL) {
    	if (tempNode->isFree) {
    	
    		// start node represents the first node on our block of
    		// contiguous memory
    		if (contiguous_frames == 0) startNode = tempNode;
    		contiguous_frames++;
    		
    		// break here because we have already found a suitable block
    		if (contiguous_frames == required_frames) break;
    		
    		tempNode = tempNode->next;
    	}
    	
    	// Reset
   		else {
   			contiguous_frames = 0;
   			tempNode = tempNode->next;
   			count++;
   		}
   	}
   	
   	// If there is not a block of contiguous memory that can hold
   	// the amount of memory requested, returns null
   	if (contiguous_frames != required_frames) {
   		printf("Not enough contiguous memory for request\n");
   		return NULL;
   	}
   	
   	// Determines the address of the first node in contiguous memory
    void* startAddress = (count * FRAME_SIZE) + data_ptr;
    
    // Performs bookkeeping duties in linked list
    // allocFlag is high in the last block of contiguous memory
    // so memory_free knows which is the final block it must free
    while (required_frames > 0) {
   		if (required_frames == 1) tempNode->allocFlag = 1;
    	startNode->isFree = 0;
    	startNode = startNode->next;
    	required_frames--;
    }
    return startAddress;
}

void memory_free(void* ptr)
{
	if (ptr == NULL) {
		printf("Cannot request to free memory from a null pointer\n");
		return;
	}
	
	// Tells us where to begin freeing contiguous memory
	int mem_block_num = (ptr - data_ptr) / FRAME_SIZE;
	node_t* tempNode = block1;
	
	// must iterate through linked list to find first block of
	// contiguous memory
	for (int i=0; i<mem_block_num; i++) {
		tempNode = tempNode->next;		
	}
	
	// Here is where the actual freeing takes place
	// We know we have freed the last block when allocFlag is high
	while (tempNode->allocFlag != 1) {
		tempNode->isFree = 1;
		tempNode = tempNode->next;
	}
	
	// Just performs some cleanup
	tempNode->allocFlag = 0;
	tempNode->isFree = 1;
	tempNode = NULL;
}

// Tests the functionality of our allocator with fragmentation
void test3()
{
	printf("TEST 3 STARTED\n");
	
	size_t len1 = memory_avail();
	char* ptr = memory_alloc(len1 / 2 - FRAME_SIZE);
	printf("Allocated %ld memory at %p\n", len1 / 2 - FRAME_SIZE, ptr);
	
	printf("We will now allocate one block of memory\n");
	size_t len2 = 1;
	char* ptr2 = memory_alloc(len2);
	printf("Allocated %ld memory at %p\n", len2, ptr2);
	printf("Memory available after allocating half of total memory: %ld\n", memory_avail());
	
	printf("We will now allocate the remaining memory.\n");
	size_t len3 = memory_avail();
	char* ptr3 = memory_alloc(len3);
	printf("Allocated %ld memory at %p\n", len3, ptr3);
	
	if (memory_avail() != 0) {
		printf("TEST 3 FAILED\n");
		return;
	}
	
	printf("Freeing the first block of allocated memory\n");
	memory_free(ptr);
	printf("memory freed at %p\n", ptr);
	memory_free(ptr3);
	printf("memory freed at %p\n", ptr3);
	printf("memory available after frees is %ld\n", memory_avail());
	
	printf("Attempting to allocate remaining available memory\n");
	memory_alloc(memory_avail());
	
	printf("Testing memory_alloc(0)\n");
	if (memory_alloc(0) != NULL) {
		printf("TEST 3 FAILED");
		return;
	}
	else printf("memory_alloc(0) exhibits expected behavior\n");
	
	memory_free(NULL);
	
	printf("TEST 3 ENDED\n");
}
