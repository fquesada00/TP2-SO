#include "keyboardDriver.h"
#include "video_driver.h"
#include "scheduler.h"
#include "list.h"
#include <stdint.h>
#include <lib.h>
#include "naiveConsole.h"
#include "standardstring.h"
#include "fileDescriptor.h"
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
            blockCurrent(fd);
        }
        buffer[i] = f->read[((f->idxR)++)%BUF_SIZE];
    }
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
    for(i = 0 ; i < n ; i++){
        f->write[((f->idxW)++)%BUF_SIZE] = buffer[i];

        // putChar(buffer[i]);
    }
    unblockProcess(fd);
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
        l->data.state = Terminated;
        l->tickets = 0;
        if (l->data.state == Ready)
            readyHeader.ready--;
        return 0;
    }

    return -1;
}
int processes()
{
    puts("Nombre    PID    Priority    RSP    StackBase    State\n");
    printList(&readyHeader);
}
int blockProcess(int pid, int block)
{
    elem_t e;
    e.PID = pid;
    listElem_t *le;
    State change;
    if (block)
    {
        change = Blocked;
        if(readyHeader.ready > 0)
            readyHeader.ready--;
    }
    else
    {
        change = Ready;
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
    if ((le = get(&readyHeader, e)) != NULL)
    {
        le->data.state = change;
        return 0;
    }
    return -1;
}
void printList(Header *head)
{
    if (head == NULL || head->first == NULL)
        return;
    listElem_t *next = head->first;
    while (next != NULL)
    {
        puts(next->data.name);
        puts("    ");
        char buff[255] = {0};
        uintToBase(next->data.PID, buff, 10);
        puts(buff);
        putChar('\t');
        uintToBase(next->priority, buff, 10);
        puts(buff);
        putChar('\t');
        uintToBase(next->data.rsp, buff, 16);
        puts("0x");
        puts(buff);
        putChar('\t');
        uintToBase(next->data.StackBase, buff, 16);
        puts("0x");
        puts(buff);
        putChar('\t');
        switch (next->data.state)
        {
        case Ready:
            puts("Ready");
            break;
        case Blocked:
            puts("Blocked");
            break;
        case Terminated:
            puts("Terminated");
            break;
        default:
            break;
        }
        putChar('\n');
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
    readyHeader.ready--;
    _int20();
    return 1;
}
void unblockForeground()
{
    blockProcess(idle_pid, 1);
    listElem_t *iter = readyHeader.first;
    while (iter != NULL && (iter->data.state != Blocked || iter->data.PID == idle_pid))
    {
        iter = iter->next;
    }
    if (iter == NULL){
        blockProcess(idle_pid,0);
        return;
    }
    iter->data.fdBlock = -1;
    readyHeader.ready++;
    iter->data.state = Ready;
}
void unblockProcess(int fd)
{
    listElem_t * iter = readyHeader.first;
    while (iter != NULL && iter->data.fdBlock != fd)
    {
        iter=iter->next;
    }
    if(iter == NULL)
        return;
    iter->data.fdBlock = -1;
    readyHeader.ready++;
    iter->data.state = Ready;
            
}
void blockCurrent(int fd)
{
    /*listElem_t r = removeCurrent(&readyHeader);
    r.data.rsp = rsp;
    if(isEmpty(&blockedHeader))
        initList(&blockedHeader,r.data,r.priority,r.tickets);
    else
        push(&blockedHeader,r.data,r.priority,r.tickets);
    _hlt();*/
    if(readyHeader.ready > 0)
        readyHeader.ready--;
    readyHeader.current->data.state = Blocked;
    readyHeader.current->data.fdBlock = fd;
    _hltAndCli();
    return;
}