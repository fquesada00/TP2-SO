#ifndef _SEMAPHORES_H_
#define _SEMAPHORES_H
#define MAX_SEM 10
#include <stddef.h>
typedef struct sem
{
    int value;
    char name[256];
} sem_t;
int sem_open(const char *name, size_t value, char created);
int sem_post(const char *name);
int sem_wait(const char *name);

#endif