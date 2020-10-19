#include <stdint.h>
#include "semLib.h"
#include "stdioLib.h"
#include "processLib.h"
#define TOTAL_PAIR_PROCESSES 4
#define SEM_ID "sem"

int64_t globalVar; //shared memory


void slowInc(int64_t *p, int64_t inc)
{
    int64_t aux = *p;
    aux += inc;
    _yield();
    *p = aux;
}

void incWrapper1(){
    inc(1,1,500);
    _exit();

}
void incWrapper2(){
    inc(1,-1,500);
    _exit();

}


void inc(int sem, int value, int N)
{
    uint64_t i;
    if (sem && semOpen(SEM_ID, 1, 0))
    {
        printf("ERROR OPENING SEM\n");
        _exit();
    }

    for (i = 0; i < N; i++)
    {
        if (sem)
            semWait(SEM_ID);
        slowInc(&globalVar, value);
        if (sem)
            semPost(SEM_ID);
    }

    if (sem)
        semClose(SEM_ID);

    printf("\n========Final value: %d========\n", globalVar);
}

void test_sync(int argc, char * argv[])
{
    uint64_t i;

    globalVar = 0;

    printf("CREATING PROCESSES...(WITH SEM)\n");
    char *argv1[] = {"incWrapper1",NULL};
    char *argv2[] = {"incWrapper2",NULL};
    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
    {
        _execv(incWrapper1, 1, argv1,0);
        _execv(incWrapper2, 1, argv2,0);
    }
    return;
}

void test_no_sync(int argc, char * argv[])
{
    uint64_t i;

    globalVar = 0;

    printf("CREATING PROCESSES...(WITHOUT SEM)\n");
    char *argv1[] = {"incWrapper1",NULL};
    char *argv2[] = {"incWrapper2",NULL};
    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
    {
        _execv(incWrapper1, 1, argv1,0);
        _execv(incWrapper2, 1, argv2,0);
    }
    return;
}
