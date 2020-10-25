#include <stdint.h>
#include "semLib.h"
#include "stdioLib.h"
#include "processLib.h"
#include "memoryLib.h"
#include "stringLib.h"
#include "shell.h"
#include <stddef.h>
#define TOTAL_PAIR_PROCESSES 2
#define SEM_ID "sem"
#define MAXNUM 100
#define N 1000000
int64_t globalVar; //shared memory

void slowInc(int64_t *p, int64_t inc)
{
    int64_t aux = *p;
    aux += inc;
    _yield();
    *p = aux;
}

void inc(int argc, char *argv[])
{
    int sem = myAtoi(argv[1]);
    int value = myAtoi(argv[2]);
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

    printf("Final value = %d\n",globalVar);
    _exit();
}

void test_sync(int argc, char *argv[])
{
    uint64_t i;

    globalVar = 0;

    printf("CREATING PROCESSES...(WITHOUT SEM)\n");

    char *argv1[4];
    char *argv2[4];
    argv1[0] = pMalloc(strlen("inc") + 1);
    argv1[1] = pMalloc(2);
    argv1[2] = pMalloc(2);
    argv1[3] = NULL;
    strcpy(argv1[0], "inc");
    strcpy(argv1[1], "1"); //SEM
    strcpy(argv1[2],"1");  //VALUE
    argv2[0] = pMalloc(strlen("inc") + 1);
    argv2[1] = pMalloc(3);
    argv2[2] = pMalloc(2);
    argv2[3] = NULL;
    strcpy(argv2[0], "inc");
    strcpy(argv2[1], "1");
    strcpy(argv2[2],"-1");  //VALUE
    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
    {
        _execv(inc, 4, argv1, 0);
        _execv(inc, 4, argv2, 0);
    }
    _exit();
}

void test_no_sync(int argc, char *argv[])
{
    uint64_t i;

    globalVar = 0;

    printf("CREATING PROCESSES...(WITHOUT SEM)\n");

    char *argv1[5];
    char *argv2[5];
    argv1[0] = pMalloc(strlen("inc") + 1);
    argv1[1] = pMalloc(2);
    argv1[2] = pMalloc(2);
    argv1[3] = pMalloc(MAXNUM);
    argv1[4] = NULL;
    strcpy(argv1[0], "inc");
    strcpy(argv1[1], "0"); //SEM
    strcpy(argv1[2],"1");  //VALUE
    strcpy(argv1[3], argv[1]); //N
    argv2[0] = pMalloc(strlen("inc") + 1);
    argv2[1] = pMalloc(3);
    argv2[2] = pMalloc(2);
    argv2[3] = pMalloc(MAXNUM);
    argv2[4] = NULL;
    strcpy(argv2[0], "inc");
    strcpy(argv2[1], "0");
    strcpy(argv2[2],"-1");  //VALUE
    strcpy(argv2[3], argv[1]);
    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
    {
        _execv(inc, 4, argv1, 0);
        _execv(inc, 4, argv2, 0);
    }
    _exit();
}
