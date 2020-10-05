#if !defined MM_BUDDY && !defined MM_BUDDY2
    #include "memory_manager.h"

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

    void *pMalloc(size_t requestedSize)
    {
        void *pReturnBlock = NULL;

        /*
            If heap is not initialized, then do it
        */
        if (heapEnd == NULL)
        {
            pInitHeap(BASE_ADDRESS, END_ADDRESS);
        }

        /*
            Warning (not an error or exception)
        */
        if (requestedSize < 0)
        {

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
        pActualBlock = pPrevBlock->pNextBlock;

        /*
            Lookup doing first fit algorithm
        */
        while (pActualBlock->blockSize < requestedSize && pActualBlock->pNextBlock != NULL)
        {
            pPrevBlock = pActualBlock;
            pActualBlock = pActualBlock->pNextBlock;
        }

        /*
            Throw error not enough space
        */
        if (pActualBlock->pNextBlock == NULL)
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
        pPrevBlock->pNextBlock = pActualBlock->pNextBlock;

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

        /*
            Take out the block from the list
        */
        pActualBlock->pNextBlock = NULL;

        return pReturnBlock;
    }

    void pInitHeap(void *baseAddress, void *endAddress)
    {
        /*
            Set start struct allocated in stack
        */
        heapStart.pNextBlock = (a_block *)baseAddress;
        heapStart.blockSize = (size_t)0;

        heapEnd = endAddress - heapHeaderSize;
        heapEnd->pNextBlock = NULL;
        heapEnd->blockSize = (size_t)0;

        a_block *pFirstBlock = (a_block *)baseAddress;
        pFirstBlock->pNextBlock = heapEnd;
        pFirstBlock->blockSize = (size_t)heapEnd - (size_t)pFirstBlock;

        remainingBytes = pFirstBlock->blockSize;
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

        /*
            Its an error of the memory manager
        */
        if (pLinkBlock->pNextBlock != NULL)
        {
            return;
        }

        remainingBytes += pLinkBlock->blockSize;

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

        /*
            Its an error of the memory manager
        */
        if (pPrevBlock->pNextBlock == NULL)
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
        if ((a_block *)((uint8_t *)pInsertBlock + pInsertBlock->blockSize) == pPrevBlock->pNextBlock)
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
                pInsertBlock->blockSize += pPrevBlock->pNextBlock->blockSize;
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
#endif