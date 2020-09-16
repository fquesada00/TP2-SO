#ifndef _TASK_MANAGER_H_
#define _TASK_MANAGER_H_
#define MAX_PROGRAMS 10
#include <stdint.h>
#include "States.h"
#define STACK_SIZE 512
typedef struct ProgramType{
    uint64_t start;
    int screen;
    int ID;
}ProgramType;
struct TaskManager{
    ProgramType programs[MAX_PROGRAMS]; 
};
int loadProgram(uint64_t start);
void continueProgram(int num, uint64_t rsp);
void loadState(State * currentState,State state);
void saveState(State currentState,State * state);
#endif