typedef struct queue Queue;

Queue * newQueue(void);
void enQueue(Queue *q, void * value);
void * deQueue(Queue *q);
void destructQueue(Queue * p);
