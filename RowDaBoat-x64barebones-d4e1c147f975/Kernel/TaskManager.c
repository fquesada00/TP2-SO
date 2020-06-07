#include <stdint.h>
#include "Video_Driver/video_driver.h"
#include "TaskManager.h"
extern void _cli();
extern void _sti();
static struct TaskManager Tasker;
static int cantPrograms;
static int ids;
static int currentID = -1;
static char runningPrograms[MAX_PROGRAMS];
static State Userland;
static char Stacks [MAX_PROGRAMS][STACK_SIZE];
static Registers states [MAX_PROGRAMS];
int loadProgram(uint64_t start)
{
    _cli();
    if(cantPrograms>= MAX_PROGRAMS)
    {
        _sti();
        return 0;
    }
    ProgramType program;
    program.start = start;
    program.ID = ids++;
    program.screen = screenNumber();
    Tasker.programs[cantPrograms++] = program;
    _sti();
    return 0;
}
void continueProgram(int num, uint64_t rsp)
{
    _cli();
    changeScreen(Tasker.programs[num].ID);
    struct State *currentState = (void *)rsp;
    //Si aun no se cargo ningun programa, cargo el primero
    if (!runningPrograms[num])
    {
        if (currentID == -1)
        {
            saveState(*currentState, &Userland);
        }
        else
        {
            saveState(*currentState, &(states[currentID]));
        }
        runningPrograms[num] = 1;
        currentID = num;
        Stacks[num][STACK_SIZE - 1] = Userland.rip; //pongo la dir de retorno de mi programa en su stack
        states[num].rsp = Stacks[num] + STACK_SIZE-1 - 8;
        states[num].rip = Tasker.programs[num].start;
        currentState->rip = Tasker.programs[currentID].start;
        currentState->rsp = Stacks[currentID]+STACK_SIZE-1 - 8;
    }
    else
    {
        saveState(*currentState, &(states[currentID]));
        currentID = num;
        loadState(currentState, states[num]);
    }
    _sti();
}

void loadState(struct State *currentState, Registers state)
{
    currentState->r15 = state.r15;
    currentState->r14 = state.r14;
    currentState->r13 = state.r13;
    currentState->r12 = state.r12;
    currentState->r11 = state.r11;
    currentState->r10 = state.r10;
    currentState->r9 = state.r9;
    currentState->r8 = state.r8;
    currentState->rsi = state.rsi;
    currentState->rdi = state.rdi;
    currentState->rbp = state.rbp;
    currentState->rdx = state.rdx;
    currentState->rcx = state.rcx;
    currentState->rbx = state.rbx;
    currentState->rax = state.rax;
    currentState->flags = state.flags;
    currentState->rsp = state.rsp;
    currentState->rip = state.rip;
}
void saveState(struct State currentState, Registers *state)
{
    state->r15 = currentState.r15;
    state->r14 = currentState.r14;
    state->r13 = currentState.r13;
    state->r12 = currentState.r12;
    state->r11 = currentState.r11;
    state->r10 = currentState.r10;
    state->r9 = currentState.r9;
    state->r8 = currentState.r8;
    state->rsi = currentState.rsi;
    state->rdi = currentState.rdi;
    state->rbp = currentState.rbp;
    state->rdx = currentState.rdx;
    state->rcx = currentState.rcx;
    state->rbx = currentState.rbx;
    state->rax = currentState.rax;
    state->flags = currentState.flags;
    state->rip = currentState.rip;
    state->rsp = currentState.rsp;
}
void restartProgram(uint64_t rsp)
{
    struct State *currentState = (void *)rsp;
    currentState->rip = Tasker.programs[currentID].start; //rip del programa actual
    currentState->rsp = Stacks[currentID] +STACK_SIZE-1 - 8;     //rsp del programa actual
}