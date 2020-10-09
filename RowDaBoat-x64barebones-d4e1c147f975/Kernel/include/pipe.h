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
int init_PCBwithPipe(uint64_t rsp, int pid, char * name,int fd,pipe_t pipe,int mode);
#endif
