#include "keyboardDriver.h"
#include "video_driver.h"
#include "scheduler.h"
#include "list.h"
#include <stdint.h>
#include <lib.h>
#include "naiveConsole.h"
#include "standardstring.h"
#include "fileDescriptor.h"
#include "pipe.h"
extern void _hlt();
extern void _cli();
extern void _hltAndCli();
extern Header readyHeader;
extern Header blockedHeader;
extern uint64_t stackSize;
extern file_t stdout;
extern int schedule(int rsp);
extern _int20();
extern int idle_pid;
int syscall_read(int fd, char *buffer, int n)
{
    if (fd < 0 || fd > MAX_FD)
    {
        return -1;
    }
    int i;
    file_t *f = readyHeader.current->data.fds[fd];
    if (f == NULL)
        return -1;
    for (i = 0; i < n; i++)
    {
        while (f->idxR == f->idxW)
        {
            blockCurrent(fd, FD_READ);
        }
        buffer[i] = f->read[((f->idxR)++) % BUF_SIZE];
    }
    unblockProcess(fd, FD_WRITE);
    return i;
}
int syscall_write(int fd, const char *buffer, int n)
{
    if (fd < 0 || fd > MAX_FD)
    {
        return -1;
    }
    int i;
    file_t *f = readyHeader.current->data.fds[fd];
    if (f == NULL)
        return -1;
    for (i = 0; i < n; i++)
    {
        if (i != 0 && i % BUF_SIZE == 0)
            blockCurrent(fd, FD_WRITE);
        f->write[((f->idxW)++) % BUF_SIZE] = buffer[i];

        // putChar(buffer[i]);
    }
    //f->write[((f->idxW)++)%BUF_SIZE] = 0;
    unblockProcess(fd, FD_READ);
    return i;
}
int syscall_registers(uint64_t *regs)
{
    // State * savedState = getRegs();
    //memcpy(regs,savedState,sizeof(State));
    return 0;
}

int syscall_read_mem(uint64_t address, char *buff)
{
    memcpy((void *)buff, (void *)address, 32);
    return 0;
}

int pKill(int pid)
{
    elem_t e;
    e.PID = pid;
    /*listElem_t l = removeElement(&readyHeader, e);
    if(l.priority == -1){
        l = removeElement(&blockedHeader,e);
        if(l.priority == -1)
            return-1;
    }
    pFree((l.data.StackBase - stackSize + sizeof(uint64_t)));
    */
    listElem_t *l;
    listElem_t el;
    if ((l = get(&readyHeader, e)) != NULL)
    {
        l->data.PID = 0;
        l->data.state = Terminated;
        l->tickets = 0;
        close(0);
        close(1);
        realeaseWaiting(l->data.PID);
        return 0;
    }

    return -1;
}
int processes()
{
    printList(&readyHeader);
}
int blockProcess(int pid, int block)
{
    elem_t e;
    e.PID = pid;
    listElem_t *le;
    le = get(&readyHeader, e);
    State change;
    if (block)
    {
        change = Blocked;
        //if(readyHeader.ready > 0)
        if (le->data.state != Blocked)
            readyHeader.ready--;
    }
    else
    {
        change = Ready;
        if (le->data.state != Ready)
            readyHeader.ready++;
    }
    /*Header *toRemove;
    Header *toInsert;
    State change;
    e.PID = pid;
    if (block)
    {
        toRemove = &readyHeader;
        toInsert = &blockedHeader;
        change = Blocked;
    }
    else
    {
        toRemove = &blockedHeader;
        toInsert = &readyHeader;
        change = Ready;
    }
    if(pid == readyHeader.current->data.PID)
    {
        return -1;
    }
    le = removeElement(toRemove, e);
    le.data.state = change;
    if (le.priority == -1)
        return -1;
    if (toInsert->first == NULL)
        initList(toInsert, le.data, le.priority, le.tickets);
    else
        push(toInsert, le.data, le.priority, le.tickets);*/
    if (le != NULL)
    {
        if (le->data.state != Terminated)
        {
            le->data.state = change;
            return 0;
        }
    }
    return -1;
}
void printList(Header *head)
{
    int width = (1024 / 16) / 6; //CAMBIAR POR CTES
    int widthHeader = width + 5;
    char buffer[256] = {0};
    char separation[256] = "                                                              ";
    int length = 0, auxLength = 0;
    length = strcpy(buffer, "NOMBRE");
    syscall_write(1, buffer, length);
    syscall_write(1, separation, widthHeader - length);
    // puts(buffer);
    // putsN(separation, widthHeader - length);
    length = strcpy(buffer, "PID");
    syscall_write(1, buffer, length);
    syscall_write(1, separation, width - length);
    // puts(buffer);
    // putsN(separation, width - length);
    length = strcpy(buffer, "PRIO");
    // puts(buffer);
    // putsN(separation, width - length);
    syscall_write(1, buffer, length);
    syscall_write(1, separation, width - length);
    length = strcpy(buffer, "RSP");
    syscall_write(1, buffer, length);
    syscall_write(1, separation, width - length);
    // puts(buffer);
    // putsN(separation, width - length);
    length = strcpy(buffer, "BP");
    syscall_write(1, buffer, length);
    syscall_write(1, separation, width - length);
    // puts(buffer);
    // putsN(separation, width - length);
    length = strcpy(buffer, "STATE");
    syscall_write(1, buffer, length);
    // puts(buffer);
    syscall_write(1, separation, width - length);
    // putsN(separation, width - length);
    if (head == NULL || head->first == NULL)
        return;
    listElem_t *next = head->first;
    char auxBuff[256] = {0};
    // putChar('\n');
    syscall_write(1, "\n", 1);
    while (next != NULL)
    {
        length = strcpy(buffer, next->data.name);
        syscall_write(1, buffer, length);
        // putsN(buffer, length);
        // putsN(separation, widthHeader - length);
        syscall_write(1, separation, widthHeader - length);
        length = uintToBase(next->data.PID, buffer, 10);
        // puts(buffer);
        // putsN(separation, width - length);
        syscall_write(1, buffer, length);
        syscall_write(1, separation, width - length);
        length = uintToBase(next->priority, buffer, 10);
        // puts(buffer);
        // putsN(separation, width - length);
        syscall_write(1, buffer, length);
        syscall_write(1, separation, width - length);
        length = strcpy(buffer, "0x");
        length += uintToBase(next->data.rsp, auxBuff, 16);
        strcat(buffer, auxBuff);
        // puts(buffer);
        // putsN(separation, width - length);
        syscall_write(1, buffer, length);
        syscall_write(1, separation, width - length);
        length = strcpy(buffer, "0x");
        length += uintToBase(next->data.StackBase, auxBuff, 16);
        strcat(buffer, auxBuff);
        syscall_write(1, buffer, length);
        // puts(buffer);
        syscall_write(1, separation, width - length);
        // putsN(separation, width - length);
        switch (next->data.state)
        {
        case Ready:
            length = strcpy(buffer, "R");
            // syscall_write(1, buffer, length);
            break;
        case Blocked:
            length = strcpy(buffer, "B");
            // syscall_write(1, buffer, length);
            break;
        case Terminated:
            length = strcpy(buffer, "T");
            // syscall_write(1, buffer, length);
            break;
        default:
            break;
        }
        // puts(buffer);
        //syscall_write(1, "\n", 1);
        length = strcat(buffer, "\n");
        syscall_write(1, buffer, length);
        next = next->next;
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
int exit(int status)
{
    readyHeader.current->data.state = Terminated;
    //if(readyHeader.ready > 0)
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
    while (iter != NULL && (iter->data.reason != reason || iter->data.BlockID != id))
    {

        iter = iter->next;
    }
    if (iter == NULL)
    {
        //puts("NULL");
        // if (idle_pid != 0 && readyHeader.ready == 0)
        //     blockProcess(idle_pid, 0);
        return;
    }
    //puts(iter->data.name);
    iter->data.BlockID = -1;
    iter->data.reason = NOTHING;
    readyHeader.ready++;
    iter->data.state = Ready;
}
void blockCurrent(int id, BlockReason reason)
{
    /*listElem_t r = removeCurrent(&readyHeader);
    r.data.rsp = rsp;
    if(isEmpty(&blockedHeader))
        initList(&blockedHeader,r.data,r.priority,r.tickets);
    else
        push(&blockedHeader,r.data,r.priority,r.tickets);
    _hlt();*/
    //if(readyHeader.ready > 0)
    if (readyHeader.current->data.state != Terminated)
    {
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
PCB *getPCB(size_t pid)
{
    elem_t e;
    e.PID = pid;
    listElem_t *l = get(&readyHeader, e);
    return &l->data;
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
int close(int fd)
{
    PCB *pcb = &readyHeader.current->data;
    switch (pcb->fds[fd]->type)
    {
    case PIPE:
        removeFromPipe(pcb->fds[fd]->id, pcb);
        break;
    case STDINOUT:
        pcb->fds[0] = NULL;
        pcb->fds[1] = NULL;
        break;
    }
}