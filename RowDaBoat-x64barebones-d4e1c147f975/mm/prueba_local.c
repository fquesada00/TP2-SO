#include <stdio.h>
#include <stdlib.h>

#define CTE (size_t)1024 * 1024 * 4  //store 4mb MODIFICAR ESTO
#define MINIMUN_BLOCK_SIZE (size_t)8 //8 bytes

#define WORD_ALIGN 8 //Buscamos en el manual del Pure y usa 8 bytes https://tracker.pureos.net/w/pureos/hardware_requirements/
#define WORD_ALIGN_MASK 7
typedef struct A_BLOCK
{
    struct A_BLOCK *pNextBlock;
    size_t BlockSize;
    char free;
} a_block;

/*
    Usando el tamaño del "header" usamos operaciones nivel bit
    reemplazando el operador "mod" y alineamos para que el tamaño sea
    multiplo de 8, que es lo que lee el tamaño de una palabra del Pure

    Alternativa ineficiente en operaciones nivel procesador:
        heapHeaderSize = sizeof(a_block) + ((WORD_ALIGN - sizeof(a_block)%WORD_ALIGN))%WORD_ALIGN
*/
static const size_t heapHeaderSize = (sizeof(a_block) + (size_t)(WORD_ALIGN - 1)) & ~(WORD_ALIGN_MASK);

static size_t remainingBytes = CTE;

/*
    Con una estructura en el stack que apunte al principio del heap
    y un puntero al final, podemos movernos por todo el heap
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

void h(void)
{
    printf("llegue\n");
}

void printDirections(char *p, int dim)
{
    for (int i = 0; i < dim; i++)
    {
        printf(" - Pos: %d at dir: %p - ", i, p + i);
    }
    printf("\n");
}
int main(void)
{
    //reserve 4mb
    char *baseAddress = malloc(1024 * 1024 * 4); //(void*)0x7efec7045010

    pInitHeap(baseAddress, baseAddress + (1024 * 1024 * 4 - 1));

    char *temp = pMalloc(sizeof(char) * 4);
    pFree(temp);

    temp = pMalloc(sizeof(char) * 2);
    printDirections(temp, 2);
    char *p = pMalloc(sizeof(char) * 4);
    printDirections(p, 4);
    char *k = pMalloc(sizeof(char) * 2);
    printDirections(k, 2);
    pFree(p);
    p = pMalloc(sizeof(char) * 9);
    k = pMalloc(9);

    p[0]='a';
    p[1]='b';
    p[2]='c';
    p[3]='d';
    k[0]='e';
    k[1]='f';
    for(int i = 0 ; i < 4 ; i++){
        printf("%c",p[i]);
    }
    for(int i = 0 ; i < 2 ; i++){
        printf("%c",k[i]);
    }
}

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
    //creo q esto no va a aca xq sino siempre devuelve null en la primer iter, chequear
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
        printf("Actual: %p, requested: %ld\n", pActualBlock, requestedSize);
        /*
            Lookup doing first fit algorithm, tambien me fijo si esta libre
        */
        while (pActualBlock->BlockSize < requestedSize && pActualBlock->pNextBlock != NULL)
        {
            pPrevBlock = pActualBlock;
            pActualBlock = pActualBlock->pNextBlock;
        }
        printf("Actual post while: %p\n", pActualBlock);

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
        printf("%p y %ld\n", pActualBlock, pActualBlock->BlockSize);
        printf("%ld <= %ld\n", pActualBlock->BlockSize - requestedSize, remainingBytes);
        if ((pActualBlock->BlockSize - requestedSize) <= (remainingBytes))
        {
            a_block *pLinkBlock = (a_block *)((u_int8_t *)pActualBlock + requestedSize);
            pLinkBlock->BlockSize = pActualBlock->BlockSize - requestedSize;
            pLinkBlock->free = 1; //Esto no es necesario porque este seria el libre, va, si 1 es ocupado, ademas como esta implementado no es necesario
            //pPrevBlock->pNextBlock = pLinkBlock;
            /*
                Update new size as its less than before
            */
            pActualBlock->BlockSize = requestedSize;
            printf("link: %p\n", pLinkBlock);
            pInsertBlockIntoList(pLinkBlock);
        }

        remainingBytes -= pActualBlock->BlockSize; //aca lo mismo no falta sumar el heap size?

        /*
            Take out the block from the list
        */
        pActualBlock->pNextBlock = NULL;
        pActualBlock->free = 0;
    }
    printf("return %p\n", pReturnBlock);
    printf("------\n\n");
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
    printf("Base: %p & End: %p\n", baseAddress, endAddress);
    heapStart.pNextBlock = (a_block *)baseAddress;
    heapStart.BlockSize = (size_t)0;
    heapStart.free = 0;

    heapEnd = endAddress - heapHeaderSize;
    heapEnd->pNextBlock = NULL;
    heapEnd->BlockSize = (size_t)0;
    heapEnd->free = 0;

    a_block *pFirstBlock = (a_block *)baseAddress;
    pFirstBlock->pNextBlock = heapEnd;
    pFirstBlock->BlockSize = (size_t)heapEnd - (size_t)pFirstBlock;
    pFirstBlock->free = 1;

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
    printf("%p\n", pointer);
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
    
        printf("FREE\n");
    printf("FREE\n");
    pInsertBlockIntoList(pLinkBlock);
}

void pInsertBlockIntoList(a_block *pInsertBlock)
{
    printf("insert: %p\n", pInsertBlock);
    a_block *pPrevBlock = &heapStart;

    /*
        Starting from first block, we iterate along the list
        until we find a block with a higher address than pInsertBlock
    */
    while (pPrevBlock->pNextBlock < pInsertBlock)
    {
        printf("1");
        pPrevBlock = pPrevBlock->pNextBlock;
    }
    printf("%p & %p\n", pPrevBlock->pNextBlock, pPrevBlock);

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
        printf("1");
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
        printf("2");
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

    printf("Prev: %p, Insert: %p, Next: %p\n", pPrevBlock->pNextBlock, pInsertBlock, pInsertBlock->pNextBlock);
}