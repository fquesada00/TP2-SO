#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_
#include "memory_manager.h"
#include "video_driver.h"
typedef enum State
{
    READY,
    BLOCKED,
    TERMINATED
} State;
typedef struct PCB
{
    int PID;
    State state;
    int fds[512];
    uint64_t rsp;
    int privilege;
}PCB;

typedef struct SwappingnNoRAX
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
    uint64_t rip;
    uint64_t cs;
    uint64_t flags;
    uint64_t rsp;
    uint64_t ss;
} SwappingNoRAX;
#endif