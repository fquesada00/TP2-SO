#include <stdint.h>
#ifndef _STANDARD_LIB__H__
#define _STANDARD_LIB__H__
int strcmp(const char*, const char*);
void printf(const char*,...);
int uintToBase(uint64_t,char*,uint32_t);
void processorInfo(void);
#endif