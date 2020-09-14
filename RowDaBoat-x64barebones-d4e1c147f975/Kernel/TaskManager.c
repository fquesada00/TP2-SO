#include <stdint.h>
#include "video_driver.h"
#include "TaskManager.h"
#include <lib.h>
extern void _cli();
extern void _sti();
static struct TaskManager Tasker;
static int cantPrograms;
static int ids;
static int currentID = -1;
static char runningPrograms[MAX_PROGRAMS];
static uint64_t Stacks [MAX_PROGRAMS][STACK_SIZE];
static State states [MAX_PROGRAMS];
int loadProgram(uint64_t start)
{
    _cli();
    if(cantPrograms>= MAX_PROGRAMS)
    {
        _sti();
        return 1;
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
        if (currentID != -1)
        {
            saveState(*currentState, &(states[currentID]));
        }
        runningPrograms[num] = 1;
        currentID = num;
        states[num].rsp =(uint64_t) Stacks[num] + (STACK_SIZE-1);
        states[num].rip = Tasker.programs[num].start;
        currentState->rip = states[num].rip;
        currentState->rsp = states[num].rsp;
    }
    else
    {
        saveState(*currentState, &(states[currentID]));
        currentID = num;
        loadState(currentState, states[num]);
    }
    _sti();
}

void loadState(struct State *currentState, State state)
{

    memcpy(currentState,&state,sizeof(State));
}
void saveState(struct State currentState, State *state)
{
    memcpy(state,&currentState,sizeof(State));
}
void restartProgram(uint64_t rsp)
{
    struct State *currentState = (void *)rsp;
    currentState->rip = Tasker.programs[currentID].start; //rip del programa actual
    currentState->rsp =(uint64_t) Stacks[currentID] +STACK_SIZE-1;     //rsp del programa actual
}