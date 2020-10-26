#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdint.h>
#include <stddef.h>

#define BASE_ADDRESS (size_t) 0x1000000
#define END_ADDRESS (size_t) 0x2800000
#define HEAP_SIZE (size_t) END_ADDRESS - (size_t)BASE_ADDRESS 

#define WORD_ALIGN 8 
#define WORD_ALIGN_MASK 7

/*
    Reserve requestedSize bytes in heap
*/
void *pMalloc(size_t requestedSize);

/*
    Receive a pointer to be freed
*/
void pFree(void *pointer);


#endif