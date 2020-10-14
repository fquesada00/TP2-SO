#include <stdint.h>
#include <stddef.h>
#include "include/standardstring.h"
#include "include/semaphores.h"

sem_t semaphores[MAX_SEM] = {0};
int sem_open(const char *name, size_t value, char created)
{
    int i = 0;
    if (created)
    {
        while (i < MAX_SEM && strcmp(name, semaphores[i].name) != 0)
        {
            i++;
        }
        if (i == MAX_SEM)
        {
            return -1;
        }
        return 1;
    }
    while (i < MAX_SEM && strcmp(name, "") != 0)
    {
        i++;
    }
    if (i == MAX_SEM)
    {
        return -1;
    }
    sem_t s;
    strcpy(s.name, name);
    s.value = value;
    semaphores[i] = s;
    return 1;
}
int sem_post(const char *name)
{
    //usar spinlock con xchg xq lo piden, aunque no sea necesario
    int i = 0;
    while (i < MAX_SEM && strcmp(name, semaphores[i].name) != 0)
    {
        i++;
    }
    if (i == MAX_SEM)
    {
        return -1;
    }
    semaphores[i].value++;
    //unblock one process blocked by this semaphore
    //cerrar spinlock
    return 1;
}
int sem_wait(const char *name)
{
    //idem sem_post
    int i = 0;
    while (i < MAX_SEM && strcmp(name, semaphores[i].name) != 0)
    {
        i++;
    }
    if (i == MAX_SEM)
    {
        return -1;
    }
    if(semaphores[i].value > 0){
        semaphores[i].value--;
    }
    else{
        //desbloquear spinlock
        //bloquear proceso y agregar a la lista de bloqueados por este semaphore
        //bloquear spinlock
        semaphores[i].value--;
    }
    //desbloquear spinlock
    return 1;
}
