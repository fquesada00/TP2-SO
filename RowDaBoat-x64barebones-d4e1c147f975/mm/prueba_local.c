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

int main(void)
{
    //reserve 10mb
    char *baseAddress = malloc(1024 * 1024 * 10);

    pInitHeap(baseAddress, baseAddress + (1024 * 1024 * 10 - 1));
}

void *pMalloc(size_t requestedSize)
{
    void *pReturnBlock = NULL;

    /*
        If heap is not initialized, then do it
    */
    if (heapEnd == NULL)
    {
        //MODIFICAR ESTO CON CTES ESTARIA COPADO
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
            WORD_ALIGN_MASK to set last bits multiple of WORD_ALIGN
            Alternativa ineficiente en operaciones nivel procesador:
                requestedSize += (WORD_ALIGN - requestedSize%WORD_ALIGN)%WORD_ALIGN
            */
        if ((requestedSize & WORD_ALIGN_MASK) != 0)
        {
            requestedSize += WORD_ALIGN;
            requestedSize &= WORD_ALIGN_MASK; //NO SERIA NOT WORD_ALIGN_MASK
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
            a_block * pLinkBlock = pActualBlock + requestedSize;
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
void pFree(void * pointer)
{

    size_t * pNextABlock = (size_t *) pointer;
    a_block * pLinkBlock;

    if (pNextABlock == NULL)
    {

    } //nothing to do here

    /*
        Before the pointer there is a_block struct
    */
    pNextABlock -= heapHeaderSize;
    pLinkBlock = (a_block *) pNextABlock;

    if (pLinkBlock->free == 0 || pLinkBlock->pNextBlock != NULL)
    {

    } //we did something wrong so

    remainingBytes += pLinkBlock->BlockSize;

    pInsertBlockIntoList(pLinkBlock);
}

void pInsertBlockIntoList(a_block *pInsertBlock)
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
        if (pPrevBlock->pNextBlock == heapEnd)
        {
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
    if (pInsertBlock != pPrevBlock)
    {
        pPrevBlock->pNextBlock = pInsertBlock;
    }
    else
    {
        //we did something wrong
    }
}