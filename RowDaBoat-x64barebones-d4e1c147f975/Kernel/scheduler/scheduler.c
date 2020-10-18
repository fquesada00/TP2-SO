#include "scheduler.h"
#include "list.h"
#include "standardstring.h"
extern void _int20();

Header readyHeader = {0};
Header blockedHeader = {0};
int currentPIDs = 1;
int initializing = 1;
uint64_t stackSize = 0x10000;
extern file_t *stdin;
extern file_t *stdout;
extern void idle();
extern int idle_pid;
extern int blockProcess(int pid, int block);
void closeCurrentProcess(int fd);
int schedule(uint64_t rsp)
{
    if (!initializing && readyHeader.current->data.state != Terminated)
    {
        (readyHeader).current->data.rsp = rsp;
    }
    else
        initializing = 0;
    if (readyHeader.current->data.state == Terminated)
    {
        readyHeader.ready--;
        if (readyHeader.current->data.PID == idle_pid)
            idle_pid = 0;
        closeCurrentProcess(0);
        closeCurrentProcess(1);
        listElem_t l = removeCurrent(&readyHeader);
        for (int i = 0; i < l.data.argc; i++)
        {
            pFree(l.data.argv[i]);
        }
        pFree((l.data.StackBase - stackSize + sizeof(uint64_t)));
    }
    if (readyHeader.ready == 0)
    {
        if (idle_pid != 0)
            blockProcess(idle_pid, 0);
        // listElem_t *iter = readyHeader.first;
        // while (iter != NULL && iter->data.PID != idle_pid)
        // {
        //     iter = iter->next;
        // }
        // if (iter != NULL)
        // {
        //     iter->data.state = Ready;
        //     readyHeader.ready++;
        // }
    }
    else if (readyHeader.ready < 0)
        puts("ERROR");
    if (readyHeader.current->data.state == Ready && readyHeader.current->tickets--)
        return readyHeader.current->data.rsp;
    else
    {
        readyHeader.current->tickets = (MAX_PRIORITY - readyHeader.current->priority);
        elem_t e;
        do
        {
            e = next(&readyHeader);
        } while (e.state == Blocked);
        return e.rsp;
    }
}

int init_process(void *entry_point, int argc, char *argv[], int fg)
{
    // if (rsp != 0)
    //     readyHeader.current->data.rsp = rsp;
    uint64_t rsp = (uint64_t)pMalloc(stackSize * sizeof(uint64_t));
    if (rsp != NULL)
    {
        rsp += stackSize - (sizeof(uint64_t));
        int pid = currentPIDs++;
        char **args = pMalloc(argc * sizeof(char *));
        for (int i = 0; i < argc; i++)
        {
            args[i] = pMalloc(strlen(argv[i]) * sizeof(char));
            strcpy(args[i], argv[i]);
        }
        init_registers(entry_point, argc, args, rsp);
        init_PCB(rsp, pid, args, argc, fg);
        // if (initializing)
        //     _int20();
        //_hlt();
        return pid;
    }
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
    init->rbp = rsp;
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
void init_PCB(uint64_t rsp, int pid, char **args, int argcount, int fg)
{
    elem_t e = {0};
    e.PID = pid;
    e.rsp = rsp - sizeof(Swapping);
    e.StackBase = rsp;
    e.privilege = 5;
    if (fg)
    {
        e.fds[0] = stdin;
        stdin->reading++;
    }
    e.fds[1] = stdout;
    stdout->writing++;
    e.BlockID = -1;
    e.reason = NOTHING;
    e.state = Ready;
    readyHeader.ready++;
    strcpy(e.name, args[0]);
    e.argv = args;
    e.argc = argcount;
    e.fg = fg;
    if (initializing)
    {
        initList(&readyHeader, e, 5, 5);
    }
    else
        push(&readyHeader, e, 5, 5);
}

//Bloquea el proceso con cuyo PID sea pid y le establece como motivo el numero de fd que lo bloqueo
// void blockProcess(int pid,int fdBlock)
// {
//     elem_t e;
//     e.PID = pid;
//     listElem_t removed = removeCurrent(&readyHeader);
//     removed.data.fdBlock = fdBlock;
//     if(blockedHeader.current == NULL)
//         initList(&blockedHeader,removed.data,removed.priority,removed.tickets);
//     else
//         push(&blockedHeader,removed.data,removed.priority,removed.tickets);

// }