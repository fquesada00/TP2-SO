#ifndef _SHELL_H_
#define _SHELL_H_
#define MAX_ARGS 4
#define CANT_CMDS 21
#define MAXLENGHT 256
#include <stdint.h>
void ps(void);
void help(void);
int kill(int pid);
int block(int pd, int block);
void nice(int pid, int p);
void help();
void sem();
void mem(uint64_t address);
void pipe();
void syscall_mem();
int isBuiltIn(int idx);
void myPid();
#endif