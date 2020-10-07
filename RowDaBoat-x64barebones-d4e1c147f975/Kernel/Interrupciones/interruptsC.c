#include "keyboardDriver.h"
#include "video_driver.h"
#include "scheduler.h"
#include "list.h"
#include <stdint.h>
#include <lib.h>
#include "naiveConsole.h"
#include "fds.h"
#include "memory_manager.h"

extern FILE_DESCRIPTOR *fds[TOTAL_FDS];
extern void _hlt();
extern Header readyHeader;
extern Header blockedHeader;
extern uint64_t stackSize;

int initFd(fd)
{
    fds[fd] = (FILE_DESCRIPTOR *)pMalloc(sizeof(FILE_DESCRIPTOR));
    if (fds[fd] == NULL)
        return -1;
    for (int i = 0; i < TOTAL_FDS; i++)
    {
        fds[fd]->blockedPids[i] = -1;
        fds[fd]->pids[i] = -1;
    }
    return 1;
}
int syscall_read(int fd, char *buffer, int n)
{
    if (fd != 0 && fd != 1 && fds[fd] == NULL)
        if (initFd(fd) == -1)
            return -1;
    int i;
    for (i = 0; i < n; i++)
    {
        if (fds[fd]->idxRead == fds[fd]->idxWrite)
            insertAndBlockPid(fd);
        if ((BUFFER_SIZE - fds[fd]->idxRead) == 1)
            fds[fd]->idxRead = 0;
        buffer[i] = fds[fd]->buffer[(fds[fd]->idxRead)++];
    }
    return i;
}

int syscall_write(int fd, const char *buffer, int n)
{
    if (fd != 0 && fd != 1 && fds[fd] == NULL)
        initFd(fd);
    if (fds[fd] == NULL)
        return -1; //error on pmalloc
    int i;
    for (i = 0; i < n; i++)
    {
        if ((BUFFER_SIZE - fds[fd]->idxWrite) == 1)
            fds[fd]->idxWrite = 0;
        fds[fd]->buffer[(fds[fd]->idxWrite)++] = buffer[i];
    }
    fds[fd]->buffer[fds[fd]->idxWrite] = '\0';
    removeAndUnblockPid(fd);
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
    listElem_t l = removeElement(&readyHeader, e);
    pFree((l.data.StackBase - stackSize + sizeof(uint64_t)));
    return 0;
}
int processes()
{
    listElem_t *next = readyHeader.first;
    puts("Nombre    PID    Priority    RSP    StackBase\n");
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
        putChar('\n');
        next = next->next;
    }
}