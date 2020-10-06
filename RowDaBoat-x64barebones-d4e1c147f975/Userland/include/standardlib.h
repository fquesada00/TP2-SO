#ifndef _STANDARD_LIB__H__
#define _STANDARD_LIB__H__
#include <stdint.h>
#include <stddef.h>
void vaArg(int n, ...);
int strcmp(const char*, const char*);
int printf(const char*,...);
int uintToBase(uint64_t,char*,uint32_t);
void processorInfo(void);
void printMemoryFromAddress(uint64_t);
int getScreen();
int getchar();
void putchar(char c);
int scanf(const char *,...);
int doubleToString(double,char*);
int strlen(char * s);
void printReg();
void DivZero();
int programLoader(void(*program)(void));
void printRtc();
void invOpCode();
void printCoreTemp();
void manShell();
int execv(void *entry_point, int argc, char *argv[]);
void * pMalloc(size_t size);
void pFree(void * block);
void * memcpy(void * destination, const void * source, uint64_t length);
#endif