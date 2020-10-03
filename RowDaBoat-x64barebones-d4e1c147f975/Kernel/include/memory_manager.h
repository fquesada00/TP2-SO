#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H_
#include <stdint.h>
#include <stdlib.h>

#define BASE_ADDRESS 0x800000
#define END_ADDRESS 0xC00000
#define HEAP_SIZE (size_t) BASE_ADDRESS - (size_t)END_ADDRESS   
#define WORD_ALIGN 8 
#define WORD_ALIGN_MASK 7

typedef struct a_block
{
    struct a_block *pNextBlock;
    size_t BlockSize;
} a_block;

/*
    Reserve requestedSize bytes in heap
*/
void *pMalloc(size_t);

/*
    Here baseAddress & endAddress must be word aligned
*/
void pInitHeap(void *, void *);

/*
    Receive a pointer to be freed
*/
void pFree(void *);

/*
    Receive a block to be inserted into the free list
*/
void pInsertBlockIntoList(a_block *);

#endif