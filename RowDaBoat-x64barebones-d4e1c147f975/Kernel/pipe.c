#include "standardstring.h"
#include "fileDescriptor.h"
#include "pipe.h"
#include "list.h"
#include "scheduler.h"
#include "include/arrayCircular.h"
#include "include/arrayPCBOrdenN.h"

pipe_t pipes[MAX_PIPE] = {0};
extern Header readyHeader;
extern uint64_t stackSize;
extern int currentPIDs;
extern file_t stdin;
extern file_t stdout;

int pipeOpen(int fds[2], const char *name)
{
    int i = 0;
    while (i < MAX_PIPE && strcmp(pipes[i].name, "") != 0)
    {
        i++;
    }
    if (i == MAX_PIPE)
        return -1;
    pipe_t p = {0};
    strcpy(p.name, name);
    int j = 0;
    while (j < MAX_FD && readyHeader.current->data.fds[j] != NULL)
    {
        j++;
    }
    if (j == MAX_FD)
        return -1;
    fds[0] = j;
    file_t *fd1 = pMalloc(sizeof(file_t));
    fd1->read = p.buff;
    fd1->reading = 1;
    fd1->write = NULL;
    fd1->writing = 0;
    fd1->idxW = 0;
    fd1->idxR = 0;
    //fd1->type = PIPE;
    readyHeader.current->data.fds[j] = fd1;
    while (j < MAX_FD && readyHeader.current->data.fds[j] != NULL)
    {
        j++;
    }
    if (j == MAX_FD)
        return -1;
    fds[1] = j;
    file_t *fd2 = pMalloc(sizeof(file_t));
    fd2->write = p.buff;
    fd2->writing = 1;
    fd2->read = NULL;
    fd2->reading = 0;
    fd2->idxW = 0;
    fd2->idxR = 0;
    //fd2->type = PIPE;
    readyHeader.current->data.fds[j] = fd2;
    p.fd[0] = fd1;
    p.fd[1] = fd2;
    pipes[i] = p;
    return 0;
}
int init_process_with_pipe(void *entry, int argc, char *argv[], int fd, const char *pipe, int mode) //mode en r9
{
    uint64_t rsp = (uint64_t)pMalloc(stackSize * sizeof(uint64_t));
    if (rsp != NULL)
    {
        rsp += stackSize - (sizeof(uint64_t));
        int pid = currentPIDs++;
        init_registers(entry, argc, argv, rsp);
        int i = 0;
        while (i < MAX_PIPE && strcmp(pipes[i].name, pipe) != 0)
        {
            i++;
        }
        if (i == MAX_PIPE)
            return -1;
        init_PCBwithPipe(rsp, pid, argv[0], fd, pipes[i], mode);
        return pid;
    }
    return -1;
}
int init_PCBwithPipe(uint64_t rsp, int pid, const char *name, int fd, pipe_t pipe, int mode)
{
    if (fd > MAX_FD || fd < 0)
        return -1;
    PCB e = {0};
    e.PID = pid;
    e.rsp = rsp - sizeof(Swapping);
    e.StackBase = rsp;
    e.privilege = 5;
    e.fds[0] = &stdin;
    stdin.reading++;
    e.fds[1] = &stdout;
    stdout.writing++;
    e.BlockID = -1;
    e.reason = NOTHING;
    if (e.fds[fd] != NULL)
    {
        if (mode)
            e.fds[fd]->writing--;
        else
            e.fds[fd]->reading--;
    }
    e.fds[fd] = pipe.fd[mode];
    e.fds[fd]->idxR = 0;
    e.fds[fd]->idxW = 0;
    if (mode)
        e.fds[fd]->writing++;
    else
        e.fds[fd]->reading++;
    e.state = Ready;
    readyHeader.ready++;
    strcpy(e.name, name);
    add(MAX_BLOCKED_PID, &e, pipe.openedPID);
    push(&readyHeader, e, 5, 5);
}

void pipeCloseFd(int fdNum, pipe_t *pipe)
{
    if (pipe->fd[fdNum] != NULL)
    {
        if (fdNum && !(--(pipe->fd[fdNum]->writing)))
        {
            pFree(pipe->fd[fdNum]);
            pipe->fd[fdNum] = NULL;
        }
        else if (!fdNum && !(--(pipe->fd[fdNum]->reading)))
        {
            pFree(pipe->fd[fdNum]);
            pipe->fd[fdNum] = NULL;
        }
    }
}

int pipeClose(const char *name)
{
    int i = 0;
    while (i < MAX_PIPE && strcmp(pipes[i].name, name) != 0)
    {
        i++;
    }
    if (i == MAX_PIPE)
    {
        return 0;
    }
    pipeCloseFd(0,&pipes[i]);
    pipeCloseFd(1,&pipes[i]);
    remove(MAX_BLOCKED_PID, &readyHeader.current->data, pipes[i].openedPID);
    if (pipes[i].fd[1] == NULL && pipes[i].fd[0] == NULL && isEmptyArray(MAX_BLOCKED_PID, pipes[i].openedPID))
    {
        strcpy(pipes[i].name, "");
    }
    return 1;
}
void printPipes()
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
            uintToBase(pipes[i].fd[0], auxBuff, 10);
            auxLength = strcat(titles, auxBuff);
            syscall_write(1, titles, auxLength);
            syscall_write(1, " ; ", 3);
            strcpy(titles, "WRITING = ");
            uintToBase(pipes[i].fd[1], auxBuff, 10);
            auxLength = strcat(titles, auxBuff);
            syscall_write(1, titles, auxLength);
            syscall_write(1, " ; ", 3);
            auxLength = strcpy(titles, "OPENED PIDs = ");
            syscall_write(1, titles, auxLength);
            printArray(MAX_BLOCKED_PID, pipes[i].openedPID);
            syscall_write(1, " ; ", 3);
        }
        i++;
    }
    syscall_write(1, "\n", 1);
}
/*
int calculateIdx(int idx){
    return (idx * -1) - 2;
}
int pipeAdd(int idx, PCB *pcb)
{
    idx = calculateIdx(idx);
    addPCB(pipes[idx].idxBlocked, MAX_BLOCKED_PID, pcb, pipes[idx].blockedPID);
}
int pipeRemove(int idx){
    idx = calculateIdx(idx);
    removePCB(pipes[idx].idxBlocked, MAX_BLOCKED_PID, pipes[idx].blockedPID);
}*/