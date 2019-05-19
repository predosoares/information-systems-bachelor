#define NUM_OF_ELEMENTS 50

typedef enum {
    RunningState,
    SleepingState,
    w4ioState
} processState;

typedef enum {
    high,
    medium,
    low
} priority_t;

typedef enum {
    first,
    second,
    third
} bound_t;

typedef struct process
{
    pid_t pid;
    processState state;
    priority_t priority;
    bound_t bound;
} Process;

typedef struct queue
{ 
    int rear, front;
    int size; 
    Process array[NUM_OF_ELEMENTS];
} Queue;

void newQueue(Queue * q);
void enQueue(Queue * q, Process p);
Process deQueue(Queue * q);
Process getFront(Queue * q);
void displayQueue(Queue * q);
int isEmpty(Queue * q);
