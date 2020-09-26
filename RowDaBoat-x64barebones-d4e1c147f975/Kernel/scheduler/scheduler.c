#include <stdint.h>
#include "memory_manager.h"
#include "video_driver.h"
extern void _hlt();
//Provisorio hasta tener malloc completamente funcional
#define MAX_PROGRAMS 2
uint64_t stack_size = 0x200;
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
static PCB PCBs[MAX_PROGRAMS];
int current_process;
int currentPIDs;
int initializing = 1;
int schedule(uint64_t rsp)
{
    if(!initializing){
        PCBs[current_process].rsp = rsp;
    }else
        initializing = 0;
    do
    {
        if(current_process+1 >= MAX_PROGRAMS)
            current_process = 0;
        else
            current_process++;

    } while (PCBs[current_process].rsp == 0);
    return PCBs[current_process].rsp;
}


int init_process(void *entry_point, int argc, char *argv[], uint64_t rsp)
{
    if(rsp != 0)
        PCBs[current_process].rsp = rsp;
    rsp = (uint64_t) pMalloc(stack_size * sizeof(uint64_t) ) + stack_size - (sizeof(uint64_t));
    rsp -=sizeof(SwappingNoRAX);    
    int pid = currentPIDs++;
    init_registers(entry_point,argc,argv,rsp);
    init_PCB(rsp,pid);
    _hlt();
    return -1;    
    
}
void init_registers(void *entry_point, int argc, char *argv[], uint64_t rsp)
{
    SwappingNoRAX *init = rsp;
    init->ss = 0x0;
    init->rsp = (uint64_t)(rsp + sizeof(struct SwappingnNoRAX));
    init->flags = 0x202;
    init->cs = 0x8;
    init->rip = entry_point;
    init->rbx = 2;
    init->rcx = 3;
    init->rdx = 4;
    init->rbp = init->rsp;
    init->rdi = argc;
    init->rsi = argv;
    init->r8 = 8;
    init->r9 = 9;
    init->r10 = 10;
    init->r11 = 11;
    init->r12 = 12;
    init->r13 = 13;
    init->r14 = 14;
    init->r15 = 15;
}
void init_PCB(uint64_t rsp, int pid)
{
    PCB *pcb = &(PCBs[pid]);
    pcb->state = READY;
    pcb->PID = pid;
    pcb->rsp = rsp;
    pcb->privilege = 120;
    PCBs->state = READY;
    char pcbs[256];
}