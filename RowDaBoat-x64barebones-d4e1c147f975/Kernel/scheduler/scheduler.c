#include "scheduler.h"
#include "list.h"
extern void _hlt();

static Header readyHeader={0};
static Header blockedHeader = {0};
int currentPIDs;
int initializing = 1;
uint64_t stackSize = 0x5000;

extern char * stdin;
extern char * stdout;

int schedule(uint64_t rsp)
{
    if (!initializing)
    {
        (readyHeader).current->data.rsp = rsp;
    }
    else
        initializing = 0;
    if (readyHeader.current->tickets--)
        return readyHeader.current->data.rsp;
    else
    {
        readyHeader.current->tickets = readyHeader.current->priority;
        elem_t e;
        e = next(&readyHeader);
        return e.rsp;
    }
}

int init_process(void *entry_point, int argc, char *argv[], uint64_t rsp)
{
    if (rsp != 0)
        readyHeader.current->data.rsp = rsp;
    rsp = (uint64_t)pMalloc(stackSize * sizeof(uint64_t)) + stackSize - (sizeof(uint64_t));
    int pid = currentPIDs++;
    init_registers(entry_point, argc, argv, rsp);
    init_PCB(rsp, pid);
    _hlt();
    return -1;
}
void init_registers(void *entry_point, int argc, char *argv[], uint64_t rsp)
{
    Swapping *init = rsp - sizeof(Swapping);
    init->ss = 0x0;
    init->rsp = rsp;
    init->flags = 0x202;
    init->cs = 0x8;
    init->rip = entry_point;
    init->rax = 1;
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
    elem_t e = {0};
    e.state = READY;
    e.PID = pid;
    e.rsp = rsp - sizeof(Swapping);
    e.privilege = 5;
    e.fds[0] = stdin;
    e.fds[1] = stdout;
    e.fdBlock = -1;
    if (initializing)
        initList(&readyHeader,e,5,5);
    else
        push(&readyHeader, e, 5, 5);
}

//Bloquea el proceso con cuyo PID sea pid y le establece como motivo el numero de fd que lo bloqueo
void blockProcess(int pid,int fdBlock)
{
    elem_t e;
    e.PID = pid;
    listElem_t removed = remove(&readyHeader);
    removed.data.fdBlock = fdBlock;
    if(blockedHeader.current == NULL)
        initList(&blockedHeader,removed.data,removed.priority,removed.tickets);
    else
        push(&blockedHeader,removed.data,removed.priority,removed.tickets);
    
}

void readyProcess(int pid)
{
    elem_t e;
    e.PID = pid;
    listElem_t removed = removeElement(&readyHeader,e);
    removed.data.fdBlock = -1;
    if(readyHeader.current == NULL)
        initList(&readyHeader,removed.data,removed.priority,removed.tickets);
    else
        push(&readyHeader,removed.data,removed.priority,removed.tickets);
    _hlt();

    
}