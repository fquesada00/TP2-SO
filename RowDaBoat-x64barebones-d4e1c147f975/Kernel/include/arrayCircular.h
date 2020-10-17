#ifndef _ARRAY_CIRCULAR_H_
#define _ARRAY_CIRCULAR_H_
#include "arrays.h"
int addPCB(int startIdx, int dim, PCB *element, PCB *array[]);
PCB *removePCB(int *startIdx, int dim, PCB *array[]);
int isPresent(int startIdx, int dim, PCB *pcb, PCB *array[]);
PCB * peekPCB(int startIdx, PCB *array[]);
#endif