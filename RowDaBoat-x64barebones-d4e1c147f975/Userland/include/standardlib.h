#ifndef _STANDARD_LIB__H__
#define _STANDARD_LIB__H__
#include <stdint.h>
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
#endif