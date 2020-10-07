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
    for (int i = 0; i < TOTAL_FDS; i++)
    {
        fds[fd]->blockedPids[i] = -1;
        fds[fd]->pids[i] = -1;
    }
    return 1;
}

int addPid(int fd, size_t pid){
    for(int i = 0 ; i < PROCESSES ; i++){
        if(fds[fd]->pids[i] == -1){
            fds[fd]->pids[i] = pid;
            fds[fd]->pidQty++;
            return 1;
        }
    }
    return -1;
}

int removePid(int fd, size_t pid){
    for(int i = 0 ; i < PROCESSES ; i++){
        if(fds[fd]->pids[i] == pid){
            fds[fd]->pids[i] = -1;
            fds[fd]->pidQty--;
            return 1;
        }
    }
    return -1;
}


void insertAndBlockPid(int fd)
{
    size_t *buffer = fds[fd]->blockedPids, pid = blockedHeader.current->data.PID;
    int i;
    for (i = 0; i < TOTAL_FDS; i++)
    {
        if (buffer[i] == -1)
        {
            buffer[i] = pid;
            blockProcess(pid, fd);
            return;
        }
    }
}

void removeAndUnblockPid(int fd)
{
    size_t *buffer = fds[fd]->blockedPids;
    int i, done = 0;
    for (i = 0; !done && i < TOTAL_FDS; i++)
    {
        if (buffer[i] != -1)
            done = 1;
    }
    listElem_t *list = blockedHeader.current;
    while (list != NULL)
    {
        if (buffer[i] == list->data.PID)
        {
            readyProcess(buffer[i]);
            buffer[i] = -1;
        }
    }
}

int close(int fd)
{
}

void add(int fd)
{
}


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