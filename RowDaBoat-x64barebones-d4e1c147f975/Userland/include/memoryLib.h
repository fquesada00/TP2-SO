#ifndef _MEMORY_LIB_H_
#define _MEMORY_LIB_H_
#include <stddef.h>
extern void * pMalloc(size_t size);
extern void pFree(void *block);
#endif