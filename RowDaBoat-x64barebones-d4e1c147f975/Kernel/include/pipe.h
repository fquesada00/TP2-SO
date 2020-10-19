#ifndef _PIPE_H_
#define _PIPE_H_
#include "ctes.h"
#include "fds.h"
#include "scheduler.h"
#include <stdint.h>
typedef struct pipe{
    char *buff;
    char name[BUFF_SIZE];
    file_t * fd[2];
    PCB * blockedPID[MAX_BLOCKED_PID];
    PCB * openedPID[MAX_BLOCKED_PID];
    int idxBlocked;
    int idxR;
    int idxW;
}pipe_t;
int pipeOpen(int fd[2], const char *name);
int init_process_with_pipe(void *entry, int argc, char *argv[], int fd, const char *pipe, int mode);
int init_PCBwithPipe(uint64_t rsp, int pid, const char * name,int fd,pipe_t *pipe,int mode);
int pipeClose(int fds[2],const char *name);
void pipeCloseFd(int,pipe_t*);
void printPipe();
void removeFromPipe(int idx, PCB *element, int fd);
int pipeIdx(int id);
#endif
