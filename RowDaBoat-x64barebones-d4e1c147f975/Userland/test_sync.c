#include <stdint.h>
#include "standardlib.h"
#define TOTAL_PAIR_PROCESSES 4
#define SEM_ID "sem"

int64_t globalVar; //shared memory
extern int _exit(int d);
extern void yield();//VER YIELD

void slowInc(int64_t *p, int64_t inc)
{
    int64_t aux = *p;
    aux += inc;
    yield();
    *p = aux;
}

void incWrapper1(){
    inc(1,1,500);
    _exit(0);

}
void incWrapper2(){
    inc(1,-1,500);
    _exit(0);

}


void inc(int sem, int value, int N)
{
    uint64_t i;
    if (sem && syscallSemOpen(SEM_ID, 1, 0))
    {
        printf("ERROR OPENING SEM\n");
        _exit(1);
    }

    for (i = 0; i < N; i++)
    {
        if (sem)
            syscallSemWait(SEM_ID);
        slowInc(&globalVar, value);
        if (sem)
            syscallSemPost(SEM_ID);
    }

    if (sem)
        syscallSemClose(SEM_ID);

    printf("\n========Final value: %d========\n", globalVar);
}

void test_sync()
{
    uint64_t i;

    globalVar = 0;

    printf("CREATING PROCESSES...(WITH SEM)\n");
    char *argv1[] = {"incWrapper1",NULL};
    char *argv2[] = {"incWrapper2",NULL};
    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
    {
        execv(incWrapper1, 1, argv1);
        execv(incWrapper2, 1, argv2);
    }
    return;
}

void test_no_sync()
{
    uint64_t i;

    globalVar = 0;

    printf("CREATING PROCESSES...(WITHOUT SEM)\n");
    char *argv1[] = {"incWrapper1",NULL};
    char *argv2[] = {"incWrapper2",NULL};
    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
    {
        execv(incWrapper1, 1, argv1);
        execv(incWrapper2, 1, argv2);
    }
    return;
}
