#include "../include/fds.h"
#include "../include/list.h"

FILE_DESCRIPTOR *fds[TOTAL_FDS] = {0};
extern Header readyHeader;
extern Header blockedHeader;

int initFd(int fd)
{
    fds[fd] = (FILE_DESCRIPTOR *)pMalloc(sizeof(FILE_DESCRIPTOR));
    if (fds[fd] == NULL)
        return -1;
    fds[fd]->pidQty = 0;
    fds[fd]->idxRead = 0;
    fds[fd]->idxWrite = 0;
    for (int i = 0; i < PROCESSES; i++)
    {
        fds[fd]->blockedPids[i] = 0;
        fds[fd]->pids[i] = 0;
    }
    fds[fd]->blockedPids[PROCESSES] = 0;
    return 1;
}

int addPid(int fd, size_t pid)
{
    for (int i = 0; i < PROCESSES; i++)
    {
        if (fds[fd]->pids[i] == -1)
        {
            fds[fd]->pids[i] = pid;
            fds[fd]->pidQty++;
            return 1;
        }
    }
    return -1;
}

int removePid(int fd, size_t pid)
{
    for (int i = 0; i < PROCESSES; i++)
    {
        if (fds[fd]->pids[i] == pid)
        {
            fds[fd]->pids[i] = -1;
            fds[fd]->pidQty--;
            return 1;
        }
    }
    return -1;
}

int insertBlockPid(int fd, size_t pid)
{
    size_t *buffer = fds[fd]->blockedPids;
    for (int i = 0; i < PROCESSES; i++)
    {
        if (buffer[i] == 0)
        {
            buffer[i] = pid;
            return 1;
        }
    }
    return -1;
}

size_t removeBlockPid(int fd)
{
    size_t *buffer = fds[fd]->blockedPids;
    if (buffer[PROCESSES] == (PROCESSES - 1))
        buffer[PROCESSES] = 0;
    size_t circularBegin = buffer[PROCESSES]++;
    size_t PID = buffer[circularBegin];
    buffer[circularBegin] = 0;
    return PID;
}


int close(int fd)
{
}

void add(int fd)
{
}

int retrieveFds(size_t pid, int *read, int *write)
{
    *read = fds[0];
    *write = fds[1];
}
/*
int retrieveFds(size_t pid, int *read, int *write)
{
    int doneRead = 0;
    for (int i = 0, j = 0; i < TOTAL_FDS; i++)
    {
        if (i != 0 && i != 1 && fds[i] == NULL)
        {
            if (!doneRead)
            {
                fds[i] = pMalloc(sizeof(FILE_DESCRIPTOR));
                *read = i;
                doneRead = 1;
            }
            else
            {
                fds[i] = pMalloc(sizeof(FILE_DESCRIPTOR));
                *write = i;
                return 1;
            }
        }
    }
    return -1;
}
*/