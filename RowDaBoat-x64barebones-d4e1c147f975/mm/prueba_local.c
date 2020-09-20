#include <stdio.h>
#include <stdlib.h>

/*
    Store 4mb.
    MODIFICAR ESTO
*/
#define CTE (size_t)1024 * 1024 * 4  

/*
    On Pure's manual: https://tracker.pureos.net/w/pureos/hardware_requirements/
*/
#define WORD_ALIGN 8 
#define WORD_ALIGN_MASK 7

typedef struct A_BLOCK
{
    struct A_BLOCK *pNextBlock;
    size_t BlockSize;
} a_block;

/*
    Knowing the size of the header by bitwise operations 
    and word alignment to set it to a multiply of 8, we
    can move through the heap efficiently

    Alternativa ineficiente en operaciones nivel procesador:
        heapHeaderSize = sizeof(a_block) + ((WORD_ALIGN - sizeof(a_block)%WORD_ALIGN))%WORD_ALIGN
*/
static const size_t heapHeaderSize = (sizeof(a_block) + (size_t)(WORD_ALIGN - 1)) & ~(WORD_ALIGN_MASK);

static size_t remainingBytes = CTE;

/*
    With a struct in stack and a struct pointer to the end
    of the heap, we can move through it
*/
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

int main(void)
{
    /*
        Reserve 4mb.
        CAMBIAR ESTO POR CTES EN DEFINE
    */
    char *baseAddress = malloc(1024 * 1024 * 4);

    pInitHeap(baseAddress, baseAddress + (1024 * 1024 * 4 - 1));

    char *temp = pMalloc(sizeof(char) * 4);
    pFree(temp);

    temp = pMalloc(sizeof(char) * 2);
    char *p = pMalloc(sizeof(char) * 4);
    char *k = pMalloc(sizeof(char) * 2);
    pFree(p);
    p = pMalloc(sizeof(char) * 9);
    k = pMalloc(9);

    p[0] = 'a';
    p[1] = 'b';
    p[2] = 'c';
    p[3] = 'd';
    k[0] = 'e';
    k[1] = 'f';
    for (int i = 0; i < 4; i++)
    {
        printf("%c", p[i]);
    }
    for (int i = 0; i < 2; i++)
    {
        printf("%c", k[i]);
    }
}

/*
    Reserve requestedSize bytes in heap
*/
void *pMalloc(size_t requestedSize)
{
    void *pReturnBlock = NULL;

    /*
        If heap is not initialized, then do it
    */
    if (heapEnd == NULL)
    {
        //pInitHeap(0x800000, 0xC000000);
    }
    //MODIFICAR SIN EL ELSE (chequear por las dudas)
    else
    {
        if (remainingBytes < requestedSize)
        {

        } //throw exception not enough space
        if (requestedSize < 0)
        {

        } //throw exception negative sizes not accepted

        /*
            Header struct is part of the size
        */
        requestedSize += heapHeaderSize;

        /*
            To align, check if requestedSize is not multiple of WORD_ALIGN_MASK.
            If yes, then its done.
            If not, then add WORD_ALIGN and bitwise "and" operator with
            "not" WORD_ALIGN_MASK to set last bits multiple of WORD_ALIGN

            Alternativa ineficiente en operaciones nivel procesador:
                requestedSize += (WORD_ALIGN - requestedSize%WORD_ALIGN)%WORD_ALIGN
        */
        if ((requestedSize & WORD_ALIGN_MASK) != 0)
        {
            requestedSize += WORD_ALIGN;
            requestedSize &= ~WORD_ALIGN_MASK;
        }

        /*
            Initialize search blocks
        */
        a_block *pPrevBlock, *pActualBlock;
        pPrevBlock = &heapStart;
        pActualBlock = pPrevBlock->pNextBlock;

        /*
            Lookup doing first fit algorithm, tambien me fijo si esta libre
        */
        while (pActualBlock->BlockSize < requestedSize && pActualBlock->pNextBlock != NULL)
        {
            pPrevBlock = pActualBlock;
            pActualBlock = pActualBlock->pNextBlock;
        }

        if (pActualBlock->pNextBlock == NULL)
        {
            return NULL;
        } //throw exception not enough space (reached end)

        /*
            Set block to be returned without header as well
        */
        pReturnBlock = (void *)((u_int8_t *)pActualBlock + heapHeaderSize);

        /*
            Skip block pointer as its going to be occupied 
        */
        pPrevBlock->pNextBlock = pActualBlock->pNextBlock;

        /*
            Now memory must be partitioned as its free space available
            to storage at least one byte more (not counting a_block struct
            as its mandatory to allocate memory).
            If not, then do not link, oppositly, link it
        */
        if ((pActualBlock->BlockSize - requestedSize) <= (remainingBytes))
        {
            a_block *pLinkBlock = (a_block *)((u_int8_t *)pActualBlock + requestedSize);
            pLinkBlock->BlockSize = pActualBlock->BlockSize - requestedSize;

            /*
                Update new size as its less than before
            */
            pActualBlock->BlockSize = requestedSize;
            pInsertBlockIntoList(pLinkBlock);
        }

        remainingBytes -= pActualBlock->BlockSize;

        /*
            Take out the block from the list
        */
        pActualBlock->pNextBlock = NULL;
    }

    return pReturnBlock;
}

/*
    Here baseAddress & endAddress must be word aligned
*/
void pInitHeap(void *baseAddress, void *endAddress)
{
    /*
        Set start struct allocated in stack
    */
    heapStart.pNextBlock = (a_block *)baseAddress;
    heapStart.BlockSize = (size_t)0;

    heapEnd = endAddress - heapHeaderSize;
    heapEnd->pNextBlock = NULL;
    heapEnd->BlockSize = (size_t)0;

    a_block *pFirstBlock = (a_block *)baseAddress;
    pFirstBlock->pNextBlock = heapEnd;
    pFirstBlock->BlockSize = (size_t)heapEnd - (size_t)pFirstBlock;

    remainingBytes = pFirstBlock->BlockSize;
}

/*
    Receive a pointer to be freed
*/
void pFree(void *pointer)
{
    /*
        Cast pointer as we are going to move 1 byte per address
    */
    u_int8_t *pNextABlock = (u_int8_t *)pointer;
    a_block *pLinkBlock;

    if (pNextABlock == NULL)
    {
        return;
    } //nothing to do here

    /*
        Before the pointer there is a_block struct
    */
    pNextABlock -= heapHeaderSize;
    pLinkBlock = (a_block *)pNextABlock;

    if (pLinkBlock->pNextBlock != NULL)
    {
        return;
    } //we did something wrong so

    remainingBytes += pLinkBlock->BlockSize;

    pInsertBlockIntoList(pLinkBlock);
}

void pInsertBlockIntoList(a_block *pInsertBlock)
{
    a_block *pPrevBlock = &heapStart;

    /*
        Starting from first block, we iterate along the list
        until we find a block with a higher address than pInsertBlock
    */
    while (pPrevBlock->pNextBlock < pInsertBlock)
    {
        pPrevBlock = pPrevBlock->pNextBlock;
    }

    if (pPrevBlock->pNextBlock == NULL)
    {
        return;
    } //we did something wrong

    /*
        Check if the previous block to the one being inserted make
        a big block that is next to each other, if yes, then merge
    */
    if ((a_block *)((u_int8_t *)pPrevBlock + pPrevBlock->BlockSize) == pInsertBlock)
    {
        pPrevBlock->BlockSize += pInsertBlock->BlockSize;
        pInsertBlock = pPrevBlock;
    }

    /*
        Same method as above but with next block.
        If a previous merge was done, it doesnt modify
        anything as we are dealing with pointers
    */
    if ((a_block *)((u_int8_t *)pInsertBlock + pInsertBlock->BlockSize) == pPrevBlock->pNextBlock)
    {
        /*
            If end was reached, then dont increase size.
            Else, skip intermediate block
        */
        if (pPrevBlock->pNextBlock == heapEnd)
        {
            pInsertBlock->pNextBlock = heapEnd;
        }
        else
        {
            pInsertBlock->BlockSize += pPrevBlock->pNextBlock->BlockSize;
            pInsertBlock->pNextBlock = pPrevBlock->pNextBlock->pNextBlock;
        }
    }
    else
    {
        pInsertBlock->pNextBlock = pPrevBlock->pNextBlock;
    }

    /*
        If first merge was not done, set next block pointer
        of previous block to the one being inserted
    */
    if (pInsertBlock != pPrevBlock)
    {
        pPrevBlock->pNextBlock = pInsertBlock;
    }
}