#ifndef _STANDARD_LIB__H__
#define _STANDARD_LIB__H__
#include <stdint.h>
#include <stddef.h>
void *memcpy(void *destination, const void *source, uint64_t length);
void * memset(void * destination, int32_t character, uint64_t length);
#endif