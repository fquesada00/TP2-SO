#include "list.h"
#include "memory_manager.h"
#include "scheduler.h"

extern void printList(Header *head);

void initList(Header *head, elem_t firstNode, int p, int ticket)
{
    head->first = newNode(firstNode, p, ticket);
    head->current = head->first;
    head->size = 1;
}

listElem_t *newNode(elem_t d, int p, int ticket)
{
    listElem_t *temp = (listElem_t *)pMalloc(sizeof(listElem_t));
    temp->data = d;
    temp->priority = p;
    temp->next = NULL;
    temp->prev = NULL;
    temp->tickets = ticket;

    return temp;
}

// Return the value at head
elem_t peek(Header *head)
{
    return (head->first->data);
}

// Removes the element with the
// highest priority form the list
void pop(Header *head)
{
    listElem_t *temp = (head->first);
    head->size--;
    if (head->first->next == NULL)
    {
        head->first = NULL;
        head->current = NULL;
        pFree(temp);
        return;
    }
    if (head->current == head->first)
    {
        head->current = head->current->next;
    }
    head->first = head->first->next;
    head->first->prev = NULL;
    pFree(temp);
}

// Function to push according to priority
void push(Header *head, elem_t d, int p, int tickets)
{
    head->size++;
    if (isEmpty(head))
    {
        head->first = newNode(d, p, tickets);
        head->current = head->first;
        return;
    }
    listElem_t *start = head->first;

    // Create new Node
    listElem_t *temp = newNode(d, p, tickets);
    // Special Case: The head of list has lesser
    // priority than new ListElem. So insert new
    // ListElem before head ListElem and change head ListElem.
    if (head->first->priority > p)
    {

        // Insert New Node before head
        temp->next = head->first;
        head->first->prev = temp;
        head->first = temp;
    }
    else
    {

        // Traverse the list and find a
        // position to insert new ListElem
        while (start->next != NULL &&
               start->next->priority <= p)
        {
            start = start->next;
        }
        if (start->next == NULL)
        {
            start->next = temp;
            temp->prev = start;
            return;
        }
        // Either at the ends of the list
        // or at required position
        temp->next = start->next;
        start->next->prev = temp;
        temp->prev = start;
        start->next = temp;
    }
}

// Function to check is list is empty
int isEmpty(Header *head)
{
    return head->first == NULL;
}

elem_t * next(Header *head)
{
    elem_t ret = {{0}, 0, {0}, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    if (isEmpty(head) || head == NULL)
        return NULL;

    listElem_t *start = head->first;

    if (head->current->next == NULL)
        head->current = start;
    else
        head->current = head->current->next;
    elem_t e = (head->current->data);
    return &e;
}

//gets element and removes the node from the list
listElem_t removeElement(Header *head, elem_t elem)
{
    listElem_t le;
    le.priority = -1;

    if (isEmpty(head) || head->current == NULL)
        return le;

    listElem_t *current = head->first;
    listElem_t *next = current;

    while (next != NULL && next->data.PID != elem.PID)
    {
        current = next;
        next = current->next;
    }
    if (next == NULL)
        return le;
    if (current == next)
    {
        le = (*current);
        pop(head);
        return le;
    }
    if(head->current == next)
        if(head->current->next != NULL)
            head->current=head->current->next;
        else
            head->current=head->first;
            
    current->next = next->next;
    le = *next;
    pFree(next);

    return le;
}

listElem_t removeCurrent(Header *head)
{
    listElem_t e = {{{0}, 0, {0}, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 0, 0,0};
    if (isEmpty(head) || head->first == NULL)
        return e;
    listElem_t *next = head->current;
    if (next == head->first)
    {
        e = *(head->first);
        pop(head);
        return e;
    }
    if (head->current->next == NULL)
    {
        head->current->prev->next = NULL;
    }
    else
    {
        head->current->prev->next = head->current->next;
        head->current->next->prev = head->current->prev;
        head->current = head->current->prev;
    }
    e = *next;
    pFree(next);
    return e;
}
listElem_t *get(Header *head, elem_t elem)
{
    if (head == NULL || head->first == NULL)
        return NULL;
    listElem_t *iter = head->first;
    while (iter != NULL && iter->data.PID != elem.PID)
    {
        iter = iter->next;
    }
    return iter;
}