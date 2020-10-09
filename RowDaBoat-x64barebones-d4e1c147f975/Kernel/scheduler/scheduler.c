#include "scheduler.h"
#include "list.h"
#include "standardstring.h"
extern void _hlt();
extern FILE_DESCRIPTOR * fds[TOTAL_FDS];
Header readyHeader={0};
Header blockedHeader = {0};
int currentPIDs;
int initializing = 1;
uint64_t stackSize = 0x100000;

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
    init_PCB(rsp, pid,argv[0]);
    _hlt();
    return -1;
}

int init_process_with_pipes(void *entry_point, int argc, char *argv[], uint64_t rsp,int read_pipe,int write_pipe){
    init_process(entry_point,argc,argv,rsp);
    //creo que se caga por el hlt
    elem_t e = pop(&readyHeader);
    e->read_pipe = read_pipe;
    e->write_pipe = write_pipe;
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
void init_PCB(uint64_t rsp, int pid,char * name)
{
    elem_t e = {0};
    e.PID = pid;
    e.rsp = rsp - sizeof(Swapping);
    e.StackBase = rsp - sizeof(uint64_t);
    e.privilege = 5;
    e.fds[0] = fds[0];
    e.fds[1] = fds[1];
    for(int i = 2 ; i < TOTAL_FDS ; i++) e.fds[i] = NULL;
    e.fdBlock = -1;
    strcpy(e.name,name);
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