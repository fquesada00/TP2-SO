#ifdef MM_BUDDY
    #include <stdint.h>
    #include "buddy_memory_manager.h"

    #define MINIMUM_BLOCK_SIZE_LOG2 5                                 //ALGO MENOR ME SACA EL DEALINEAMIENTO A PALABRA
    #define MINIMUM_BLOCK_SIZE ((size_t)1 << MINIMUM_BLOCK_SIZE_LOG2) //8 bytes

    #define MAXIMUM_BLOCK_SIZE_LOG2 31
    #define MAXIMUM_BLOCK_SIZE ((size_t)1 << MAXIMUM_BLOCK_SIZE_LOG2)

    #define LEVELS (MAXIMUM_BLOCK_SIZE_LOG2 - MINIMUM_BLOCK_SIZE_LOG2) + 1

    #define WORD_ALIGN 8 //Buscamos en el manual del Pure y usa 8 bytes https://tracker.pureos.net/w/pureos/hardware_requirements/

    #define BASE_ADDRESS 0x800000 //Esto seria el offset
    #define MAX_NODES (((size_t)1 << (LEVELS + 1)) - 1)

    #define HEADER_SIZE sizeof(a_block)

    typedef struct A_BLOCK
    {
        struct A_BLOCK *nextBlock;
        size_t level;
    } a_block;

    void *recursiveMalloc(size_t level);
    void recursiveFree(void *kernelStart, size_t level);
    size_t getLevel(void *userStart);
    void *removeHeaderFromList(int level);
    int findLevel(size_t requestedBytes);
    void insertHeaderIntoList(void *kernelStart, size_t level);
    size_t getBlockNumber(a_block *kernelStart);
    void initialize();
    void removeSpecificHeaderFromList(void *kernelStart, size_t level);
    void insertSpecificHeaderIntoList(void *header, size_t level);

    static a_block *headers[LEVELS + 1];
    static char initialized = 0;
    static size_t remainingBytes = MAXIMUM_BLOCK_SIZE; 

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
            insertHeaderIntoList(returnPointer + blockSize, level);
            insertHeaderIntoList(returnPointer, level);
        }
        return removeHeaderFromList(level);
    }

    void pFree(void *pointer)
    {
        void *toFree = pointer - HEADER_SIZE;
        a_block *blockToFree = (a_block *)toFree;
        recursiveFree(toFree, blockToFree->level);
        remainingBytes += BLOCK_SIZE(blockToFree->level);
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
        insertHeaderIntoList(header, level);

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
            block = block->nextBlock;
        }
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
            headers[level] = block->nextBlock;
            return;
        }

        /*
            Lookup to obtain the block to remove from the level
        */
        while (block->nextBlock != NULL)
        {
            if (block->nextBlock == toRemove)
            {
                block->nextBlock = toRemove->nextBlock;
                return;
            }
            aux = block;
            block = block->nextBlock;
        }

        /*
            Maybe toRemove is the last block
        */
        if (block == toRemove)
        {
            aux->nextBlock = NULL;
            return;
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
        headers[level] = headers[level]->nextBlock;

        return returnValue;
    }

    /*
        Return the level of an specific amount of bytes (header size must be included)
    */
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

    /*
        Insert a header at the end of the list of the level
    */
    void insertHeaderIntoList(void *header, size_t level)
    {
        /*
            Iterate trough the list of the level from the start
        */
        a_block *block = headers[level];
        if (block == NULL)
        {
            block = (a_block *)header;
            block->nextBlock = NULL;
            block->level = level;
            headers[level] = block;
        }
        else
        {
            while (block->nextBlock != NULL)
            {
                block = block->nextBlock;
            }
            block->nextBlock = (a_block *)header;
            block = block->nextBlock;
            block->level = level;
            block->nextBlock = NULL;
        }
    }

    size_t getBlockNumber(a_block *header)
    {
        size_t relativeStart = (uint8_t *)kernelStart - (uint8_t *)BASE_ADDRESS;
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
        insertHeaderIntoList((void *)BASE_ADDRESS, 0);
    }
#endif
