#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "queue.h"

void newQueue( Queue * new, priority_t priority)
{
    new->rear = -1;
    new->front = -1;
    new->size = NUM_OF_ELEMENTS;
    new->priority = priority;
    
    for (int i = 0; i < NUM_OF_ELEMENTS; i++)
    {
        new->array[i].pid = -1;
        new->array[i].state = 99;
        new->array[i].priority = 99;
    }
}

void newNode(Queue * q, Process p)
{
    q->array[q->rear].pid = p.pid;
    q->array[q->rear].state = p.state;
    q->array[q->rear].priority = p.priority;
    q->array[q->rear].start = p.start;
}

void enQueue(Queue * q, Process p) 
{ 
    if ((q->front == 0 && q->rear == q->size-1) || 
            (q->rear == (q->front-1)%(q->size-1))) 
    { 
        printf("Queue is Full\n"); 
        return;
    } 
  
    else if (q->front == -1) /* Insert First Element */
    { 
        q->front = q->rear = 0; 
        newNode(q, p);
    } 
  
    else if (q->rear == q->size-1 && q->front != 0) 
    { 
        q->rear = 0; 
        newNode(q, p);
    } 
  
    else
    { 
        q->rear++; 
        newNode(q, p);
    }
} 
  
Process deQueue(Queue * q) 
{
    Process data;

    if (q->front == -1) 
    { 
        printf("> Queue is Empty\n");
        data.pid = -1;
        return data; 
    } 
  
    data = q->array[q->front]; 
    
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

Process getFront(Queue * q)
{
    Process curr;

    if (q->front == -1)
    {
        printf("> Queue is Empty, cannot get front!\n");
        curr.pid = -1;
        return curr;
    }
    
    curr = q->array[q->front];
    return curr;
}

void displayQueue(Queue * q) 
{ 
    if (q->front == -1) 
    { 
        puts("> Queue is Empty"); 
        return; 
    } 
    printf("> Elements in Circular Queue are: \n"); 
    if (q->rear >= q->front) 
    { 
        for (int i = q->front; i <= q->rear; i++) 
            printf("\t>> %d %d %d \n", q->array[i].pid, q->array[i].state, q->array[i].priority); 
    } 
    else
    { 
        for (int i = q->front; i < q->size; i++) 
            printf("%d %d %d \n", q->array[i].pid, q->array[i].state, q->array[i].priority);
  
        for (int i = 0; i <= q->rear; i++) 
            printf("%d %d %d \n", q->array[i].pid, q->array[i].state, q->array[i].priority); 

    } 
}

int isEmpty(Queue * q)
{
    return (q->front == -1);
}

int numberOfNodes(Queue * q)
{
    return abs(abs(q->size - q->front) - abs(q->size - q->rear));
}