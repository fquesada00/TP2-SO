#include <stdio.h>
#include <stdlib.h>


#define MINIMUM_BLOCK_SIZE_LOG2 8                                 //ALGO MENOR ME SACA EL DEALINEAMIENTO A PALABRA
#define MINIMUN_BLOCK_SIZE ((size_t)1 << MINIMUM_BLOCK_SIZE_LOG2) //8 bytes

#define MAXIMUM_BLOCK_SIZE_LOG2 21 // MAXIMO 2MB
#define MAXIMUM_BLOCK_SIZE ((size_t)1 << MAXIMUM_BLOCK_SIZE_LOG2)

#define LEVELS (MAXIMUM_BLOCK_SIZE_LOG2 - MINIMUM_BLOCK_SIZE_LOG2)

#define WORD_ALIGN 8 //Buscamos en el manual del Pure y usa 8 bytes https://tracker.pureos.net/w/pureos/hardware_requirements/

#define BASE_ADDRESS 0 //Esto seria el offset
#define MAX_NODES (((size_t)1 << (LEVELS + 1)) - 1)

static char initialized = 0;
static size_t remainingBytes = 2; //es la cant de mb que se reserven
typedef struct A_BLOCK
{
    struct A_BLOCK *nextBlock;
    struct A_BLOCK *prevBlock; //podria guardarme otra cosa como un id o algo asi
    size_t level;
} a_block;

#define HEADER (sizeof(a_block)) //NECESITO 8 BYTES PARA SABER CUANTO OCUPO; ESTO VA EN EL HEAP

static a_block *headers[LEVELS + 1];

void *pMalloc(size_t requestedSize);
void *recursiveMalloc(size_t level);
void free(void *userStart);
void recursiveFree(void *kernelStart, size_t level);
size_t getLevel(void *userStart);
void *removeHeaderFromList(level);
int findLevel(size_t requestedBytes);
void insertHeaderIntoList(void *kernelStart, size_t level);
size_t getBlockNumber(a_block *kernelStart);
void initialize();

void *pMalloc(size_t requestedSize)
{
    if (!initialized)
    {
        initialize();
        initialized = 1;
    }
    size_t level = findLevel(requestedSize);
    if (level)
        return recursiveMalloc(level);
    return headers[0] != NULL? (void*)((u_int8_t *)headers[0]->nextBlock + HEADER) : NULL;
}

void *recursiveMalloc(size_t level)
{
    void *memory;
    if (headers[level] == NULL)
    {
        
        memory = recursiveMalloc(level - 1);
        if (!memory)
            return NULL; //No tengo memoria

        size_t size = 1 << (MAXIMUM_BLOCK_SIZE_LOG2 -  level); //Tamaño de este bloque

        //Tengo que partir este bloque en dos
        insertHeaderIntoList(memory, level);
        insertHeaderIntoList(memory + size, level);
    }
    return removeHeaderFromList(level);
}

void free(void *userStart)
{
    recursiveFree(userStart - HEADER,getLevel(userStart))
}

void recursiveFree(void *kernelStart, size_t level)
{
    void *buddy;
    size_t linearTreeNumber = getBlockNumber(kernelStart);
    size_t sizeLevel = 1 << (MAXIMUM_BLOCK_SIZE_LOG2 - level);

    if (index & 1) == 0)
        buddy = (u_int8_t)kernelStart + size;
    else
        buddy = (u_int8_t)kernelStart – size;

    //Tengo, mi direccion y la de mi buddy, ahora tengo que
    insertHeaderIntoList(kernelStart, level); //Meto la direccion asi es mas facil la recursion

    block = headers[level];
    while (block != NULL)
    {
        if (block == buddy)
        {
            removeSpecificHeaderFromList(block, level);
            removeSpecificHeaderFromList(kernelStart, level);
            if (index & 1) == 0)
                recursiveFree(kernelStart, level - 1);
            else
                recursiveFree(buddy, level - 1);
            return;
        }
        block = block->nextBlock;
    }
}

void removeSpecificHeaderFromList(void * kernelStart,size_t level){
    a_block *block = headers[level];
    
    while(block != NULL) {
        if(block == kernelStart){
            block = block->prevBlock
            block->next = block->next->next;
            return;
        }
    }
    return;
}

size_t getLevel(void *userStart)
{
    userStart = (u_int8_t *)userStart - HEADER;
    return ((a_block *)userStart)->level;
}

void *removeHeaderFromList(level)
{
    if (headers[level] == NULL)
        return NULL;
    void *returnValue = headers[level];
    headers[level] = headers[level]->next;
    if (headers[level] != NULL)
        headers[level]->prevBlock = NULL;
    return returnValue;
}


int findLevel(size_t requestedBytes)
{
    int level = 0, totalBytes = 2; //completar con una cte
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
        else return level;
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

size_t getBlockNumber(a_block *kernelStart)
{
    int level = kernelStart->level;
    int block_size = 1 << (MAXIMUM_BLOCK_SIZE_LOG2 - level);
    int relativeStart = kernelStart - OFFSET;
    int relativeBlockNumber = relativeStart / block_size;
    return relativeBlockNumber + 1; //Hay que verificarlo en papel esto
}    


void initialize()
{
    insertHeaderIntoList(BASE_ADDRESS, 0);
}

