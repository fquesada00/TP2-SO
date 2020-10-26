#include "standardlib.h"
#include "stringLib.h"
#include <stddef.h>
#include "shell.h"
#include "programs.h"
#include "stdioLib.h"
#include "pipeLib.h"
#include "processLib.h"
#include "stringLib.h"
#include "test_util.h"
#include "memoryLib.h"
char *cmdName[] = {"sh","myPid","ps", "loop", "help", "sem", "cat", "wc", "filter", "pipe", "phylo", "leo", "escribo", "test_mm", "test_prio", "test_processes", "test_no_sync", "test_sync", "mem", "kill", "nice", "block"};
void *cmd[] = {shell,myPid,ps, loop, help, sem, cat, wc, filter, pipePrint, philosopherTable, leo, escribo, test_mm, test_prio, test_processes, test_no_sync, test_sync, mem, kill, nice, block};
int getCmd(char *cmd)
{
    for (int i = 0; i < CANT_CMDS; i++)
    {
        if (strcmp(cmd, cmdName[i]) == 0)
            return i;
    }
    return -1;
}
typedef enum ShellCmd
{
    SH,
    MYPID,
    PS,
    LOOP,
    HELP,
    SEM,
    CAT,
    WC,
    FILTER,
    PIPE,
    PHYLO,
    LEO,
    ESCRIBO,
    TEST_MM,
    TEST_PRIO,
    TEST_PROCESSES,
    TEST_NO_SYNC,
    TEST_SYNC,
    MEM,
    KILL,
    NICE,
    BLOCK

} ShellCmd;
//agregar testeos
void shell(int argc, char * argv[])
{
    printf("\nInitializing shell\n");
    printf("\nType 'help' to see the shell commands list\n");
    char FirstCommand[256] = {0};
    char SecondCommand[256] = {0};
    char ThirdCommand[256] = {0};
    char ForthCommand[256] = {0};
    int argsRead;
    int cmdIdx1, cmdIdx3;
    int waiting;
    int wait = 1;
    int foreground = 1;
    int pipe[2];
    int actualPipe = 0;
    uint64_t memory;
    int pid, p, blockFlag;
    char *argv1[MAX_ARGS];
    argv1[0] =(char *) pMalloc(MAXLENGHT * sizeof(char));
    argv1[1] =(char *) pMalloc(MAXLENGHT * sizeof(char));
    argv1[12] =(char *) pMalloc(MAXLENGHT * sizeof(char));
    char *user =(char *) pMalloc(MAXLENGHT * sizeof(char));
    char pipeName[256]={0};
    strcpy(pipeName,"MyPipe");
    printf("\nBienvenido! Ingrese un nombre de usuario:\n\n");
    scanf("%s", user);

    do
    {
        printf(" /%s:~$ ", user);
        argsRead = scanf("%s %s %s %s", FirstCommand, SecondCommand, ThirdCommand, ForthCommand);
        putchar('\n');
        cmdIdx1 = getCmd(FirstCommand);
        if (cmdIdx1 != -1)
        {
            if (isBuiltIn(cmdIdx1))
            {
                switch (cmdIdx1)
                {
                case PS:
                    ps();
                    break;
                case HELP:
                    help();
                    break;
                case SEM:
                    sem();
                    break;
                case PIPE:
                    pipePrint();
                    break;
                case MYPID:
                    myPid();
                    break;
                case MEM:
                    memory = myAtoi(SecondCommand);
                    if (memory < 0)
                        break;
                    mem(memory);
                    break;
                case KILL:
                    pid = myAtoi(SecondCommand);
                    if (pid <= 0)
                        break;
                    kill(pid);
                    break;
                case NICE:
                    pid = myAtoi(SecondCommand);
                    printf("pid es %d", pid);
                    p = myAtoi(ThirdCommand);
                    printf("p es %d", p);

                    if (pid <= 0 || p < 0 || p > MAXPRIVILAGE)
                        break;
                    nice(pid, p);
                    break;
                case BLOCK:
                    pid = myAtoi(SecondCommand);
                    blockFlag = myAtoi(ThirdCommand);
                    block(pid, blockFlag);
                    break;
                }
            }
            else
            {
                switch (argsRead)
                {
                case 1:
                    printf("%s",FirstCommand);
                    strcpy(argv1[0], FirstCommand);
                    argv1[argsRead] = NULL;
                    waiting = _execv(cmd[cmdIdx1], argsRead, argv1, foreground);
                    _waitPID(waiting);
                    break;
                case 2:

                    strcpy(argv1[0], FirstCommand);

                    if (strcmp(SecondCommand, "&") != 0)
                    {
        
                        strcpy(argv1[1], SecondCommand);
                    }
                    else
                    {
                        foreground = 0;
                        wait = 0;
                        argsRead = 1;
                    }

                    argv1[argsRead] = NULL;
                    waiting = _execv(cmd[cmdIdx1], argsRead, argv1, foreground);
                    if (wait)
                        _waitPID(waiting);
                    break;
                case 4:
                    if(strcmp(ForthCommand, "&") == 0)
                        foreground = 0;
                case 3:
                    cmdIdx1 = getCmd(FirstCommand);
                    if (cmdIdx1 != -1)
                    {
                        if (strcmp(SecondCommand, "|") == 0)
                        {
                            cmdIdx3 = getCmd(ThirdCommand);
                            if (cmdIdx3 != -1)
                            {
                                char pipeNum[256] = {0};
                                uintToBase(actualPipe++,pipeNum,10);
                                strcat(pipeName,pipeNum);
                                pipeOpen(pipe, pipeName);
                                strcpy(argv1[0], FirstCommand);
                                argv1[1] = NULL;
                                waiting = initProcessWithPipe(cmd[cmdIdx1], ARGSPIPE, argv1, STDOUT_FILENO,
                                                              pipeName, W, foreground);
                                strcpy(argv1[0], ThirdCommand);
                                argv1[1] = NULL;
                                initProcessWithPipe(cmd[cmdIdx3], ARGSPIPE, argv1, STDIN_FILENO,
                                                    pipeName, R, foreground);
                                if(foreground)
                                    _waitPID(waiting);
                                pipeClose(pipe, pipeName);
                                strcpy(pipeName,"myPipe");
                            }
                        }
                        else
                        {
                            strcpy(argv1[0], FirstCommand);
                            strcpy(argv1[1], SecondCommand);
                            strcpy(argv1[2], ThirdCommand);
                            argv1[3] = NULL;
                            waiting = _execv(cmd[cmdIdx1], argsRead, argv1, foreground);
                        }
                    }
                    break;

                default:
                    break;
                }
            }
        }
        else
            printf("\n%s no es un comando valido o la cantidad de argumentos no es valida\n", FirstCommand);
    } while (1);
}

int isBuiltIn(int idx)
{
    return idx == PS ||
           idx == HELP ||
           idx == SEM ||
           idx == PIPE ||
           idx == MEM ||
           idx == KILL ||
           idx == NICE ||
           idx == BLOCK ||
           idx == MYPID;
}