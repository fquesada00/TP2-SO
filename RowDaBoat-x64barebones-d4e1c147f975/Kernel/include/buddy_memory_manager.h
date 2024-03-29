#ifndef _BUDDY_MEMORY_MANAGER_H_
#define _BUDDY_MEMORY_MANAGER_H_

#include "memory.h"

/*
    Set minimun size of block
*/
#define MINIMUM_BLOCK_SIZE_LOG2 5                                 
#define MINIMUM_BLOCK_SIZE ((size_t)1 << MINIMUM_BLOCK_SIZE_LOG2) 

/*
    Set maximum size of block
*/
#define MAXIMUM_BLOCK_SIZE_LOG2 25
#define MAXIMUM_BLOCK_SIZE ((size_t)1 << MAXIMUM_BLOCK_SIZE_LOG2)

/*
    Set number of lists (or levels of the tree)
*/
#define LEVELS (MAXIMUM_BLOCK_SIZE_LOG2 - MINIMUM_BLOCK_SIZE_LOG2) + 1

#define HEADER_SIZE sizeof(a_block)

#define BLOCK_SIZE(level) (size_t)(1 << (MAXIMUM_BLOCK_SIZE_LOG2 - level))

/*
    Auxiliar function to iterate through each list
*/
void *recursiveMalloc(size_t level);

/*
    Auxiliar function to iterate through each list
*/
void recursiveFree(void *header, size_t level);

/*
    Insert a header into its right list level position, so list
    is sorted
*/
void insertSpecificHeaderIntoList(void *header, size_t level);

/*
    Remove an especific header from its level list, so list
    remains sorted
*/
void removeSpecificHeaderFromList(void *header, size_t level);

/*
    Remove first header of the level list
*/
void *removeHeaderFromList(int level);

/*
    Obtain level of n bytes
*/
int findLevel(size_t requestedBytes);

/*
    Obtain the block number of a header from an specific
    level list
*/
size_t getBlockNumber(a_block *header);

/*
    Initialize heap and its first list of level 0
*/
void initialize();

#endif