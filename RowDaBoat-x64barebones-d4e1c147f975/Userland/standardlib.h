#ifndef _STANDARD_LIB__H__
#define _STANDARD_LIB__H__
#include <stdint.h>
int strcmp(const char*, const char*);
void printf(const char*,...);
int uintToBase(uint64_t,char*,uint32_t);
void processorInfo(void);
void printMemoryFromAddress(long int);
void printRegisters(void);
int getScreen();
int getchar();
void putchar(char c);
int scanf(const char *,...);
int doubleToString(double,char*);
int strlen(char * s);
#endif