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

void initFd(int fd)
{
    fds[fd] = (FILE_DESCRIPTOR *)pMalloc(sizeof(FILE_DESCRIPTOR));
}

int syscall_read(int fd, char *buffer, int n)
{
    if (fds[fd] == NULL)
        initFd(fd);
    if (fds[fd] == NULL)
        return -1; //error on pmalloc
    int i;
    for (i = 0; i < n; i++)
    {
        if (fds[fd]->buffer)
    }
    for (i = 0; i < n; i++)
    {
        while (is_buffer_empty())
        {
            _hlt();
        }
        if (!is_buffer_empty())
            buffer[i] = get_buffer();
    }

    return i;
}

int fdOccupied(int fds[], int fd)
{
    for (int i = 0; i < TOTAL_FDS; i++)
    {
        if (fds[i] == fd)
            return 1;
    }
    return 0;
}

void unblockPID(int fd)
{
    Header h = blockedHeader;
    while (h.current != NULL)
    {
        if (h.current->data.fdBlock == fd)
        {
            readyProcess(h.current->data.PID);
            return;
        }
        h.current = h.current->next; //es null el ultimo o es circular el next?
    }
}

int syscall_write(int fd, const char *buffer, int n)
{
    if (fds[fd] == NULL)
        initFd(fd);
    if (fds[fd] == NULL)
        return -1; //error on pmalloc
    int i;
    for (i = 0; i < n; i++)
    {
        if(fds[fd]->idxWrite )
        fds[fd]->buffer[i] = buffer[i];
    }
    fds[fd]->buffer[i] = '\0';
    unblockPID(fd);
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