#ifndef _LIST_H_
#define _LIST_H_
#include "scheduler.h"
//Implementacion de listas de prioridad basada en https://www.geeksforgeeks.org/priority-queue-using-linked-list/
typedef PCB elem_t;


typedef struct ListElem { 
    elem_t data; 
  
    // Lower values indicate higher priority (max 0 - min 10) 
    int priority;
    //Tickets for scheduling time
    int tickets; 
  
    struct ListElem* next;

    struct ListElem* prev;

  
}listElem_t;

typedef struct{
    listElem_t * first;
    listElem_t * current;
    int size;
    int ready;
    int foreground;
}Header;

void initList(Header * head,elem_t firstNode, int p, int ticket);

listElem_t* newNode(elem_t d, int p, int tickets);

elem_t peek(Header * head); //Esta anda bien

void pop(Header * head); //Este anda bien 

void push(Header * head, elem_t d, int p, int tickets); //Este anda bien!

int isEmpty(Header * head); //Esta anda bien

elem_t * next(Header * head); //Este anda bien

listElem_t removeCurrent(Header * head);

listElem_t removeElement(Header * head,elem_t elem);

listElem_t * get(Header * head, elem_t elem);

#endif