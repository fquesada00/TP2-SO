// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#if !defined MM_BUDDY
#include "memory_manager.h"
#include "memory_manager_lib.h"
#include "standardLib.h"
#include "video_driver.h"

/*    
        This memory manager works with a list of headers with a pointer to the next free header.
        When a block is reserved, it starts looking from the first free header, this algorithm is called
        first fit. If it cannot fit, then there is not enough space to storage the requestedSize (plus the header).
        Here we can have external fragmentation, meaning the space can be enough but its the sum of 2 or more
        free headers, and it cannot fit in one single header, so pMalloc returns NULL.
        If the space is available, then it returns the header pointer plus the header size.

        To free a block, it substracts the size of the header, and looks if the left and right header are free,
        so they can merge a make a big free block.
    */
/*
        LINKS DE INTERES
        https://courses.cs.washington.edu/courses/cse351/10sp/lectures/15-memallocation.pdf
        https://www.youtube.com/watch?v=74s0m4YoHgM
        https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/master/portable/MemMang/heap_4.c
    */

/*
        Knowing the size of the header by bitwise operations 
        and word alignment to set it to a multiply of 8, we
        can move through the heap efficiently

        Alternativa ineficiente en operaciones nivel procesador:
            heapHeaderSize = sizeof(a_block) + ((WORD_ALIGN - sizeof(a_block)%WORD_ALIGN))%WORD_ALIGN
    */
static const size_t heapHeaderSize = (sizeof(a_block) + (size_t)(WORD_ALIGN - 1)) & ~(WORD_ALIGN_MASK);

static size_t remainingBytes = HEAP_SIZE;

/*
        With a struct in stack and a struct pointer to the end
        of the heap, we can move through it
    */
static a_block heapStart, *heapEnd = NULL;
size_t heap_size = HEAP_SIZE;
size_t free_size = HEAP_SIZE;

void *pMalloc(size_t requestedSize)
{
    void *pReturnBlock = NULL;

    /*
            If heap is not initialized, then do it
        */
    if (heapEnd == NULL)
    {
        pInitHeap((void *)BASE_ADDRESS, (void *)END_ADDRESS);
    }

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
            Throw error not enough space
        */
    if (remainingBytes < requestedSize)
    {
        return NULL;
    }

    /*
            Initialize search blocks
        */
    a_block *pPrevBlock, *pActualBlock;
    pPrevBlock = &heapStart;
    pActualBlock = pPrevBlock->pNextFreeBlock;

    /*
            Lookup doing first fit algorithm
        */
    while (pActualBlock->blockSize < requestedSize && pActualBlock->pNextFreeBlock != NULL)
    {
        pPrevBlock = pActualBlock;
        pActualBlock = pActualBlock->pNextFreeBlock;
    }

    /*
            Throw error not enough space
        */
    if (pActualBlock->pNextFreeBlock == NULL)
    {
        return NULL;
    }

    /*
            Set block to be returned without header as well
        */
    pReturnBlock = (void *)((uint8_t *)pActualBlock + heapHeaderSize);

    /*
            Skip block pointer as its going to be occupied 
        */
    pPrevBlock->pNextFreeBlock = pActualBlock->pNextFreeBlock;

    /*
            Now memory must be partitioned as its free space available
            to storage at least one byte more (not counting a_block struct
            as its mandatory to allocate memory).
            If not, then do not link, oppositly, link it
        */
    if ((pActualBlock->blockSize - requestedSize) <= (remainingBytes))
    {
        a_block *pLinkBlock = (a_block *)((uint8_t *)pActualBlock + requestedSize);
        pLinkBlock->blockSize = pActualBlock->blockSize - requestedSize;

        /*
                Update new size as its less than before
            */
        pActualBlock->blockSize = requestedSize;
        pInsertBlockIntoList(pLinkBlock);
    }

    remainingBytes -= pActualBlock->blockSize;
    free_size = remainingBytes;
    return pReturnBlock;
}

void pInitHeap(void *baseAddress, void *endAddress)
{
    /*
            Set start struct allocated in stack
        */
    heapStart.pNextFreeBlock = (a_block *)baseAddress;
    heapStart.blockSize = (size_t)0;

    uint64_t aux = (uint64_t)endAddress;

    aux -= heapHeaderSize;

    heapEnd = (void *)aux;
    heapEnd->pNextFreeBlock = NULL;
    heapEnd->blockSize = (size_t)0;

    a_block *pFirstBlock = (a_block *)baseAddress;
    pFirstBlock->pNextFreeBlock = heapEnd;
    pFirstBlock->blockSize = (size_t)heapEnd - (size_t)pFirstBlock;

    remainingBytes = pFirstBlock->blockSize;

    free_size = remainingBytes;
}

void pFree(void *pointer)
{
    /*
            Cast pointer as we are going to move 1 byte per address
        */
    uint8_t *pNextABlock = (uint8_t *)pointer;
    a_block *pLinkBlock;

    /*
            Nothing to do here
        */
    if (pNextABlock == NULL)
    {
        return;
    }

    /*
            Before the pointer there is a_block struct
        */
    pNextABlock -= heapHeaderSize;
    pLinkBlock = (a_block *)pNextABlock;

    remainingBytes += pLinkBlock->blockSize;
    free_size = remainingBytes;
    pInsertBlockIntoList(pLinkBlock);
}

void pInsertBlockIntoList(a_block *pInsertBlock)
{
    if (pInsertBlock == heapEnd)
    {
        return;
    }

    a_block *pPrevBlock = &heapStart;

    /*
            Starting from first block, we iterate along the list
            until we find a block with a higher address than pInsertBlock
        */
    while (pPrevBlock->pNextFreeBlock < pInsertBlock)
    {
        pPrevBlock = pPrevBlock->pNextFreeBlock;
    }

    /*
            Its an error of the memory manager, not gonna happen actually
        */
    if (pPrevBlock->pNextFreeBlock == NULL)
    {
        return;
    }

    /*
            Check if the previous block to the one being inserted make
            a big block that is next to each other, if yes, then merge
        */
    if ((a_block *)((uint8_t *)pPrevBlock + pPrevBlock->blockSize) == pInsertBlock)
    {
        pPrevBlock->blockSize += pInsertBlock->blockSize;
        pInsertBlock = pPrevBlock;
    }

    /*
            Same method as above but with next block.
            If a previous merge was done, it doesnt modify
            anything as we are dealing with pointers
        */
    if ((a_block *)((uint8_t *)pInsertBlock + pInsertBlock->blockSize) == pPrevBlock->pNextFreeBlock)
    {
        /*
                If end was reached, then dont increase size.
                Else, skip intermediate block
            */
        if (pPrevBlock->pNextFreeBlock == heapEnd)
        {
            pInsertBlock->pNextFreeBlock = heapEnd;
        }
        else
        {
            pInsertBlock->blockSize += pPrevBlock->pNextFreeBlock->blockSize;
            pInsertBlock->pNextFreeBlock = pPrevBlock->pNextFreeBlock->pNextFreeBlock;
        }
    }
    else
    {
        pInsertBlock->pNextFreeBlock = pPrevBlock->pNextFreeBlock;
    }

    /*
            If first merge was not done, set next block pointer
            of previous block to the one being inserted
        */
    if (pInsertBlock != pPrevBlock)
    {
        pPrevBlock->pNextFreeBlock = pInsertBlock;
    }
}
#endif