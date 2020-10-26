#include "syscalls.h"
#include "scheduler.h"
#include <stdint.h>
#include <standardLib.h>
#include "memory_manager.h"
#include "buddy_memory_manager.h"
#include "standardstring.h"
#include "fds.h"
#include "interrupts.h"
#include "ctes.h"
#include "list.h"
extern Header readyHeader;
extern Header blockedHeader;
extern uint64_t stackSize;
extern size_t heap_size;
extern size_t free_size;

int syscall_read(int fd, char *buffer, int n)
{
    if (fd < 0 || fd > MAX_FDS)
    {
        return -1;
    }
    int i;
    file_t *f = readyHeader.current->data.fds[fd];
    if (f == NULL)
        return -1;
    for (i = 0; i < n; i++)
    {
        while (*(f->idxRead) >= *(f->idxWrite))
        {
            blockCurrent(readyHeader.current->data.fds[fd]->id, FD_READ);
        }
        buffer[i] = f->read[((*(f->idxRead))++) % BUFF_SIZE];
    }
    unblockProcess(readyHeader.current->data.fds[fd]->id, FD_WRITE);
    return i;
}
int syscall_write(int fd, const char *buffer, int n)
{
    if (fd < 0 || fd > MAX_FDS)
    {
        return -1;
    }
    int i;
    file_t *f = readyHeader.current->data.fds[fd];
    if (f == NULL)
        return -1;
    for (i = 0; i < n; i++)
    {
        if (i != 0 && i % BUFF_SIZE == 0)
            blockCurrent(readyHeader.current->data.fds[fd]->id, FD_WRITE);
        f->write[((*(f->idxWrite))++) % BUFF_SIZE] = buffer[i];
    }
    unblockProcess(readyHeader.current->data.fds[fd]->id, FD_READ);
    return i;
}

void syscall_mem()
{
    char buff[256] = "Estado de la memoria\n\nTotal = ";
    char auxBuff[256] = {0};
    int ocupada = heap_size - free_size;
    uintToBase(heap_size, auxBuff, 10);
    strcat(buff, auxBuff);
    int length = strcat(buff, " B");
    syscall_write(1, buff, length);
    strcpy(buff, " ; Ocupada = ");
    uintToBase(ocupada, auxBuff, 10);
    strcat(buff, auxBuff);
    length = strcat(buff, " B");
    syscall_write(1, buff, length);
    strcpy(buff, " ; Libre = ");
    uintToBase(free_size, auxBuff, 10);
    strcat(auxBuff, " B\n");
    length = strcat(buff, auxBuff);
    syscall_write(1, buff, length);
}

void writeToStd(char *buff, size_t n, file_t *f)
{
    size_t i;
    for (i = 0; i < n; i++)
    {
        if (i != 0 && i % BUFF_SIZE == 0)
            blockCurrent(f->id, FD_WRITE);
        f->write[((*(f->idxWrite))++) % BUFF_SIZE] = buff[i];
    }
    unblockProcess(f->id, FD_READ);
    return;
}

void flushFD(int fd)
{
    if (readyHeader.current->data.fds[fd] != NULL)
    {
        while (*readyHeader.current->data.fds[fd]->idxRead < ((*readyHeader.current->data.fds[fd]->idxWrite) - 1))
            (*readyHeader.current->data.fds[fd]->idxRead)++;
    }
}