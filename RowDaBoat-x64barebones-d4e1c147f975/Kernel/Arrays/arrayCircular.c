// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <stddef.h>
#include "arrayCircular.h"
int addPCB(int startIdx, int dim, PCB *element, PCB *array[])
{
    for (int i = startIdx, j = 0; j < dim; i++, j++)
    {
        if (i == dim)
        {
            i = 0;
        }
        if (array[i] == NULL)
        {
            array[i] = element;
            return 0;
        }
        else if (array[i] == element)
        {
            return 0;
        }
    }
    return 1;
}
PCB *removePCB(int *startIdx, int dim, PCB *array[])
{
    if (array[*startIdx] == NULL)
    {
        return NULL;
    }
    PCB *auxPCB = array[*startIdx];
    array[*startIdx] = NULL;
    if (*startIdx == (dim - 1))
    {
        *startIdx = 0;
    }
    else
    {
        (*startIdx)++;
    }
    return auxPCB;
}
int isPresent(int startIdx, int dim, PCB *pcb, PCB *array[])
{
    if (array[startIdx] == NULL)
        return 0;
    for (int i = 0; i < dim; i++)
    {
        if (pcb == array[i])
        {
            return 1;
        }
    }
    return 0;
}
PCB *peekPCB(int startIdx, PCB *array[])
{
    if (array[startIdx] == NULL)
    {
        return NULL;
    }
    return array[startIdx];
}