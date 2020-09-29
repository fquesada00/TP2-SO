#ifndef _MEMORY_H_
#define _MEMORY_H_
#include <stdio.h>
#include <stdint.h>


void *pMalloc(size_t requestedSize);
void pfree(void *userStart);


#endif