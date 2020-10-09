#include "standardstring.h"
#include "fileDescriptor.h"
#include "pipe.h"
#include "list.h"
#include "scheduler.h"

pipe_t pipes[MAX_PIPE] = {0};
extern Header readyHeader;
extern uint64_t stackSize;
extern int currentPIDs;
extern file_t stdin;
extern file_t stdout;

int pipe(int fds[2], char *name)
{
    int i = 0;
    while (i < MAX_PIPE && strcmp(pipes[i].name, "") != 0)
    {
        i++;
    }
    if (i == MAX_PIPE)
        return -1;
    pipe_t p;
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
    readyHeader.current->data.fds[j] = fd2;
    p.fd[0] = fd1;
    p.fd[1] = fd2;
    return 0;
}
int init_process_with_pipe(void *entry, int argc, char *argv[], int fd, char *pipe, int mode) //mode en r9
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
int init_PCBwithPipe(uint64_t rsp, int pid, char *name, int fd, pipe_t pipe, int mode)
{
    if (fd > MAX_FD || fd < 0)
        return -1;
    elem_t e = {0};
    e.PID = pid;
    e.rsp = rsp - sizeof(Swapping);
    e.StackBase = rsp;
    e.privilege = 5;
    e.fds[0] = &stdin;
    stdin.reading++;
    e.fds[1] = &stdout;
    stdout.writing++;
    e.fdBlock = -1;
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
    if(mode)
        e.fds[fd]->writing++;
    else
        e.fds[fd]->reading++;
    e.state = Ready;
    readyHeader.ready++;
    strcpy(e.name, name);
    push(&readyHeader, e, 5, 5);
}
