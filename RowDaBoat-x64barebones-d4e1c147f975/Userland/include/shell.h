#ifndef _SHELL_H_
#define _SHELL_H_
#define MAX_ARGS 3
#define CANT_CMDS 15
#define MAXLENGHT 256
void ps(void);
void help(void);
int kill(int pid);
int block(int pd, int block);
int nice(int pid, int p);
void help();
void sem();
void mem();
void pipe();
#endif