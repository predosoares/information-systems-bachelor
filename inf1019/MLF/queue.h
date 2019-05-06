#define NUM_OF_ELEMENTS 50

typedef struct queue
{ 
    int rear, front; 
    int size; 
    void * array[NUM_OF_ELEMENTS];
} Queue;

void newQueue(Queue * q);
void enQueue(Queue * q, void * value);
void * deQueue(Queue * q);
void * getFront(Queue * q);
