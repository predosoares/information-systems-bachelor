#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "queue.h"

void newQueue( Queue * new )
{
    new->rear = -1;
    new->front = -1;
    new->size = NUM_OF_ELEMENTS;
    
    for (int i = 0; i < NUM_OF_ELEMENTS; i++)
    {
        new->array[i] = NULL;
    }
}

void enQueue(Queue * q, void * value) 
{ 
    if ((q->front == 0 && q->rear == q->size-1) || 
            (q->rear == (q->front-1)%(q->size-1))) 
    { 
        printf("\nQueue is Full"); 
        return;
    } 
  
    else if (q->front == -1) /* Insert First Element */
    { 
        q->front = q->rear = 0; 
        q->array[q->rear] = value; 
    } 
  
    else if (q->rear == q->size-1 && q->front != 0) 
    { 
        q->rear = 0; 
        q->array[q->rear] = value; 
    } 
  
    else
    { 
        q->rear++; 
        q->array[q->rear] = value; 
    }

    
} 
  
void * deQueue(Queue * q) 
{ 
    if (q->front == -1) 
    { 
        printf("\nQueue is Empty"); 
        return (void *)-1; 
    } 
  
    void * data = q->array[q->front]; 
    q->array[q->front] = (void *)-1; 
    
    if (q->front == q->rear) 
    { 
        q->front = -1; 
        q->rear = -1; 
    } 
    else if (q->front == q->size-1) 
        q->front = 0; 
    else
        q->front++; 
  
    return data; 
}

void * getFront(Queue * q)
{
    Queue * curr = q->array[q->front];
    return (void *) curr;
}