#ifndef _FDS_H_
#define _FDS_H_
#include <stdint.h>
#include <stddef.h>
#define TOTAL_FDS 512
#define PROCESSES 50
#define BUFFER_SIZE 256

typedef struct{
    size_t pids[PROCESSES];
    size_t blockedPids[PROCESSES];
    size_t idxWrite;
    size_t idxRead;
    size_t pidQty;
    char buffer[BUFFER_SIZE];
}FILE_DESCRIPTOR;

int initFd(int fd);
void insertAndBlockPid(int fd);
void removeAndUnblockPid(int fd);
int close(int fd);
void add(int fd);
void retrieveFds(size_t pid, int * read, int * write);

#endif