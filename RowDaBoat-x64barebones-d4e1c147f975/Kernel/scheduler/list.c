#include "list.h"
#include "memory_manager.h"
listElem_t* newNode(elem_t d, int p,int ticket) 
{ 
    listElem_t* temp = (listElem_t*)pMalloc(sizeof(listElem_t)); 
    temp->data = d; 
    temp->priority = p; 
    temp->next = NULL;
    temp->prev = NULL;
    temp->current = temp;
    temp->tickets = ticket; 
  
    return temp; 
} 
  
// Return the value at head 
elem_t peek(listElem_t** head) 
{ 
    return (*head)->data; 
} 
  
// Removes the element with the 
// highest priority form the list 
void pop(listElem_t** head) 
{ 
    listElem_t* temp = *head; 
    (*head) = (*head)->next; 
    pFree(temp); 
} 
  
// Function to push according to priority 
void push(listElem_t** head, elem_t d, int p,int tickets) 
{ 
    listElem_t* start = (*head); 
  
    // Create new Node 
    listElem_t* temp = newNode(d, p,tickets); 
    // Special Case: The head of list has lesser 
    // priority than new ListElem. So insert new 
    // ListElem before head ListElem and change head ListElem. 
    if ((*head)->priority > p) { 
  
        // Insert New Node before head 
        temp->next = *head; 
        head->
        (*head) = temp; 
    } 
    else { 
  
        // Traverse the list and find a 
        // position to insert new ListElem 
        while (start->next != NULL && 
               start->next->priority <= p) { 
            start = start->next; 
        } 
  
        // Either at the ends of the list 
        // or at required position 
        temp->next = start->next; 
        start->next = temp; 
    } 
} 
  
// Function to check is list is empty 
int isEmpty(listElem_t** head) 
{ 
    return (*head) == NULL; 
}

elem_t next(listElem_t ** head)
{
    listElem_t * start = (*head);
    elem_t ret = {-1,-1,-1};
    if(start == NULL || start->current == NULL)
        return  ret;
    if(start->current->next == NULL)
        start->current = start;
    else
        start->current = start->current->next;
    return start->current->data;
}

void removeCurrent(listElem_t ** head) {
    listElem_t * start = (*head);

    toFree = head->current;

    if(toFree == start) { //Si es igual a la primera muevo todos los valores del segundo al primero y borro el segundo
        toFree = start->next;
        start->data = toFree->data;
        start->priority = toFree->priority;
        start->tickets = toFree->tickets;
        start->next = toFree->next;
        //start->current = ? CHAN! 
    }
}