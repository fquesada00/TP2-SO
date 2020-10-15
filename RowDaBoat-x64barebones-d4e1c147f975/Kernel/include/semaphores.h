#ifndef _SEMAPHORES_H_
#define _SEMAPHORES_H
#define MAX_SEM 10
#define MAX_BLOCKED_PID 10
#include <stddef.h>
typedef struct sem
{
    uint64_t lock;
    char name[256];
    size_t blockedPID[MAX_BLOCKED_PID+1];
    int value;
} sem_t;
int sem_open(const char *name, size_t value, char created);
int sem_post(const char *name);
int sem_wait(const char *name);
int sem_close(const char *name);
void sem();
int addPID(sem_t *sem, size_t pid);
size_t removePID(sem_t *sem);
void acquire(sem_t *sem);
void release(sem_t *sem);
#endif