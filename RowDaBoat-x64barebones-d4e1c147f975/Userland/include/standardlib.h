#ifndef _STANDARD_LIB__H__
#define _STANDARD_LIB__H__
#include <stdint.h>
#include <stddef.h>
void vaArg(int n, ...);
int printf(const char *, ...);
int uintToBase(uint64_t, char *, uint32_t);
void processorInfo(void);
void printMemoryFromAddress(uint64_t);
int getScreen();
int getchar();
void putchar(char c);
int scanf(const char *, ...);
int doubleToString(double, char *);
void printReg();
void DivZero();
int programLoader(void (*program)(void));
void printRtc();
void invOpCode();
void printCoreTemp();
void manShell();
int execv(void *entry_point, int argc, char *argv[]);
void *pMalloc(size_t size);
void pFree(void *block);
void *memcpy(void *destination, const void *source, uint64_t length);
int kill(int pid);
void ps();
int block(int pid, int block);
int getPID();
void nice(int pid, int p);
void syscallSemPrint();
int syscallSemOpen(const char *name, size_t value, char created);
int syscallSemWait(const char *name);
int syscallSemPost(const char *name);
int syscallSemClose(const char *name);
#endif