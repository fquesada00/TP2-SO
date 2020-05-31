#ifndef _STANDARD_LIB__H__
#define _STANDARD_LIB__H__
#include <stdint.h>
int strcmp(const char*, const char*);
void printf(const char*,...);
int uintToBase(uint64_t,char*,uint32_t);
void processorInfo(void);
void printMemoryFromAddress(long int);
void printRegisters(void);
void putchar(unsigned char c);
#endif