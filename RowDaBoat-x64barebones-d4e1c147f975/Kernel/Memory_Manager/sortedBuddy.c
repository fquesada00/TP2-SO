#ifdef MM_BUDDY
#include "buddy_memory_manager.h"
#include "memory_manager_lib.h"
#include "standardLib.h"
#include "video_driver.h"
/*
        This buddy memory manager works with lists of blocks
        of different size.
        When setting the minimum and maximum size, its set of
        powers of 2, e.g.: minimum size 2^5 -> 32 bytes and
        maximum size 2^7 -> 128 bytes, then level 0 block is one block
        of 128 bytes and level 2 blocks are 128/32=4 blocks of 32 bytes.
        Obviously, neither minimum nor maximum size can exceed the HEAP_SIZE.
        Continuing with the example, if 32 bytes are requested with pMalloc(32), 
        level 2 list its NOT going to be initialized or even try to search for a 
        free block as every request has one header size (mentioned bellow) before 
        the pointer. In this buddy, the header size is of 16 bytes, so 
        to wrap it up, if a search on last list (level 2 list here) is wanted,
        pMalloc must be called with 16-k bytes, where k goes from 0 to 16.
    */

/*
        LINKS DE INTERES
        http://brokenthorn.com/Resources/OSDev26.html
    */
static a_block *headers[LEVELS + 1];
static char initialized = 0;
static size_t remainingBytes = MAXIMUM_BLOCK_SIZE;

size_t heap_size = MAXIMUM_BLOCK_SIZE;
size_t free_size = remainingBytes;
void *pMalloc(size_t requestedSize)
{
    /*
            If heap is not initialized, then do it, only one time
        */
    if (!initialized)
    {
        initialize();
        initialized = 1;
    }
    requestedSize += HEADER_SIZE;
    size_t level = findLevel(requestedSize);
    /*
            Not enough space exception
        */
    if (level == -1 || requestedSize > remainingBytes)
    {
        return NULL;
    }
    /*
            Must skip header
        */
    void *returnPointer = recursiveMalloc(level) + HEADER_SIZE;
    remainingBytes -= requestedSize;
    free_size = remainingBytes;
    return returnPointer;
}

void *recursiveMalloc(size_t level)
{
    void *returnPointer;
    if (headers[level] == NULL)
    {
        returnPointer = recursiveMalloc(level - 1);
        /*
                Not enough space
            */
        if (returnPointer == NULL)
        {
            return NULL;
        }
        /*
                Obtain block's size of the level
            */
        size_t blockSize = BLOCK_SIZE(level);
        /*
                Now we are in the "child", so it has to be splitted in two
            */
        insertSpecificHeaderIntoList(returnPointer, level);
        insertSpecificHeaderIntoList(returnPointer + blockSize, level);
    }
    return removeHeaderFromList(level);
}

void pFree(void *pointer)
{
    void *toFree = pointer - HEADER_SIZE;
    a_block *blockToFree = (a_block *)toFree;
    recursiveFree(toFree, blockToFree->level);
    remainingBytes += BLOCK_SIZE(blockToFree->level);
    free_size = remainingBytes;
}

void recursiveFree(void *header, size_t level)
{
    void *buddy;
    size_t blockIdx = getBlockNumber(header);
    size_t blockSize = BLOCK_SIZE(level);
    /*
            Check if buddy is on right (blockIdx is odd), if not
            then is on left (bloclkIdx is even)
        */
    if (blockIdx & 1)
        buddy = (uint8_t *)header + blockSize;
    else
        buddy = (uint8_t *)header - blockSize;
    /*
            Insert block in the free list of the level
        */
    insertSpecificHeaderIntoList(header, level);
    a_block *block = headers[level];
    /*
            Lookup to obtain buddy's header from the start of the level
        */
    while (block != NULL)
    {
        if ((void *)block == buddy)
        {
            /*
                    If buddy exists, then remove both of them
                */
            removeSpecificHeaderFromList(header, level);
            removeSpecificHeaderFromList(buddy, level);
            a_block *aux;
            if (blockIdx & 1)
            {
                aux = (a_block *)header;
                aux->level--;
                recursiveFree(header, (level - 1));
            }
            else
            {
                aux = (a_block *)buddy;
                aux->level--;
                recursiveFree(buddy, (level - 1));
            }
            return;
        }
        block = block->pNextFreeBlock;
    }
}

void insertSpecificHeaderIntoList(void *header, size_t level)
{
    a_block *block = headers[level];
    a_block *toInsert = (a_block *)header;
    if(header == NULL){
        return;
    }
    /*
            List is not initialized, then do it
        */
    if (block == NULL)
    {
        headers[level] = toInsert;
        toInsert->level = level;
        toInsert->pNextFreeBlock = NULL;
        return;
    }
    while (block->pNextFreeBlock != NULL && block->pNextFreeBlock < toInsert)
    {
        block = block->pNextFreeBlock;
    }
    /*
            If block has to be inserted at the end, check it doesn't reach end
        */
    if (block->pNextFreeBlock == NULL)
    {
        block->pNextFreeBlock = toInsert;
        toInsert->pNextFreeBlock = NULL;
    }
    else
    {
        toInsert->pNextFreeBlock = block->pNextFreeBlock;
        block->pNextFreeBlock = toInsert;
    }
    block->pNextFreeBlock->level = level;
}

void removeSpecificHeaderFromList(void *header, size_t level)
{
    a_block *block = headers[level];
    a_block *toRemove = (a_block *)header;
    a_block *aux;
    /*
            Maybe toRemove is the first block
        */
    if (block == toRemove)
    {
        headers[level] = block->pNextFreeBlock;
        return;
    }
    /*
            Lookup to obtain the block to remove from the level
        */
    while (block->pNextFreeBlock != NULL)
    {
        if (block->pNextFreeBlock == toRemove)
        {
            block->pNextFreeBlock = toRemove->pNextFreeBlock;
            return;
        }
        aux = block;
        block = block->pNextFreeBlock;
    }
    /*
            Maybe toRemove is the last block
        */
    if (block == toRemove)
    {
        aux->pNextFreeBlock = NULL;
    }
    /*
            If reached here, then the pointer is not in the list.
            Then throw an exception
        */
}

void *removeHeaderFromList(int level)
{
    /*
            If list its not initialized, then there is no element
        */
    if (headers[level] == NULL)
    {
        return NULL;
    }
    void *returnValue = (void *)headers[level];
    headers[level] = headers[level]->pNextFreeBlock;
    return returnValue;
}

int findLevel(size_t requestedBytes)
{
    int level = 0, totalBytes = 1 << MAXIMUM_BLOCK_SIZE_LOG2;
    if (requestedBytes > totalBytes)
        return -1;
    while (level < LEVELS)
    {
        if (requestedBytes < totalBytes)
        {
            totalBytes >>= 1;
            level++;
        }
        else
            return requestedBytes == totalBytes ? level : level - 1;
    }
    return LEVELS - 1;
}

size_t getBlockNumber(a_block *header)
{
    size_t relativeStart = (uint8_t *)header - (uint8_t *)BASE_ADDRESS;
    size_t relativeBlockNumber = relativeStart / BLOCK_SIZE(header->level);
    /*
            Add one so blocks are numbered: 1, 2, 3, 4 ...
            meaning first block is always odd (except the only block
            of the list at level 0, which number is 0)
        */
    return relativeBlockNumber + 1;
}

void initialize()
{
    insertSpecificHeaderIntoList((void *)BASE_ADDRESS, 0);
}
#endif