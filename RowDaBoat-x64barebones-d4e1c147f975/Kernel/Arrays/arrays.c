// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <stddef.h>
#include "arrays.h"
extern int syscall_write(int, const char *, int);
extern uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);
void printArray(int dim, PCB *array[])
{
    char buffer[256] = {0};
    for (int i = 0; i < dim; i++)
    {
        if (array[i] != NULL)
        {
            int length = uintToBase(array[i]->PID, buffer, 10);
            syscall_write(1, buffer, length);
            if(i == (dim - 1)) continue;
            syscall_write(1,", ", 2);
        }
    }
}
int isEmptyArray(int dim, PCB *array[])
{
    for (int i = 0; i < dim; i++)
    {
        if (array[i] != NULL)
        {
            return 0;
        }
    }
    return 1;
}