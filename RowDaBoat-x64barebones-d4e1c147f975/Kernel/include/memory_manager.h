#if !defined MM_BUDDY && !defined MM_BUDDY2
#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H_

#include "memory.h"

typedef struct a_block
{
    struct a_block *pNextFreeBlock;
    struct a_block *pNextBlocked;
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

/*
    Receive a block and insert it into the blocked list
*/
void insertBlock(a_block *insert);

/*
    Receive a block and try to remove it from the blocked list.
    Return value on success is 1, if not, 0
*/
int removeBlock(a_block *remove);

/*
    Receive a block and check if its present into the blocked list.
    Return value on success is 1, if not, 0
*/
int isBlocked(a_block *block);


#endif
#endif