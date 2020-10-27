// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "fds.h"
#include "memory_manager.h"
#include "keyboardDriver.h"
#include "ctes.h"
#include "scheduler.h"
#include "list.h"
#include "syscalls.h"
#include "pipe.h"
int idFds = 0;
file_t *stdout;
file_t *stdin;
extern Header readyHeader;
void init_fds()
{
    stdin = pMalloc(sizeof(file_t));
    stdin->read = pMalloc(sizeof(char) * BUFF_SIZE);
    stdin->write = stdin->read;
    stdin->idxRead = pMalloc(sizeof(int));
    stdin->idxWrite = pMalloc(sizeof(int));
    *stdin->idxWrite = 0;
    *stdin->idxRead = 0;
    stdin->id = idFds++;
    stdin->type = STDINOUT;
    stdout = pMalloc(sizeof(file_t));
    stdout->read = pMalloc(sizeof(char) * BUFF_SIZE);
    stdout->write = stdout->read;
    stdout->idxRead = pMalloc(sizeof(int));
    stdout->idxWrite = pMalloc(sizeof(int));
    *stdout->idxRead = 0;
    *stdout->idxWrite = 0;
    stdout->id = idFds++;
    stdout->type = STDINOUT;
}

void closeCurrentProcess(int fd)
{
    PCB *pcb = &readyHeader.current->data;
    if (fd != 0 && pcb->fds[fd] != NULL && pcb->fds[fd]->write != NULL)
    {
        char buffer[2] = {0};
        buffer[0] = EOF;
        buffer[1] = '\0';
        syscall_write(fd, buffer, 1);
    }
    switch (pcb->fds[fd]->type)
    {
    case PIPE:
        removeFromPipe(pipeIdx(pcb->fds[fd]->id), pcb, fd);
        break;
    case STDINOUT:
        break;
    }
    pcb->fds[fd] = NULL;
}

void closePID(size_t pid, int fd)
{
    PCB *pcb = getPCB(pid);
    if (pcb->fds[fd] != NULL && pcb->fds[fd]->write != NULL) 
    {
        char buffer[2] = {0};
        buffer[0] = EOF;
        buffer[1] = '\0';
        syscall_write(fd, buffer, 1);
    }
    switch (pcb->fds[fd]->type)
    {
    case PIPE:
        removeFromPipe(pipeIdx(pcb->fds[fd]->id), pcb, fd);
        break;
    case STDINOUT:
        break;
    }
    pcb->fds[fd] = NULL;
}