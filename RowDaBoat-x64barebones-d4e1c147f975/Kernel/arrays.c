#include "include/arrays.h"
extern int syscall_write(int, const char *, int);
extern uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);
void printArray(int dim, PCB *array[])
{
    char buffer[256] = {0};
    for (int i = 0; i < dim; i++)
    {
        if (array[i] != NULL)
        {
            syscall_write(1, buffer, uintToBase(array[i]->PID, buffer, 10));
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