#include <stdio.h>
#include <stdlib.h>

#define CTE (size_t)1000000 * 4      //store 4mb MODIFICAR ESTO
#define MINIMUN_BLOCK_SIZE (size_t)8 //8 bytes
typedef struct A_BLOCK
{
    a_block *pNextBlock;
    size_t BlockSize;
} a_block;

//chequear mascara y alineamiento
static const size_t heapHeaderSize = sizeof(a_block) & (size_t)-2;
static size_t remainingBytes = CTE;
static a_block heapStart, *heapEnd = NULL;

/*
    LINKS DE INTERES
    https://courses.cs.washington.edu/courses/cse351/10sp/lectures/15-memallocation.pdf
    https://www.youtube.com/watch?v=74s0m4YoHgM
    https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/master/portable/MemMang/heap_4.c
    using implicit free list w/ first fit algorithm 
*/

void *pMalloc(size_t);
void pInitHeap(void *, void *);
void pFree(void *);
void pInsertBlockIntoList(a_block *);

void *pMalloc(size_t requestedSize)
{

    //if heap not initialize, do it
    if (heapEnd == NULL)
    {
        //MODIFICAR ESTO CON CTES ESTARIA COPADO
        pInitHeap(0x800000, 0xC000000);
    }
    else
    {
        if (remainingBytes < requestedSize)
        {

        } //throw exception not enough space
        if (requestedSize < 0)
        {

        } //throw exception negative sizes not accepted

        //passed controls then go on w/ first fit
        a_block *pPrevBlock, *pActualBlock;
        void *pReturnBlock;

        pPrevBlock = &heapStart;
        pActualBlock = pPrevBlock->pNextBlock;

        //find available block
        while (pActualBlock->BlockSize < requestedSize && pActualBlock->pNextBlock != NULL)
        {
            pPrevBlock = pActualBlock;
            pActualBlock = pActualBlock->pNextBlock;
        }

        if (pActualBlock->pNextBlock == NULL)
        {

        } //throw exception not enough space (reached end)

        pReturnBlock = heapHeaderSize + (void *)pActualBlock;

        //block must be taken out as its going to be used
        pPrevBlock->pNextBlock = pActualBlock->pNextBlock;

        //partition to use more than 1 block
        //TODO
        if ((pActualBlock->BlockSize - requestedSize) > MINIMUN_BLOCK_SIZE)
        {

            a_block *pPartitionedBlock = NULL;
            insertBlockIntoList(pPartitionedBlock);
        }

        //supongo que salio todo ok
        remainingBytes -= pActualBlock->BlockSize;
        pActualBlock->pNextBlock = NULL;
    }
}

//baseAddress & endAddress must be 8 byte aligned
void pInitHeap(void *baseAddress, void *endAddress)
{

    //set start header static value
    heapStart.pNextBlock = (a_block *)baseAddress;
    heapStart.BlockSize = (size_t)0;

    //set end having set start
    heapEnd = endAddress - heapHeaderSize;
    heapEnd->pNextBlock = NULL;
    heapEnd->BlockSize = (size_t)0;

    /* initialize first block
        his size is the diff where end begins and firstBlock + is size starts
    */
    a_block *pFirstBlock = (a_block *)baseAddress;
    pFirstBlock->pNextBlock = heapEnd;
    pFirstBlock->BlockSize = (size_t)heapEnd - pFirstBlock;

    remainingBytes = pFirstBlock->BlockSize;
}

void pFree(void *pointer)
{

    size_t *pNextABlock = (size_t *)pointer;
    a_block *pToLink;


    if (pNextABlock == NULL)
    {

    } //nothing to do here

    //before pointer its an struct
    pNextABlock -= heapHeaderSize;
    pToLink = (a_block *)pNextABlock;

    if (pToLink->pNextBlock != NULL)
    {

    } //we did something wrong so

    remainingBytes += pToLink->BlockSize;

    insertBlockIntoList(pToLink);
    
}

void insertBlockIntoList(a_block *pInsertBlock)
{

    //iterate through the list
    a_block *pPrevBlock = &heapStart;

    //never reaches end as we are asking for next block and inequality
    while (pPrevBlock->pNextBlock < pInsertBlock)
    {
        pPrevBlock = pPrevBlock->pNextBlock;
    }

    //check if nextBlock of prevBlock is next to insertBlock, if so merge them
    if ((pPrevBlock + pPrevBlock->BlockSize) == pInsertBlock)
    {
        pPrevBlock->BlockSize += pInsertBlock->BlockSize;
        pInsertBlock = pPrevBlock;
    }
    else
    {
        //no entendi el error que habria q tirar aca 
    }

    //analogous but with next block from insertBlock, if so merge them
    //we do not care if a previous merge was done as we increase the size
    if ((pInsertBlock + pInsertBlock->BlockSize) == pPrevBlock->pNextBlock)
    {
        //if we reached the end then do not increase size
        if (pPrevBlock->pNextBlock == heapEnd){
            pInsertBlock->pNextBlock = heapEnd;
        }
        else
        {
            //set pointers skiping intermediate block
            //as said before if a previous merge was done does not change anything
            //as we are dealing with memory addresses
            pInsertBlock->BlockSize += pPrevBlock->pNextBlock->BlockSize;
            pInsertBlock->pNextBlock = pPrevBlock->pNextBlock->pNextBlock;

        }
    }
    else
    {
        //set pointer accordingly
        pInsertBlock->pNextBlock = pPrevBlock->pNextBlock;
    }

    //if they didnt merge then set nextBlock from prevBlock accordingly
    if(pInsertBlock != pPrevBlock)
    {
        pPrevBlock->pNextBlock = pInsertBlock;
    }
    else
    {
        //we did something wrong
    }
    
}