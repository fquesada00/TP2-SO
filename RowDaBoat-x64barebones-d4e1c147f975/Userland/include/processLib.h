#ifndef _PROCESS_LIB_H_
#define _PROCESS_LIB_H_
#define MAXPRIVILAGE 10
extern int _execv(void *entry_point, int argc, char *argv[],int fg);
extern int pKill(int pid);
extern void printProcesses();
extern int pBlock(int pid, int block);
extern int _getPID();
extern void _nice(int pid, int p);
extern int _waitPID(int pid);
extern int _yield();
extern int _exit(void);
#endif