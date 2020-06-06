#ifndef _TASK_MANAGER_H_
#define _TASK_MANAGER_H_
#define MAX_PROGRAMS 40
#include <stdint.h>
#define STACK_SIZE 4096
typedef struct Registers{
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
    uint64_t flags;
    uint64_t rip;
    uint64_t rsp;

}Registers;
typedef struct ProgramType{
    uint64_t start;
    int screen;
    int ID;
}ProgramType;
struct State{
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
};
struct TaskManager{
    ProgramType programs[MAX_PROGRAMS];
    Registers states [MAX_PROGRAMS];// 
    char Stacks [MAX_PROGRAMS][STACK_SIZE]; //4KB de stack cada programa
};
int loadProgram(uint64_t start);
void continueProgram(int num, uint64_t rsp);
void loadState(struct State * currentState,Registers state);
void saveState(struct State currentState,Registers * state);
#endif