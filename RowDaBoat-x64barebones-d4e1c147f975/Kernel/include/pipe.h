#ifndef PIPE_H
#define PIPE_H
#include <stdint.h>
#include "fileDescriptor.h"
#define MAX_PIPE 10
typedef struct pipe{
    char buff[BUF_SIZE];
    char name[BUF_SIZE];
    file_t * fd[2];
}pipe_t;
int pipeOpen(int fd[2], const char *name);
int init_process_with_pipe(void *entry, int argc, char *argv[], int fd, const char *pipe, int mode);
int init_PCBwithPipe(uint64_t rsp, int pid, const char * name,int fd,pipe_t pipe,int mode);
int pipeClose(int fd, const char *name);
#endif
