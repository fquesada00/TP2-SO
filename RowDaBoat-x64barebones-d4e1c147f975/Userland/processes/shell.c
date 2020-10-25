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
char *cmdName[] = {"ps", "loop", "help", "sem", "cat", "wc", "filter", "pipe", "phylo", "leo", "escribo", "test_mm", "test_prio", "test_processes", "test_no_sync", "test_sync", "mem", "kill", "nice", "block"};
void *cmd[] = {ps, loop, help, sem, cat, wc, filter, pipe, phylo, leo, escribo, test_mm, test_prio, test_processes, test_no_sync, test_sync, mem, kill, nice, block};
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
void shell()
{
    printf("\nInitializing shell\n");
    printf("\nType 'help' to see the shell commands list\n");

    char FirstCommand[256] = {0};
    char SecondCommand[256] = {0};
    char ThirdCommand[256] = {0};
    int argsRead;
    int cmdIdx1, cmdIdx3;
    int waiting;
    int wait = 1;
    int foreground = 1;
    int pipe[2];
    uint64_t memory;
    int pid, p, blockFlag;
    char *argv[MAX_ARGS];
    argv[0] =(char *) pMalloc(MAXLENGHT * sizeof(char));
    argv[1] =(char *) pMalloc(MAXLENGHT * sizeof(char));
    argv[2] =(char *) pMalloc(MAXLENGHT * sizeof(char));
    char *user =(char *) pMalloc(MAXLENGHT * sizeof(char));
    printf("\nBienvenido! Ingrese un nombre de usuario:\n\n");
    scanf("%s", user);

    do
    {
        printf(" /%s:~$ ", user);
        argsRead = scanf("%s %s %s", FirstCommand, SecondCommand, ThirdCommand);
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
                    strcpy(argv[0], FirstCommand);
                    argv[argsRead] = NULL;
                    waiting = _execv(cmd[cmdIdx1], argsRead, argv, foreground);
                    _waitPID(waiting);
                    break;
                case 2:

                    strcpy(argv[0], FirstCommand);

                    if (strcmp(SecondCommand, "&") != 0)
                    {
                        strcpy(argv[1], SecondCommand);
                    }
                    else
                    {
                        foreground = 0;
                        wait = 0;
                        argsRead = 1;
                    }

                    argv[argsRead] = NULL;
                    waiting = _execv(cmd[cmdIdx1], argsRead, argv, foreground);
                    if (wait)
                        _waitPID(waiting);
                    break;
                case 3:
                    cmdIdx1 = getCmd(FirstCommand);
                    if (cmdIdx1 != -1)
                    {
                        if (strcmp(SecondCommand, "|") == 0)
                        {
                            cmdIdx3 = getCmd(ThirdCommand);
                            if (cmdIdx3 != -1)
                            {
                                pipeOpen(pipe, "MyPipe");
                                strcpy(argv[0], FirstCommand);
                                argv[1] = NULL;
                                waiting = initProcessWithPipe(cmd[cmdIdx1], ARGSPIPE, argv, STDOUT_FILENO,
                                                              "MyPipe", W, foreground);
                                strcpy(argv[0], ThirdCommand);
                                argv[1] = NULL;
                                initProcessWithPipe(cmd[cmdIdx3], ARGSPIPE, argv, STDIN_FILENO,
                                                    "MyPipe", R, foreground);
                                _waitPID(waiting);
                                pipeClose(pipe, "MyPipe");
                            }
                        }
                        else
                        {
                            strcpy(argv[0], FirstCommand);
                            strcpy(argv[1], SecondCommand);
                            strcpy(argv[2], ThirdCommand);
                            argv[3] = NULL;
                            waiting = _execv(cmd[cmdIdx1], argsRead, argv, foreground);
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
           idx == BLOCK;
}