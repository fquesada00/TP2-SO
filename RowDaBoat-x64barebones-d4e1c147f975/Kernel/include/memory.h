#ifndef _MEMORY_H_
#define _MEMORY_H_
#include <stdio.h>
#include <stdint.h>

#define BASE_ADDRESS 0x800000

/*
    Reserve requestedSize bytes in heap
*/
void *pMalloc(size_t);

/*
    Receive a pointer to be freed
*/
void pFree(void *pointer);


#endif