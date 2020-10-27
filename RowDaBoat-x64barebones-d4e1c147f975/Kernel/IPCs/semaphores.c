// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "semaphores.h"
#include "ctes.h"
#include "list.h"
#include "interrupts.h"
#include "standardstring.h"
#include "arrayCircular.h"
#include "arrayPCBOrdenN.h"
#include "scheduler.h"
#include "standardLib.h"
#include "syscalls.h"
#include <stdint.h>
extern size_t _xchg(uint64_t pointer, size_t value);
extern Header readyHeader;
sem_t semaphores[MAX_SEM] = {{0}};
void acquire(sem_t *sem)
{
    while (_xchg((uint64_t)&(sem->lock), 0) != 1)
        _int20();
}
void release(sem_t *sem)
{
    _xchg((uint64_t)&(sem->lock), 1);
}
int sem_open(const char *name, size_t value, char created)
{
    if (created)
    {
        for (int i = 0; i < MAX_SEM; i++)
        {
            if (strcmp(semaphores[i].name, name) == 0)
            {
                acquire(&semaphores[i]);
                if (isPresent(semaphores[i].idxOpenedPID, MAX_BLOCKED_PID, &readyHeader.current->data, semaphores[i].openPID))
                {
                    release(&semaphores[i]);
                    return 1;
                }
                add(MAX_BLOCKED_PID, &readyHeader.current->data, semaphores[i].openPID);
                semaphores[i].value = value;
                release(&semaphores[i]);
                return 0;
            }
        }
        return 1;
    }
    int i;
    for (i = 0; i < MAX_SEM; i++)
    {
        if (strcmp(semaphores[i].name, "") == 0)
        {
            break;
        }
        else if (strcmp(semaphores[i].name, name) == 0)
        {
            add(MAX_BLOCKED_PID, &readyHeader.current->data, semaphores[i].openPID);
            return 0;
        }
    }
    if (i == MAX_SEM)
        return 1;
    sem_t s = {0};
    s.lock = 1;
    strcpy(s.name, name);
    s.value = value;
    s.idxBlockedPID = 0;
    s.idxOpenedPID = 0;
    semaphores[i] = s;
    add(MAX_BLOCKED_PID, &readyHeader.current->data, semaphores[i].openPID);
    return 0;
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
        return 1;
    }
    acquire(&semaphores[i]);
    if (strcmp(semaphores[i].name, "") == 0)
    {
        release(&semaphores[i]);
        return 1;
    }
    semaphores[i].value++;
    PCB *pcb = peekPCB(semaphores[i].idxBlockedPID, semaphores[i].blockedPID);
    if (pcb == NULL)
    {
        release(&semaphores[i]);
        return 0;
    }
    unblockProcessByPCB(pcb);
    release(&semaphores[i]);
    return 0;
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
        return 1;
    }
    acquire(&semaphores[i]);
    if (strcmp(semaphores[i].name, "") == 0)
    {
        release(&semaphores[i]);
        return 1;
    }
    if (semaphores[i].value > 0)
    {
        PCB *now = peekPCB(semaphores[i].idxBlockedPID, semaphores[i].blockedPID);
        if (now == NULL)
        {
            semaphores[i].value--;
            release(&semaphores[i]);
            return 0;
        }
        else
        {
            unblockProcessByPCB(now);
            addPCB(semaphores[i].idxBlockedPID, MAX_BLOCKED_PID, &readyHeader.current->data, semaphores[i].blockedPID);
            release(&semaphores[i]);
            blockCurrent(i, SEM);
        }
    }
    else
    {

        addPCB(semaphores[i].idxBlockedPID, MAX_BLOCKED_PID, &readyHeader.current->data, semaphores[i].blockedPID);
        release(&semaphores[i]);
        blockCurrent(i, SEM);
    }
    removePCB(&semaphores[i].idxBlockedPID, MAX_BLOCKED_PID, semaphores[i].blockedPID);
    acquire(&semaphores[i]);
    semaphores[i].value--;
    release(&semaphores[i]);
    return 0;
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
        return 1;
    }
    acquire(&semaphores[i]);
    if (strcmp(semaphores[i].name, "") == 0)
    {
        release(&semaphores[i]);
        return 1;
    }
    remove(MAX_BLOCKED_PID, &readyHeader.current->data, semaphores[i].openPID);
    if (isEmptyArray(MAX_BLOCKED_PID, semaphores[i].openPID))
        semaphores[i].name[0] = 0;
    release(&semaphores[i]);
    return 0;
}

void sem()
{
    int i = 0, auxLength = 0;
    char titles[512] = {0};
    char auxBuff[256] = {0};
    while (i < MAX_SEM)
    {
        if (strcmp(semaphores[i].name, "") != 0)
        {
            strcpy(titles, "NAME = ");
            auxLength = strcat(titles, semaphores[i].name);
            syscall_write(1, titles,auxLength);
            syscall_write(1," ; ",3);
            strcpy(titles, "VALUE = ");
            uintToBase(semaphores[i].value, auxBuff, 10);
            auxLength = strcat(titles, auxBuff);
            syscall_write(1, titles, auxLength);
            syscall_write(1," ; ",3);
            auxLength = strcpy(titles, "OPENED PIDs = ");
            syscall_write(1, titles, auxLength);
            printArray(MAX_BLOCKED_PID, semaphores[i].openPID);
            syscall_write(1," ; ",3);
            auxLength = strcpy(titles, "BLOCKED PIDs = ");
            syscall_write(1, titles, auxLength);
            printArray(MAX_BLOCKED_PID, semaphores[i].blockedPID);
            syscall_write(1,"\n",1);
        }
        i++;
    }
    syscall_write(1,"\n",1);
}