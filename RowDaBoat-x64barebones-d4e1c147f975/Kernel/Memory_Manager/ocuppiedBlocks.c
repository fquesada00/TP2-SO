#include "occupiedBlocks.h"

a_block occupiedBlockList;

void insertBlock(a_block *insert)
{
    a_block *start = occupiedBlockList.nextBlock;
    occupiedBlockList.nextBlock = insert;
    insert->nextBlock = start;
}

int removeBlock(a_block *remove)
{
    a_block *start = occupiedBlockList.nextBlock;
    if (start == NULL || (a_block *)BASE_ADDRESS > remove || (a_block *)END_ADDRESS < remove)
    {
        return -1; 
    }
    if (start == remove)
    {
        occupiedBlockList.nextBlock = start->nextBlock;
        return 1;
    }
    while (start->nextBlock != NULL && start->nextBlock != remove)
    {
        start = start->nextBlock;
    }

    if (start->nextBlock == NULL)
    {
        return -1;
    }

    start->nextBlock = remove->nextBlock;
    return 1;
}