#ifndef _STATES_H_
#define _STATES_H_
#include <stdint.h>
typedef struct State{
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
}State;
void save_regs(uint64_t rsp);
State * getRegs();
void printreg(State saved);
uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);
#endif