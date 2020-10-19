#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_
#include "ctes.h"
#include "fds.h"
#include <stdint.h>
#include <stddef.h>
typedef enum State
{
    Ready,
    Blocked,
    Terminated
} State;
typedef enum BlockReason
{
    FD_READ,
    FD_WRITE,
    PID,
    SEM,
    NOTHING
}BlockReason;
typedef struct PCB
{
    char name[255];
    int PID;
    file_t * fds[MAX_FDS];
    uint64_t rsp;
    uint64_t StackBase;
    int privilege;
    BlockReason reason;
    int BlockID;
    State state;
    char ** argv;
    int argc;
    int fg;
} PCB;

typedef struct Swapping
{
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    uint64_t rip;
    uint64_t cs;
    uint64_t flags;
    uint64_t rsp;
    uint64_t ss;
} Swapping;

int schedule(uint64_t rsp);
int init_process(void *entry_point, int argc, char *argv[],int fg);
void init_registers(void *entry_point, int argc, char *argv[], uint64_t rsp);
void init_PCB(uint64_t rsp, int pid, char **args, int argcount,int fg);
PCB *getPCB(size_t pid);
void unblockProcessByPCB(PCB *process);
int getPID();
int waitPID(int pid);
void realeaseWaiting(int pid);
void nice(int pid, int p);
int pExit();
void unblockProcess(int id, BlockReason reason);
void blockCurrent(int id, BlockReason reason);
int blockProcess(int pid, int block);
int pKill(int pid);
void processStatus();
void yield();

#endif