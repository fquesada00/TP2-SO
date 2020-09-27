#include "scheduler.h"
#include "list.h"
extern void _hlt();

listElem_t *PCBs;
int currentPIDs;
int initializing = 1;
uint64_t stackSize = 0x200;

int schedule(uint64_t rsp)
{
    if (!initializing)
    {
        (PCBs)->current->data.pcb.rsp = rsp;
    }
    else
        initializing = 0;
    if (PCBs->current->tickets--)
        return PCBs->current->data.pcb.rsp;
    else
    {
        PCBs->current->tickets = PCBs->current->priority;
        elem_t e;
        do
        {
            e = next(&PCBs);
        } while (e.pcb.state == BLOCKED);
        return e.pcb.rsp;
    }
}

int init_process(void *entry_point, int argc, char *argv[], uint64_t rsp)
{
    if (rsp != 0)
        PCBs->current->data.pcb.rsp = rsp;
    rsp = (uint64_t)pMalloc(stackSize * sizeof(uint64_t)) + stackSize - (sizeof(uint64_t));
    rsp -= sizeof(SwappingNoRAX);
    int pid = currentPIDs++;
    init_registers(entry_point, argc, argv, rsp);
    init_PCB(rsp, pid);
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
    elem_t e;
    e.pcb.state = READY;
    e.pcb.PID = pid;
    e.pcb.rsp = rsp;
    e.pcb.privilege = 5;
    e.pcb.state = READY;
    if (initializing)
        PCBs = newNode(e, 5, 5);
    else
        push(&PCBs, e, 5, 5);
}