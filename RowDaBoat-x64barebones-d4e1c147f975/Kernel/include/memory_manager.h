#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H_

#include <stdint.h>
#include <stdlib.h>
#include "memory.h"

#define END_ADDRESS 0xC00000
#define HEAP_SIZE (size_t) BASE_ADDRESS - (size_t)END_ADDRESS   
#define WORD_ALIGN 8 
#define WORD_ALIGN_MASK 7

typedef struct a_block
{
    struct a_block *pNextBlock;
    size_t blockSize;
} a_block;

/*
    Here baseAddress & endAddress must be word aligned
*/
void pInitHeap(void *baseAddress, void *endAddress);

/*
    Receive a block to be inserted into the free list
*/
void pInsertBlockIntoList(a_block *block);

#endif