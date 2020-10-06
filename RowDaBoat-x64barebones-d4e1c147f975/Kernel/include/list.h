#ifndef _LIST_H_
#define _LIST_H_
#include "scheduler.h"
//Implementacion de listas de prioridad basada en https://www.geeksforgeeks.org/priority-queue-using-linked-list/
typedef struct elem{
    PCB pcb;
}elem_t;
typedef struct ListElem { 
    elem_t data; 
  
    // Lower values indicate higher priority (max 0 - min 10) 
    int priority;
    //Tickets for scheduling time
    int tickets; 
  
    struct ListElem* next;

    struct ListElem* prev;

    struct ListElem * current;
  
}listElem_t;

listElem_t* newNode(elem_t d, int p, int tickets);

elem_t peek(listElem_t** head);

void pop(listElem_t** head);

void push(listElem_t** head, elem_t d, int p, int tickets);

int isEmpty(listElem_t** head);

elem_t next(listElem_t ** head);

void removeCurrent(listElem_t ** head); //podria devovler si lo pudo borrar o no pero no se porque fallaria, 



#endif