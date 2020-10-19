#include "scheduler.h"
#include "memory_manager.h"
#include "video_driver.h"
#include "fds.h"
#include "interrupts.h"
#include "list.h"
#include "standardstring.h"
#include "syscalls.h"
#include "standardLib.h"
#include "standardstring.h"
#include <stddef.h>
extern file_t *stdin;
extern file_t *stdout;
extern int idle_pid;
Header readyHeader = {0};
int currentPIDs = 1;
int initializing = 1;
uint64_t stackSize = 0x10000;
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
        pFree((void *)(l.data.StackBase - stackSize + sizeof(uint64_t)));
    }
    if (readyHeader.ready == 0)
    {
        if (idle_pid != 0)
            blockProcess(idle_pid, 0);
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
    uint64_t rsp = (uint64_t)pMalloc(stackSize * sizeof(uint64_t));
    if ((size_t *)rsp != NULL)
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
        return pid;
    }
    return -1;
}

void init_registers(void *entry_point, int argc, char *argv[], uint64_t rsp)
{
    Swapping *init = (void *)rsp - sizeof(Swapping);
    init->ss = 0x0;
    init->rsp = rsp;
    init->flags = 0x202;
    init->cs = 0x8;
    init->rip = (uint64_t)entry_point;
    init->rax = 1;
    init->rbx = 2;
    init->rcx = 3;
    init->rdx = 4;
    init->rbp = rsp;
    init->rdi = argc;
    init->rsi = (uint64_t)argv;
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
    elem_t e = {{0}, 0, {0}, 0, 0, 0, 0, 0, 0, 0, 0, 0};
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

PCB *getPCB(size_t pid)
{
    elem_t e;
    e.PID = pid;
    listElem_t *l = get(&readyHeader, e);
    return &l->data;
}

void unblockProcessByPCB(PCB *process)
{
    if (process == NULL)
        return;
    if (process->state == Blocked)
    {
        blockProcess(idle_pid, 1);
        process->BlockID = -1;
        process->reason = NOTHING;
        process->state = Ready;
        readyHeader.ready++;
    }
}

int getPID()
{
    return readyHeader.current->data.PID;
}

void nice(int pid, int p)
{
    elem_t e;
    e.PID = pid;
    listElem_t *process = get(&readyHeader, e);
    if (process == NULL)
    {
        return;
    }
    process->priority = p;
    process->data.privilege = p;
    process->tickets = 0;
}

int pExit()
{
    readyHeader.current->data.state = Terminated;
    readyHeader.current->tickets = 0;
    realeaseWaiting(readyHeader.current->data.PID);
    _int20();
    puts("=========================NUNCA LLEGUE========================");
    return 1;
}

void unblockProcess(int id, BlockReason reason)
{
    listElem_t *iter = readyHeader.first;
    if (idle_pid != 0)
        blockProcess(idle_pid, 1);
    while (iter != NULL && (iter->data.reason != reason || iter->data.BlockID != id || iter->data.state != Blocked))
    {
        iter = iter->next;
    }
    if (iter == NULL)
    {
        return;
    }
    iter->data.BlockID = -1;
    iter->data.reason = NOTHING;
    readyHeader.ready++;
    iter->data.state = Ready;
}

void blockCurrent(int id, BlockReason reason)
{
    if (readyHeader.current->data.state != Terminated)
    {
        if (readyHeader.current->data.state == Ready)
            readyHeader.ready--;
        readyHeader.current->data.state = Blocked;
        readyHeader.current->data.BlockID = id;
        readyHeader.current->data.reason = reason;
        readyHeader.current->tickets = 0;
    }
    _int20();
    return;
}
int waitPID(int pid)
{
    blockCurrent(pid, PID);
}

void yield()
{
    readyHeader.current->tickets = 0;
    _int20();
}

void realeaseWaiting(int pid)
{
    listElem_t *iter = readyHeader.first;
    while (iter != NULL && (iter->data.BlockID != pid || iter->data.reason != PID))
    {
        iter = iter->next;
    }
    if (iter == NULL)
        return;
    iter->data.state = Ready;
    readyHeader.ready++;
    iter->data.BlockID = -1;
    iter->data.reason = NOTHING;
}

int blockProcess(int pid, int block)
{
    elem_t e;
    e.PID = pid;
    listElem_t *le;
    le = get(&readyHeader, e);
    State change;
    if (le != NULL)
    {
        if (block)
        {
            change = Blocked;
            if (le->data.state != Blocked)
                readyHeader.ready--;
        }
        else
        {
            change = Ready;
            if (le->data.state != Ready)
                readyHeader.ready++;
        }
        if (le->data.state != Terminated)
        {
            le->data.state = change;

            return 0;
        }
    }
    return -1;
}

int pKill(int pid)
{
    elem_t e;
    e.PID = pid;
    listElem_t *l;
    if ((l = get(&readyHeader, e)) != NULL)
    {
        if (l->data.state == Ready)
            readyHeader.ready--;
        l->data.state = Terminated;
        l->tickets = 0;
        realeaseWaiting(l->data.PID);
        return 0;
    }
    return -1;
}

void processStatus()
{
    int width = (SCREEN_WIDTH / CHARSIZE) / 6; // Here 6 its the number of words we want to print. If wanted more or less word per line, change it
    int widthHeader = width + 5;
    char buffer[256] = {0};
    char separation[256] = "                                                              ";
    int length = 0;
    length = strcpy(buffer, "NOMBRE");
    syscall_write(1, buffer, length);
    syscall_write(1, separation, widthHeader - length);
    length = strcpy(buffer, "PID");
    syscall_write(1, buffer, length);
    syscall_write(1, separation, width - length);
    length = strcpy(buffer, "PRIO");
    syscall_write(1, buffer, length);
    syscall_write(1, separation, width - length);
    length = strcpy(buffer, "RSP");
    syscall_write(1, buffer, length);
    syscall_write(1, separation, width - length);
    length = strcpy(buffer, "BP");
    syscall_write(1, buffer, length);
    syscall_write(1, separation, width - length);
    length = strcpy(buffer, "STATE");
    syscall_write(1, buffer, length);
    syscall_write(1, separation, width - length);
    if (readyHeader.first == NULL)
        return;
    listElem_t *next = readyHeader.first;
    char auxBuff[256] = {0};
    syscall_write(1, "\n", 1);
    while (next != NULL)
    {
        length = strcpy(buffer, next->data.name);
        syscall_write(1, buffer, length);
        syscall_write(1, separation, widthHeader - length);
        length = uintToBase(next->data.PID, buffer, 10);
        syscall_write(1, buffer, length);
        syscall_write(1, separation, width - length);
        length = uintToBase(next->priority, buffer, 10);
        syscall_write(1, buffer, length);
        syscall_write(1, separation, width - length);
        length = strcpy(buffer, "0x");
        length += uintToBase(next->data.rsp, auxBuff, 16);
        strcat(buffer, auxBuff);
        syscall_write(1, buffer, length);
        syscall_write(1, separation, width - length);
        length = strcpy(buffer, "0x");
        length += uintToBase(next->data.StackBase, auxBuff, 16);
        strcat(buffer, auxBuff);
        syscall_write(1, buffer, length);
        syscall_write(1, separation, width - length);
        switch (next->data.state)
        {
        case Ready:
            length = strcpy(buffer, "R");
            break;
        case Blocked:
            length = strcpy(buffer, "B");
            break;
        case Terminated:
            length = strcpy(buffer, "T");
            break;
        default:
            break;
        }
        length = strcat(buffer, "\n");
        syscall_write(1, buffer, length);
        next = next->next;
    }
}