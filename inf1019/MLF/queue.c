#include <stdlib.h>
#include "queue.h"

struct Node 
{ 
    void * data; 
    struct Node* link; 
}; 
  
struct queue 
{
    struct Node *front, *rear; 
};

Queue * newQueue(void)
{
    Queue * new = (Queue *) malloc (sizeof(Queue));
    
    if (new == NULL)
    {
        puts("Error");
        exit(-1);
    }

    new->front = NULL;
    new->rear = NULL;
    return new;
}

void enQueue(Queue *q, void * value)
{
    struct Node *temp = (struct Node *) malloc (sizeof(struct Node));
    
    temp->data = value; 
    
    if (q->front == NULL)
    {
        q->front = temp; 
    }
    else
    {
        q->rear->link = temp;
    }
    
    q->rear = temp;
    q->rear->link = q->front; 
} 


void * deQueue(Queue *q) 
{ 
    if (q->front == NULL) 
    {
        printf ("Queue is empty"); 
        return ""; 
    } 
  
    void * value;
    
    if (q->front == q->rear) 
    { 
        value = q->front->data; 
        free(q->front); 
        q->front = NULL; 
        q->rear = NULL; 
    } 
    else
    { 
        struct Node *temp = q->front; 
        value = temp->data; 
        q->front = q->front->link; 
        q->rear->link= q->front; 
        free(temp); 
    } 
  
    return value ; 
}

void * getValue(Queue * p)
{
    return p->front->data; 
}

void destructQueue(Queue * p)
{
    free(p);
}
