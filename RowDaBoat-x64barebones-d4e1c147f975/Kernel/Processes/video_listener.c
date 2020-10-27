// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "video_listener.h"
#include "fds.h"
#include "video_driver.h"
#include "scheduler.h"
#include "interrupts.h"
#include "syscalls.h"
extern file_t *stdout;
void video_listener()
{
    _cli();
    char buff[BUFF_SIZE + 1];
    int n, aux;
    while (1)
    {
        aux = *(stdout->idxWrite) - *(stdout->idxRead);
        if (aux != 0)
        {
            n = syscall_read(1, buff, aux);
            buff[n] = '\0';
            puts(buff);
        }
        else
        {
            blockCurrent(stdout->id, FD_READ);
        }
    }
}
