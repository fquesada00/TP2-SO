#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
2 -> 4
3 -> 8
4 -> 16
5 -> 32
6 -> 64
7 -> 128 
8 -> 256
9 -> 512
10 -> 1024
*/
#define MINIMUM_BLOCK_SIZE_LOG2 5                                 //ALGO MENOR ME SACA EL DEALINEAMIENTO A PALABRA
#define MINIMUM_BLOCK_SIZE ((size_t)1 << MINIMUM_BLOCK_SIZE_LOG2) //8 bytes

#define MAXIMUM_BLOCK_SIZE_LOG2 24
#define MAXIMUM_BLOCK_SIZE ((size_t)1 << MAXIMUM_BLOCK_SIZE_LOG2)

#define LEVELS (MAXIMUM_BLOCK_SIZE_LOG2 - MINIMUM_BLOCK_SIZE_LOG2) + 1

#define WORD_ALIGN 8 //Buscamos en el manual del Pure y usa 8 bytes https://tracker.pureos.net/w/pureos/hardware_requirements/

#define BASE_ADDRESS 0 //Esto seria el offset
#define MAX_NODES (((size_t)1 << (LEVELS + 1)) - 1)

static char initialized = 0;
static size_t remainingBytes = MAXIMUM_BLOCK_SIZE; //es la cant de bytes que se reserven

typedef struct A_BLOCK
{
    struct A_BLOCK *nextBlock;
    size_t level;
} a_block;

/*
    a_block's size if 8+8 bytes, so we don't have to check word alignment
*/
#define HEADER_SIZE sizeof(a_block)

static a_block *headers[LEVELS + 1];

void *pMalloc(size_t requestedSize, void *);
void *recursiveMalloc(size_t level, void *);
void pfree(void *userStart, void *);
void recursiveFree(void *kernelStart, size_t level, void *);
size_t getLevel(void *userStart);
void *removeHeaderFromList(int level);
int findLevel(size_t requestedBytes);
void insertHeaderIntoList(void *kernelStart, size_t level);
size_t getBlockNumber(a_block *kernelStart, void *offset);
void initialize(void *);
void removeSpecificHeaderFromList(void *kernelStart, size_t level);
void insertSpecificHeaderIntoList(void *header, size_t level, void *offset);

//f auxs
void printListLevel(size_t l)
{
    a_block *start = headers[l];
    printf("printing %ld level list at %p, continue\n", l, start);
    while (start != NULL)
    {
        printf("%p-", start);
        start = start->nextBlock;
    }
    printf("\nend printing\n");
}

void printHeader(void *pointer)
{
    a_block *p = (a_block *)(pointer - HEADER_SIZE);
    printf("\nLevel: %ld\n", p->level);
}

int main()
{
    FILE * f = fopen("resultados_b3.txt","w+");
    for (int i = 0; i < 100; i++)
    {
        clock_t begin = clock();
        int *baseAddress = malloc(1024 * 1024 * 16);
        size_t size = 1024 * 512;

        void *testers[size];
        for (int i = 0; i < size; i++)
        {
            testers[i] = pMalloc(16, baseAddress);
            //printf("\n%ld\n",getBlockNumber((a_block *)((u_int8_t *)testers[i] - HEADER_SIZE),baseAddress));
        }

        for (int i = 0; i < size; i++)
        {
            pfree(testers[i], baseAddress);
        }
        for (int i = 0; i < LEVELS; i++)
        {
            //printListLevel(i);
        }
        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        fprintf(f,"%f\n",time_spent);
    }
}

void *pMalloc(size_t requestedSize, void *baseAddress)
{
    /*
        If heap is not initialized, then do it, only one time
    */
    if (!initialized)
    {
        initialize(baseAddress);
        initialized = 1;
    }
    requestedSize += HEADER_SIZE;
    size_t level = findLevel(requestedSize);
    if (level == -1 || requestedSize > remainingBytes)
    {
        return NULL;
    } //throw exception not enough space

    /*
        Must skip header
    */
    void *returnPointer = recursiveMalloc(level, baseAddress) + HEADER_SIZE;

    remainingBytes -= requestedSize;

    return returnPointer;
}

void *recursiveMalloc(size_t level, void *offset)
{
    void *returnPointer;
    if (headers[level] == NULL)
    {
        returnPointer = recursiveMalloc(level - 1, offset);
        if (returnPointer == NULL)
        {
            return NULL;
        } //throw exception not enough space

        /*
            Obtain block's size of the level
        */
        size_t blockSize = 1 << (MAXIMUM_BLOCK_SIZE_LOG2 - level);

        /*
            Now we are in the "child", so it has to be splitted in two
        */
        //insertHeaderIntoList(returnPointer, level);
        //insertHeaderIntoList(returnPointer + blockSize, level);
        insertSpecificHeaderIntoList(returnPointer, level, offset);
        insertSpecificHeaderIntoList(returnPointer + blockSize, level, offset);
    }
    return removeHeaderFromList(level);
}

void pfree(void *pointer, void *offset)
{
    void *toFree = pointer - HEADER_SIZE;
    a_block *blockToFree = (a_block *)toFree;
    recursiveFree(toFree, blockToFree->level, offset);
    remainingBytes += (1 << (MAXIMUM_BLOCK_SIZE_LOG2 - blockToFree->level));
}

void recursiveFree(void *header, size_t level, void *offset)
{
    void *buddy;
    size_t blockIdx = getBlockNumber(header, offset);
    size_t blockSize = 1 << (MAXIMUM_BLOCK_SIZE_LOG2 - level);

    /*
        Check if buddy is on right (blockIdx is odd), if not
        then is on left (bloclkIdx is even)
    */
    if (blockIdx & 1)
        buddy = (u_int8_t *)header + blockSize;
    else
        buddy = (u_int8_t *)header - blockSize;

    /*
        Insert block in the free list of the level
    */
    //insertHeaderIntoList(header, level);
    insertSpecificHeaderIntoList(header, level, offset);

    /*
        Lookup to obtain buddy's header from the start of the level
    */
    a_block *block = headers[level];

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
                recursiveFree(header, (level - 1), offset);
            }
            else
            {
                aux = (a_block *)buddy;
                aux->level--;
                recursiveFree(buddy, (level - 1), offset);
            }
            return;
        }
        block = block->nextBlock;
    }
}

void insertSpecificHeaderIntoList(void *header, size_t level, void *offset)
{
    a_block *block = headers[level];
    a_block *toInsert = (a_block *)header;
    if (block == NULL)
    {
        headers[level] = toInsert;
        toInsert->level = level;
        toInsert->nextBlock = NULL;
        return;
    } //list not initialized

    while (block->nextBlock != NULL && block->nextBlock < toInsert)
    {
        block = block->nextBlock;
    }

    /*
        If block has to be inserted at the end, check it doesn't reach end
    */
    if (block->nextBlock == NULL)
    {
        block->nextBlock = toInsert;
        toInsert->nextBlock = NULL;
    }
    else
    {
        toInsert->nextBlock = block->nextBlock;
        block->nextBlock = toInsert;
    }
    block->nextBlock->level = level;
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
    }
}

size_t getLevel(void *userStart)
{
    userStart = (u_int8_t *)userStart - HEADER_SIZE;
    return ((a_block *)userStart)->level;
}

void *removeHeaderFromList(int level)
{
    if (headers[level] == NULL)
    {
        return NULL;
    } //not enough space (exception not here as its not a problem of this function)

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

    block = headers[level];
    while (block != NULL)
    {
        block = block->nextBlock;
    }
}

size_t getBlockNumber(a_block *kernelStart, void *offset)
{
    int level = kernelStart->level;
    int block_size = 1 << (MAXIMUM_BLOCK_SIZE_LOG2 - level);
    int relativeStart = (u_int8_t *)kernelStart - (u_int8_t *)offset;
    int relativeBlockNumber = relativeStart / block_size;
    return relativeBlockNumber + 1;
}

void initialize(void *baseAddress)
{
    insertHeaderIntoList(baseAddress, 0);
}
