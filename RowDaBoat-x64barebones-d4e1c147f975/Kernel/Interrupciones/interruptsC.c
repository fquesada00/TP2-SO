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
            blockCurrent(fd,FD);
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
    //f->write[((f->idxW)++)%BUF_SIZE] = 0;
    unblockProcess(fd,FD);
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
        //if(readyHeader.ready > 0)
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
    //if(readyHeader.ready > 0)
        readyHeader.ready--;
    readyHeader.current->tickets = 0;
    _int20();
    puts("=========================NUNCA LLEGUE========================");
    return 1;
}
void unblockProcess(int id, BlockReason reason)
{   
    listElem_t * iter = readyHeader.first;
    while (iter != NULL && (iter->data.reason != reason || iter->data.BlockID != id))
    {
    
        iter=iter->next;
    }
    if(iter == NULL){
        puts("NULL");
        blockProcess(idle_pid,0);
        return;
    }
    //puts(iter->data.name);
    puts("El proceso que se desbloqueo fue: ");
    char buff[256] = {0};
    uintToBase(iter->data.PID,buff,10);
    puts(buff);
    putChar('\n');
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
    puts("El proceso que se bloqueo fue: ");
    char buff[256] = {0};
    uintToBase(readyHeader.current->data.PID,buff,10);
    puts(buff);
    putChar('\n');
    //if(readyHeader.ready > 0)
        readyHeader.ready--;
    readyHeader.current->data.state = Blocked;
    readyHeader.current->data.BlockID = id;
    readyHeader.current->data.reason = reason;
    readyHeader.current->tickets = 0;
    _int20();
    return;
}
int waitPID(int pid)
{
    blockCurrent(pid,PID);
}
void unblockProcessByPCB(PCB * process)
{
    if(process == NULL)
        return;
    puts("El proceso que se desbloqueo fue: ");
    char buff[256] = {0};
    uintToBase(process->PID,buff,10);
    puts(buff);
    putChar('\n');
    blockProcess(idle_pid,1);
    process->BlockID = -1;
    process->reason = NOTHING;
    process->state =Ready;
    readyHeader.ready++;
}
PCB * getPCB(size_t pid)
{
    elem_t e;
    e.PID = pid;
    listElem_t * l = get(&readyHeader,e);
    return &l->data;
}
void yield()
{
    readyHeader.current->tickets = 0;
    _int20();
}