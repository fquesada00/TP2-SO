#include "pipe.h"
#include <stdint.h>
#include "fds.h"
#include "scheduler.h"
#include "list.h"
#include "ctes.h"
#include "arrayCircular.h"
#include "arrayPCBOrdenN.h"
#include "standardstring.h"
#include "memory_manager.h"
#include "buddy_memory_manager.h"
#include "syscalls.h"
#include "standardLib.h"
typedef PCB elem_t;

extern Header readyHeader;
extern uint64_t stackSize;
extern int currentPIDs;
extern file_t *stdin;
extern file_t *stdout;
extern int idFds;
pipe_t pipes[MAX_PIPE] = {{0}};

int pipeOpen(int fds[2], const char *name)
{
    int i = 0, isOpen = 0, pos = -1;
    while (i < MAX_PIPE)
    {
        if (pos == -1 && strcmp(pipes[i].name, "") == 0)
        {
            pos = i;
        }
        if (strcmp(pipes[i].name, name) == 0)
        {
            isOpen = 1;
            break;
        }
        i++;
    }
    if (i == MAX_PIPE || isOpen)
        return -1;
    pipe_t p = {0};
    pipes[pos] = p;
    pipes[pos].buff = (char *)pMalloc(BUFF_SIZE);
    strcpy(pipes[pos].name, name);
    int j = 0;
    while (j < MAX_FDS && readyHeader.current->data.fds[j] != NULL)
    {
        j++;
    }
    if (j == MAX_FDS)
        return -1;
    fds[0] = j;
    file_t *fd1 = (file_t *)pMalloc(sizeof(file_t));
    fd1->read = pipes[pos].buff;
    fd1->reading = 1;
    fd1->write = NULL;
    fd1->writing = 0;
    fd1->idxWrite = &pipes[pos].idxW;
    fd1->idxRead = &pipes[pos].idxR;
    fd1->type = PIPE;
    fd1->id = idFds;
    readyHeader.current->data.fds[j] = fd1;
    while (j < MAX_FDS && readyHeader.current->data.fds[j] != NULL)
    {
        j++;
    }
    if (j == MAX_FDS)
        return -1;
    fds[1] = j;
    file_t *fd2 = (file_t *)pMalloc(sizeof(file_t));
    fd2->write = pipes[pos].buff;
    fd2->writing = 1;
    fd2->read = NULL;
    fd2->reading = 0;
    fd2->idxWrite = &pipes[pos].idxW;
    fd2->idxRead = &pipes[pos].idxR;
    fd2->type = PIPE;
    fd2->id = idFds++;
    readyHeader.current->data.fds[j] = fd2;
    pipes[pos].fd[0] = fd1;
    pipes[pos].fd[1] = fd2;
    add(MAX_BLOCKED_PID, &readyHeader.current->data, pipes[pos].openedPID);
    return 0;
}
int init_process_with_pipe(void *entry, int argc, char *argv[], int fd, const char *pipe, int mode, int fg) //mode en r9
{
    void *toFree = pMalloc(stackSize * sizeof(uint64_t));
    void *rsp = toFree;
    if ((size_t *)rsp != NULL)
    {
        rsp += stackSize * sizeof(uint64_t) - (sizeof(uint64_t));
        int pid = currentPIDs++;
        init_registers(entry, argc, argv, rsp);
        int i = 0;
        for (; i < MAX_PIPE; i++)
        {
            if (strcmp(pipes[i].name, pipe) == 0)
            {
                break;
            }
        }
        if (i == MAX_PIPE)
            return -1;
        init_PCBwithPipe(rsp, pid, argv[0], fd, &pipes[i], mode, fg, toFree);
        return pid;
    }
    return -1;
}
int init_PCBwithPipe(void *rsp, int pid, const char *name, int fd, pipe_t *pipe, int mode, int fg, void *toFree)
{
    if (fd > MAX_FDS || fd < 0)
        return -1;
    elem_t e = {{0}, 0, {0}, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    e.PID = pid;
    e.rsp = (uint64_t)rsp - sizeof(Swapping);
    e.StackBase = (uint64_t)rsp;
    e.privilege = 5;
    e.fg = fg;
    e.toFree = toFree;
    if (fg)
    {
        e.fds[0] = stdin;
        stdin->reading++;
    }
    e.fds[1] = stdout;
    stdout->writing++;
    e.BlockID = -1;
    e.reason = NOTHING;
    if (e.fds[fd] != NULL)
    {
        if (mode)
            e.fds[fd]->writing--;
        else if (fg)
            e.fds[fd]->reading--;
    }
    e.fds[fd] = pipe->fd[mode];
    e.fds[fd]->idxRead = &pipe->idxR;
    e.fds[fd]->idxWrite = &pipe->idxW;
    if (mode)
        e.fds[fd]->writing++;
    else
        e.fds[fd]->reading++;
    e.state = Ready;
    readyHeader.ready++;
    strcpy(e.name, name);
    push(&readyHeader, e, 5, 5);
    PCB *insert = getPCB(e.PID);
    add(MAX_BLOCKED_PID, insert, pipe->openedPID);
}

void pipeCloseFd(int fdNum, pipe_t *pipe)
{
    if (pipe->fd[fdNum] != NULL)
    {
        if (fdNum && !(--(pipe->fd[fdNum]->writing)))
        {
            pFree(pipe->fd[fdNum]);
        }
        else if (!fdNum && !(--(pipe->fd[fdNum]->reading)))
        {
            pFree(pipe->fd[fdNum]);
        }
    }
    if (pipe->fd[0]->reading == 0 && pipe->fd[1]->writing == 0)
    {
        pipe_t p = {0};
        *pipe = p;
    }
}

int pipeClose(int fds[2], const char *name)
{
    int i = 0;
    for (; i < MAX_PIPE; i++)
    {
        if (strcmp(pipes[i].name, name) == 0)
        {
            break;
        }
    }
    if (i == MAX_PIPE)
    {
        return 0;
    }
    pipeCloseFd(0, &pipes[i]);
    pipeCloseFd(1, &pipes[i]);
    remove(MAX_BLOCKED_PID, &readyHeader.current->data, pipes[i].openedPID);
    if (pipes[i].fd[1] == NULL && pipes[i].fd[0] == NULL && isEmptyArray(MAX_BLOCKED_PID, pipes[i].openedPID))
    {
        strcpy(pipes[i].name, "");
        pFree(pipes[i].buff);
        pipes[i].buff = NULL;
    }
    readyHeader.current->data.fds[fds[0]] = NULL;
    readyHeader.current->data.fds[fds[1]] = NULL;
    return 1;
}

void printPipe()
{
    int i = 0, auxLength = 0;
    char titles[512] = {0};
    char auxBuff[256] = {0};
    while (i < MAX_PIPE)
    {
        if (strcmp(pipes[i].name, "") != 0)
        {
            strcpy(titles, "NAME = ");
            auxLength = strcat(titles, pipes[i].name);
            syscall_write(1, titles, auxLength);
            syscall_write(1, " ; ", 3);
            strcpy(titles, "READING = ");
            uintToBase(pipes[i].fd[0]->reading, auxBuff, 10);
            auxLength = strcat(titles, auxBuff);
            syscall_write(1, titles, auxLength);
            syscall_write(1, " ; ", 3);
            strcpy(titles, "WRITING = ");
            uintToBase(pipes[i].fd[1]->writing, auxBuff, 10);
            auxLength = strcat(titles, auxBuff);
            syscall_write(1, titles, auxLength);
            syscall_write(1, " ; ", 3);
            auxLength = strcpy(titles, "OPENED PIDs = ");
            syscall_write(1, titles, auxLength);
            printArray(MAX_BLOCKED_PID, pipes[i].openedPID);
        }
        i++;
    }
    syscall_write(1, "\n", 1);
}
void removeFromPipe(int idx, PCB *element, int fd)
{
    if (element->fds[fd] != NULL)
    {
        remove(MAX_BLOCKED_PID, element, pipes[idx].openedPID);
        pipeCloseFd(element->fds[fd]->read == NULL, &pipes[idx]);
    }
}

int pipeIdx(int id)
{
    int i;
    for (i = 0; i < MAX_PIPE; i++)
    {
        if (pipes[i].fd[0] != NULL)
        {
            if (pipes[i].fd[0]->id == id)
            {
                return i;
            }
        }
        else if (pipes[i].fd[1] != NULL)
        {
            if (pipes[i].fd[1]->id == id)
            {
                return i;
            }
        }
    }
    return -1;
}