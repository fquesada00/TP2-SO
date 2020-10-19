#ifndef _STANDARD_LIB_H
#define _STANDARD_LIB_H_
#include <stdint.h>
void *memset(void *destination, int32_t character, uint64_t length);
void *memcpy(void *destination, const void *source, uint64_t length);
uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);
#endif