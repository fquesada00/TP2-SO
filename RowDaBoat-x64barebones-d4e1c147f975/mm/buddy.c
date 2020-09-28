#include <stdio.h>
#include <stdlib.h>

#define MINIMUM_BLOCK_SIZE_LOG2 5                                 //ALGO MENOR ME SACA EL DEALINEAMIENTO A PALABRA
#define MINIMUN_BLOCK_SIZE ((size_t)1 << MINIMUM_BLOCK_SIZE_LOG2) //8 bytes

#define MAXIMUM_BLOCK_SIZE_LOG2 9
#define MAXIMUM_BLOCK_SIZE ((size_t)1 << MAXIMUM_BLOCK_SIZE_LOG2)

#define LEVELS (MAXIMUM_BLOCK_SIZE_LOG2 - MINIMUM_BLOCK_SIZE_LOG2) + 1

#define WORD_ALIGN 8 //Buscamos en el manual del Pure y usa 8 bytes https://tracker.pureos.net/w/pureos/hardware_requirements/

#define BASE_ADDRESS 0 //Esto seria el offset
#define MAX_NODES (((size_t)1 << (LEVELS + 1)) - 1)

static char initialized = 0;
static size_t remainingBytes = 512; //es la cant de bytes que se reserven
typedef struct A_BLOCK
{
    struct A_BLOCK *nextBlock;
    struct A_BLOCK *prevBlock; //podria guardarme otra cosa como un id o algo asi
    size_t level;
} a_block;

#define HEADER (sizeof(a_block)) //NECESITO 8 BYTES PARA SABER CUANTO OCUPO; ESTO VA EN EL HEAP

static a_block *headers[LEVELS + 1];

void *pMalloc(size_t requestedSize, void *);
void *recursiveMalloc(size_t level);
void pfree(void *userStart, void *);
void recursiveFree(void *kernelStart, size_t level, void *);
size_t getLevel(void *userStart);
void *removeHeaderFromList(int level);
int findLevel(size_t requestedBytes);
void insertHeaderIntoList(void *kernelStart, size_t level);
size_t getBlockNumber(a_block *kernelStart, void *offset);
void initialize(void *);
void removeSpecificHeaderFromList(void *kernelStart, size_t level);

//f auxs
void printListLevel(size_t l){
    a_block * start = headers[l];
    printf("printing %d level list\n", l);
    printf("%p\n");
    while(start != NULL){
        printf("%p-",start);
        start = start->nextBlock;
    }
    printf("end printing\n");
}
int main()
{
    int *baseAddress = malloc(512);
    printf("%p %p %ld\n", baseAddress, &baseAddress,HEADER);
    int *v1 = pMalloc(512, baseAddress);
    printf("v:%p\n", v1);
    int *v2 = pMalloc(32, baseAddress);
    printf("%p\n", v2);
    printListLevel(findLevel(32));
    pfree(v1,baseAddress);
    pfree(v2,baseAddress);
    v1 = pMalloc(64, baseAddress); //falla con 76
    printf("%p\n", v1);
}

void *pMalloc(size_t requestedSize, void *baseAddress)
{
    if (!initialized)
    {
        initialize(baseAddress);
        initialized = 1;
    }
    size_t level = findLevel(requestedSize);
    remainingBytes -= (requestedSize + HEADER);
    if (level)
        return recursiveMalloc(level) + HEADER;
    return headers[0] != NULL ? (void *)((u_int8_t *)headers[0]->nextBlock + HEADER) : NULL;
}

void *recursiveMalloc(size_t level)
{
    void *memory;
    if (headers[level] == NULL)
    {

        memory = recursiveMalloc(level - 1);
        if (memory == NULL)
            return NULL; //No tengo memoria

        size_t blockSize = 1 << (MAXIMUM_BLOCK_SIZE_LOG2 - level); //Tamaño de este bloque

        //Tengo que partir este bloque en dos
        insertHeaderIntoList(memory, level);
        insertHeaderIntoList(memory + blockSize, level);
    }
    return removeHeaderFromList(level);
}

void pfree(void *userStart, void *offset)
{
    recursiveFree(userStart - HEADER, getLevel(userStart), offset);
}

void recursiveFree(void *kernelStart, size_t level, void *offset)
{
    void *buddy;
    size_t blockIdx = getBlockNumber(kernelStart, offset);
    size_t blockSize = 1 << (MAXIMUM_BLOCK_SIZE_LOG2 - level);

    /*
        Check if buddy is on right (blockIdx is odd), if not
        then is on left (bloclkIdx is even)
    */
    if ((blockIdx & 1) == 0)
        buddy = (u_int8_t *)kernelStart + blockSize;
    else
        buddy = (u_int8_t *)kernelStart - blockSize;

    //Tengo, mi direccion y la de mi buddy, ahora tengo que
    insertHeaderIntoList(kernelStart, level); //Meto la direccion asi es mas facil la recursion

    /*
        Lookup to obtain buddy's header from the start of the level
    */
    a_block *block = headers[level];
    while (block != NULL)
    {
        if (block == buddy)
        {
            remainingBytes += blockSize*2;
            removeSpecificHeaderFromList((void *)block, level);
            removeSpecificHeaderFromList(kernelStart, level);
            if ((blockIdx & 1) == 0)
                recursiveFree(kernelStart, level - 1, offset);
            else
                recursiveFree(buddy, level - 1, offset);
            return;
        }
        block = block->nextBlock;
    }
}

void removeSpecificHeaderFromList(void *kernelStart, size_t level)
{
    a_block *block = headers[level];

    /*
        Lookup to obtain the block to remove from the level
        Here we deal with 6 pointers
    */
    while (block != NULL)
    {
        if (block == (a_block *)kernelStart)
        {
            if (block->prevBlock == NULL)
            {
                if (block->nextBlock == NULL)
                {
                    block = NULL;
                    headers[level] = NULL;
                }
                else
                {
                    block = block->nextBlock;
                    headers[level] = block;
                    block->prevBlock->nextBlock = NULL;
                    block->prevBlock = NULL;
                }
            }
            else
            {
                if (block->nextBlock == NULL)
                {
                    block->prevBlock->nextBlock = NULL;
                }
                else
                {
                    block->prevBlock->nextBlock = block->nextBlock;
                    block->nextBlock->prevBlock = block->prevBlock;
                    block->nextBlock = NULL;
                    block->prevBlock = NULL;
                }
            }
        }
    }
}

size_t getLevel(void *userStart)
{
    userStart = (u_int8_t *)userStart - HEADER;
    return ((a_block *)userStart)->level;
}

void *removeHeaderFromList(int level)
{
    if (headers[level] == NULL)
        return NULL;
    void *returnValue = (u_int8_t *)headers[level] + HEADER;
    headers[level] = headers[level]->nextBlock;
    if (headers[level] != NULL)
        headers[level]->prevBlock = NULL;
    return returnValue;
}

int findLevel(size_t requestedBytes)
{
    int level = 0, totalBytes = 1 << MAXIMUM_BLOCK_SIZE_LOG2;
    requestedBytes += HEADER;
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
            return requestedBytes == totalBytes? level : level - 1;
    }
    return LEVELS - 1;
}

void insertHeaderIntoList(void *kernelStart, size_t level)
{
    a_block *block = headers[level];
    if (block == NULL)
    {
        block = (a_block *)kernelStart;
        block->nextBlock = NULL;
        block->prevBlock = NULL;
        block->level = level;
        headers[level] = block;
    }
    else
    {
        while (block->nextBlock != NULL)
        {
            block = block->nextBlock;
        }
        block->nextBlock = (a_block *)kernelStart;
        block->nextBlock->prevBlock = block;
        block = block->nextBlock;
        block->level = level;
        block->nextBlock = NULL;
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
