#ifndef _PIPE_H_
#define _PIPE_H_
#include <stdint.h>
#include "fileDescriptor.h"
#include "semaphores.h"
#include "scheduler.h"
#define MAX_PIPE 10
typedef struct pipe{
    char buff[BUF_SIZE];
    char name[BUF_SIZE];
    file_t * fd[2];
    PCB * blockedPID[MAX_BLOCKED_PID];
    PCB * openedPID[MAX_BLOCKED_PID];
    int idxBlocked;
}pipe_t;
int pipeOpen(int fd[2], const char *name);
int init_process_with_pipe(void *entry, int argc, char *argv[], int fd, const char *pipe, int mode);
int init_PCBwithPipe(uint64_t rsp, int pid, const char * name,int fd,pipe_t pipe,int mode);
int pipeClose(const char *name);
void pipeCloseFd(int,pipe_t*);
void printPipe();
#endif
