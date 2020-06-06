#include <stdint.h>
#include "Video_Driver/video_driver.h"
#include "TaskManager.h"
static struct TaskManager Tasker;
static int cantPrograms;
static int ids;
static int currentID=-1;
static char runningPrograms[MAX_PROGRAMS];
int loadProgram(uint64_t start)
{
    ProgramType program;
    program.start=start;
    program.ID=ids++;
    program.screen=screenNumber();
    Tasker.programs[cantPrograms++]=program;
}
void continueProgram(int num, uint64_t rsp)
{
    changeScreen(Tasker.programs[num].screen);
    struct State * currentState = (void*)rsp;
    //Si aun no se cargo ningun programa, cargo el primero
    if(!runningPrograms[num])
    {
        if(currentID !=-1)
            saveState(*currentState,&(Tasker.states[currentID]));
        runningPrograms[num]=1;
        currentID = num;
        Tasker.Stacks[num][STACK_SIZE-1]=currentState->rip;//pongo la dir de retorno de mi programa en su stack
        Tasker.states[num].rsp=Tasker.Stacks[num]-(char)8;
        Tasker.states[num].rip=Tasker.programs[num].start;
        currentState->rip=Tasker.programs[currentID].start;
        currentState->rsp=Tasker.Stacks[currentID]-8;
    }else
        {
            saveState(*currentState,&(Tasker.states[currentID]));
            currentID=num;
            loadState(currentState,Tasker.states[num]);
        }
}

void loadState(struct State * currentState,Registers state)
{
    currentState->r15=state.r15;
    currentState->r14=state.r14;
    currentState->r13=state.r13;
    currentState->r12=state.r12;
    currentState->r11=state.r11;
    currentState->r10=state.r10;
    currentState->r9=state.r9;
    currentState->r8=state.r8;
    currentState->rsi=state.rsi;
    currentState->rdi=state.rdi;
    currentState->rbp=state.rbp;
    currentState->rdx=state.rdx;
    currentState->rcx=state.rcx;
    currentState->rbx=state.rbx;
    currentState->rax=state.rax;
    currentState->flags=state.flags;
    currentState->rsp=state.rsp;
    currentState->rip=state.rip;
    


}
void saveState(struct State currentState,Registers * state)
{
    state->r15=currentState.r15;
    state->r14=currentState.r14;
    state->r13=currentState.r13;
    state->r12=currentState.r12;
    state->r11=currentState.r11;
    state->r10=currentState.r10;
    state->r9=currentState.r9;
    state->r8=currentState.r8;
    state->rsi=currentState.rsi;
    state->rdi=currentState.rdi;
    state->rbp=currentState.rbp;
    state->rdx=currentState.rdx;
    state->rcx=currentState.rcx;
    state->rbx=currentState.rbx;
    state->rax=currentState.rax;
    state->flags=currentState.flags;
    state->rip=currentState.rip;
    state->rsp=currentState.rsp;
}
void restartProgram(uint64_t rsp)
{
    struct State * currentState = (void*)rsp;
    currentState->rip=Tasker.programs[currentID].start; //rip del programa actual
    currentState->rsp=Tasker.Stacks[currentID]-8; //rsp del programa actual
}