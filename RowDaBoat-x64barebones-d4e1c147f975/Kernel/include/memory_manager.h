#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H_

#include "memory.h"
#include "occupiedBlocks.h"
/*
    Here baseAddress & endAddress must be word aligned
*/
void pInitHeap(void *baseAddress, void *endAddress);

/*
    Receive a block to be inserted into the free list
*/
void pInsertBlockIntoList(a_block *block);

#endif