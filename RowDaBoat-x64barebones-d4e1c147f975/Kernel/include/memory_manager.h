#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H_
#include <stdint.h>
#include <stdlib.h>

#define CTE (size_t)1000000 * 4      //store 4mb MODIFICAR ESTO
#define MINIMUN_BLOCK_SIZE (size_t)8 //8 bytes
#define WORD_ALIGN 8 
#define WORD_ALIGN_MASK 7

typedef struct a_block
{
    struct a_block *pNextBlock;
    size_t BlockSize;
} a_block;

void *pMalloc(size_t);
void pInitHeap(void *, void *);
void pFree(void *);
void pInsertBlockIntoList(a_block *);

#endif