#include <stdint.h>
#include <stddef.h>
#include "include/standardstring.h"
#include "include/semaphores.h"
#include "include/scheduler.h"
#include "include/list.h"
extern Header readyHeader;
extern size_t _xchg(uint64_t pointer, size_t value);
sem_t semaphores[MAX_SEM] = {0};
int addPID(sem_t *sem, size_t pid)
{
    for (int i = sem->blockedPID[MAX_BLOCKED_PID], j = 0; j < MAX_BLOCKED_PID; i++, j++)
    {
        if (i == MAX_BLOCKED_PID)
        {
            i = 0;
        }
        if (sem->blockedPID[i] == 0)
        {
            sem->blockedPID[i] = pid;
            return 1;
        }
    }
    return -1;
}

size_t removePID(sem_t *sem)
{
    if (sem->blockedPID[sem->blockedPID[MAX_BLOCKED_PID]] == 0)
        return 0;
    size_t auxPID = sem->blockedPID[sem->blockedPID[MAX_BLOCKED_PID]];
    sem->blockedPID[sem->blockedPID[MAX_BLOCKED_PID]] = 0;

    if (sem->blockedPID[MAX_BLOCKED_PID] == (MAX_BLOCKED_PID - 1))
        sem->blockedPID[MAX_BLOCKED_PID] = 0;
    else
        sem->blockedPID[MAX_BLOCKED_PID]++;
    return auxPID;
}

int sem_open(const char *name, size_t value, char created)
{
    int flag = 0, idx = 0, i = 0;
    while (i < MAX_SEM)
    {
        if (strcmp(name, semaphores[i].name) == 0)
        {
            if (created)
            {
                semaphores[i].value = value;
                return 1;
            }
            return -1;
        }
        if (!flag && strcmp(semaphores[i].name, "") == 0)
        {
            idx = i;
            flag = 1;
        }
        i++;
    }
    if (!flag)
    {
        return -1;
    }
    sem_t s;
    strcpy(s.name, name);
    s.value = value;
    s.lock = 1;
    semaphores[idx] = s;
    return 1;
}
int sem_post(const char *name)
{
    int i = 0;
    while (i < MAX_SEM && strcmp(name, semaphores[i].name) != 0)
    {
        i++;
    }
    if (i == MAX_SEM)
    {
        return -1;
    }
    acquire(&semaphores[i]);
    if (strcmp(semaphores[i].name, "") == 0)
    {
        return -1;
    }
    semaphores[i].value++;
    size_t pid = removePID(&semaphores[i]);
    if (pid == 0)
    {
        return -1;
    }
    //unblock(pid)
    release(&semaphores[i]);
    return 1;
}
int sem_wait(const char *name)
{
    int i = 0;
    while (i < MAX_SEM && strcmp(name, semaphores[i].name) != 0)
    {
        i++;
    }
    if (i == MAX_SEM)
    {
        return -1;
    }
    acquire(&semaphores[i]);
    if (strcmp(semaphores[i].name, "") == 0)
    {
        return -1;
    }
    if (semaphores[i].value > 0)
    {
        semaphores[i].value--;
    }
    else
    {
        addPID(&semaphores[i], readyHeader.current->data.PID);
        release(&semaphores[i]);
        //block current process
        //desbloqueo spinlock -> bloqueo spinlock -> addPid -> bloqueoProceso->desbloque spinlock -> bloqueo spinlock -> resto value (PROBLEMA: el unblock desbloquea ints, entonces puede haber inanicion xq no hice release)
        acquire(&semaphores[i]);
        semaphores[i].value--;
    }
    release(&semaphores[i]);
    return 1;
}
int sem_close(const char *name)
{
    int i = 0;
    while (i < MAX_SEM && strcmp(semaphores[i].name, name) != 0)
    {
        i++;
    }
    if (i == MAX_SEM)
    {
        return -1;
    }
    strcpy(semaphores[i].name, "");
    return 1;
}
void acquire(sem_t *sem)
{
    while (_xchg((uint64_t) & (sem->lock), 0) != 1)
        ;
}
void release(sem_t *sem)
{
    _xchg((uint64_t) & (sem->lock), 1);
}
