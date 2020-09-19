#include <stdio.h>
#include <stdlib.h>

#define CTE (size_t)1000000 * 4      //store 4mb MODIFICAR ESTO
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
        printf("Dir at pos %d: %p - ", i,p + i);
    }
    printf("\n");
}
int main(void)
{
    //reserve 10mb
    a_block a,b;
    a.free=0;
    b.free=1;
    //printf("%d\n",heapHeaderSize);
    //printf("%d %d\n",(sizeof(a) + (size_t)(WORD_ALIGN - 1)) & ~(WORD_ALIGN_MASK),(sizeof(b) + (size_t)(WORD_ALIGN - 1)) & ~(WORD_ALIGN_MASK));


    char *baseAddress = malloc(1024 * 1024 * 10); //(void*)0x7efec7045010

    pInitHeap(baseAddress, baseAddress + (1024 * 1024 * 10 - 1));

    char *temp = pMalloc(sizeof(int) * 4);
    printDirections(temp, 4);
    printf("temp-header: %p & %p\n",(temp-heapHeaderSize), temp);
    pFree(temp);
    
    temp = pMalloc(sizeof(int) * 4);
    printDirections(temp, 5);
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
        printf("requested %d\n",requestedSize);
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

        } //throw exception not enough space (reached end)

        /*
            Set block to be returned without header as well
        */
        pReturnBlock = (void *)pActualBlock + heapHeaderSize;

        /*
            Skip block pointer as its going to be occupied 
        */
        //Bueno, aca tenemos una diferencia de implementacion, aca lo que hace es ignora los bloques que no estan libres,
        //osea tiene una lista de libres y los ocupados los deja por ahi, cuando se liberan los vuelve a incorporar
        //Como es mas eficiente la que implementan ellos, la dejo, sino tendriamos que checkear el free en elwhile de arriba y cambiarlo
        pPrevBlock->pNextBlock = pActualBlock->pNextBlock;

        /*
            Now memory must be partitioned as its free space available
            to storage at least one byte more (not counting a_block struct
            as its mandatory to allocate memory).
            If not, then do not link, oppositly, link it
        */
        if ((pActualBlock->BlockSize - requestedSize) > (remainingBytes - heapHeaderSize))
        {
            a_block *pLinkBlock = pActualBlock + requestedSize;
            pLinkBlock->BlockSize = pActualBlock->BlockSize - requestedSize;
            pLinkBlock->free = 1; //Esto no es necesario porque este seria el libre, va, si 1 es ocupado, ademas como esta implementado no es necesario

            /*
                Update new size as its less than before
            */
            pActualBlock->BlockSize = requestedSize; //No nos faltaria sumar el heap size, porque los bloques tienen el tamaño del heap
            //cuando tratemos de liberar vamos a tener tamaño equivocado
            pInsertBlockIntoList(pLinkBlock);
        }

        remainingBytes -= pActualBlock->BlockSize; //aca lo mismo no falta sumar el heap size?

        /*
            Take out the block from the list
        */
        pActualBlock->pNextBlock = NULL;
        pActualBlock->free = 0;
    }
    printf("return %p\n",pReturnBlock);
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
    //printf("first %p, %d\n",pFirstBlock,(sizeof(*pFirstBlock) + (size_t)(WORD_ALIGN - 1)) & ~(WORD_ALIGN_MASK));

    remainingBytes = pFirstBlock->BlockSize;
}

/*
    Receive a pointer to be freed
*/
void pFree(void *pointer)
{

    size_t *pNextABlock = (size_t *)pointer;
    a_block *pLinkBlock;
    printf("%p\n",pNextABlock);

    if (pNextABlock == NULL)
    {

    } //nothing to do here
    
    /*
        Before the pointer there is a_block struct
    */
    pNextABlock -= heapHeaderSize;
    printf("%d\n",heapHeaderSize);
    printf("nextBlock: %p\n",pNextABlock);
    pLinkBlock = (a_block *)pNextABlock;
    printf("%p\n", pLinkBlock);
    //ACA ESTA FALLANDO
    if (pLinkBlock->free == 0 || pLinkBlock->pNextBlock == NULL)
    {

    } //we did something wrong so
    h();
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

    if (pPrevBlock->free == 0 || pPrevBlock->pNextBlock == NULL)
    {

    } //we did something wrong

    /*
        Check if the previous block to the one being inserted make
        a big block that is next to each other, if yes, then merge
    */
    if ((pPrevBlock + pPrevBlock->BlockSize) == pInsertBlock)
    {
        pPrevBlock->BlockSize += pInsertBlock->BlockSize;
        pInsertBlock = pPrevBlock;
    }

    /*
        Same method as above but with next block.
        If a previous merge was done, it doesnt modify
        anything as we are dealing with pointers
    */
    if ((pInsertBlock + pInsertBlock->BlockSize) == pPrevBlock->pNextBlock)
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