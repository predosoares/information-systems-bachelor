#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include "queue.h"

#define EVER ;;
#define SHMKEY 8847
#define NUM_OF_PRIORITY_QUEUES 3
#define QUANTUM 1
#define WAIT 3

typedef struct program
{
    char executable[20];
    char cpuBound1[5];
    char cpuBound2[5];
    char cpuBound3[5];
} Program;

priority_t priorityOfCurrentProcess;
unsigned short quantuns;
Queue * priorityQueues[NUM_OF_PRIORITY_QUEUES];
Queue * waitingIOQueue;

int power(int x, unsigned int y)
{
    int temp;
    if (y == 0)
    {
        return 1;
    }

    temp = power (x, y / 2);
    if ((y % 2) == 0)
    {
        return temp * temp;
    }
    else
    {
        return x * temp * temp;
    }
}

void processTerminated(Process p)
{
    printf(">> Process %d termineted!\n", p.pid);
}

void startNextProcess(void)
{
    Process next;

    for (int i = 0; i < NUM_OF_PRIORITY_QUEUES; i++)
    {
        if (!isEmpty(priorityQueues[i]))
        {
            next = getFront(priorityQueues[i]);
            next.state = RunningState;
            priorityOfCurrentProcess = next.priority;
            quantuns = (unsigned short) power(2,priorityOfCurrentProcess);
            kill(next.pid, SIGCONT);
            break;
        }
    }
}

void sigusr1Handler(int signal)
{
    Process curr = deQueue(priorityQueues[priorityOfCurrentProcess]);

    processTerminated(curr);

    startNextProcess();
}

void handlePriority(bool up)
{
    unsigned short gap;
    Process curr = deQueue(priorityQueues[priorityOfCurrentProcess]);
    curr.state = SleepingState;

    switch(up)
    {
        case true:
            gap = (priorityOfCurrentProcess == high) ? 0 : 1;
            curr.priority -= gap;
            enQueue(priorityQueues[priorityOfCurrentProcess - gap], curr);
            break;
        case false:
            gap = (priorityOfCurrentProcess == low) ? 0 : 1;
            curr.priority += gap;
            enQueue(priorityQueues[priorityOfCurrentProcess + gap], curr);
            break;
        default:
            break;
    }
}
/*
void insertInWaitingQueue(Process curr)
{
    time_t start;
    int gap;
    time(&start);
    curr.start = clock();
    
    gap = (priorityOfCurrentProcess == high) ? 0 : 1;
    curr.priority = priorityOfCurrentProcess - gap;

    enQueue(waitingIOQueue, curr);
}
*/
/*
void checkIfDoneIOAndPutInReady(void)
{
    Process waiting, ready;
    time_t segundos;
    int numOfWaitingProcesses;
    double diff_t;
     
    if (!isEmpty(waitingIOQueue))
    {
        numOfWaitingProcesses = numberOfNodes(waitingIOQueue);
        time(&segundos);

        for (int i = 0; i <= numOfWaitingProcesses; i++)
        {
            waiting = getFront(waitingIOQueue);
            diff_t = (clock() - waiting.start)/CLOCKS_PER_SEC;
            printf(">>> Waited for %f sec\n", diff_t);
            if ( diff_t >= WAIT )
            {
                
                ready = deQueue(waitingIOQueue);
                printf("> The process %d is ready again\n", ready.pid);
                enQueue(priorityQueues[ready.priority], ready);
            }
        }
    }
}
*/
void sigttinHandler(int signal)
{
    Process curr = getFront(priorityQueues[priorityOfCurrentProcess]);
    printf("> The process %d realized a I/O operation\n", curr.pid);
    
    //checkIfDoneIOAndPutInReady();
    //insertInWaitingQueue(curr);
    handlePriority(true);
    
    startNextProcess();
}

void sigalrmHandler(int signal)
{
    Process curr = getFront(priorityQueues[priorityOfCurrentProcess]);
    printf("> Passaram %d segundos\n", QUANTUM);
    
    //checkIfDoneIOAndPutInReady();

    quantuns--;

    if ((quantuns == 0) && (curr.pid != -1))
    {
        kill(curr.pid, SIGSTOP);
        handlePriority(false); //para descer
        startNextProcess();
    }    
}

void printProcess(Process p)
{
    printf("> %d %d %d %d\n", p.pid, p.state, p.priority, p.bound);
}

Process createNewProcessId(void)
{
    Process new;

    new.pid = getpid();
    new.state = SleepingState;
    new.priority = high;
    new.bound = first;

    return new;
}

int allEmpty(void)
{
    for (int i = 0; i < NUM_OF_PRIORITY_QUEUES; i++)
    {
        if (!isEmpty(priorityQueues[i]))
        {
            return false;
        }
    }

    if (!isEmpty(waitingIOQueue))
    {
        return false;
    }

    return true;
}

int main(int argc, char* argv[])
{    
    FILE * pFile;
    Program programs[NUM_OF_ELEMENTS];
    Queue * pointerToSharedMemory;
    Process curr;
    pid_t pid;
    char program[20], burst1[5], burst2[5], burst3[5];
    int status, shmid, numProcesses = 0;

    /*
    if (argc != 2)
    {
        puts("> Error at the argument statements!");
        exit(-1);
    }
    */
    
    shmid = shmget (IPC_PRIVATE,
                    (NUM_OF_PRIORITY_QUEUES*sizeof(Queue)),
                    IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
    
    if (shmid == -1)
    {
        puts ("> Error allocating shared memory!");
        exit (-1);
    }
    
    pointerToSharedMemory = (Queue *) shmat (shmid, 0, 0);

    if (pointerToSharedMemory == (Queue *)-1)
    {
        puts ("> Error attaching the memory to scheduler!");
        exit (-1);
    }

    puts("> The memory was correctly attached to the main process.");

    for (int i = 0; i < NUM_OF_PRIORITY_QUEUES; i++)
    {
       priorityQueues[i] = pointerToSharedMemory;
       newQueue(priorityQueues[i], i);
       pointerToSharedMemory++;
    }
    
    puts("> The priority queues was inicialized.");

    waitingIOQueue = (Queue *) malloc(sizeof(Queue));
    newQueue(waitingIOQueue, 99);

    puts("> The waiting for I/O queue was inicialized.");

    pFile = fopen("processos.txt", "r");

    while (fscanf(pFile,"%s %s %s %s", &program, &burst1, &burst2, &burst3) != EOF)
    {
        strcpy(programs[numProcesses].executable, program);
        strcpy(programs[numProcesses].cpuBound1, burst1);
        strcpy(programs[numProcesses].cpuBound2, burst2);
        strcpy(programs[numProcesses].cpuBound3, burst3);
        numProcesses++;
    }

    fclose(pFile);

    puts("> The file was read and the data was stored.");
    printf("> There is a total of %d processes.\n", numProcesses);

    signal(SIGTTIN, sigttinHandler);
    signal(SIGUSR1, sigusr1Handler);
    signal(SIGALRM, sigalrmHandler);
    
    for (int i = 0; i < numProcesses; i++)
    {
        
        puts("> Creating a child process.");

        if((pid = fork()) == 0) // Child code
        {
            Process new = createNewProcessId();

            printf("> Putting in the queue [pid: %d]\n", new.pid);
            
            pointerToSharedMemory = (Queue *) shmat (shmid, 0, 0);
            
            enQueue(pointerToSharedMemory, new);

            shmdt (pointerToSharedMemory);
            
            argv[0] = programs[i].executable;
            argv[1] = programs[i].cpuBound1;
            argv[2] = programs[i].cpuBound2;
            argv[3] = programs[i].cpuBound3;
            
            puts("> Putting to sleep.");
            
            raise(SIGSTOP);
            
            execv(argv[0], argv);
            // Em casos de erro
            exit(-1);

        } else if (pid == -1)
        {
            puts("> Error making a child process!");
            exit(-1);
        }
    }

    //Espera todos os processos filhos estarem suspensos
    for (int i = 0; i < numProcesses; i++) waitpid(-1, &status, WUNTRACED);
    
    displayQueue(priorityQueues[0]);

    puts("> Start scheduler.");
    
    curr = getFront(priorityQueues[0]);
    curr.state = RunningState;
    priorityOfCurrentProcess = high;
    quantuns = 1;
    kill(curr.pid, SIGCONT);

    for(EVER)
    {
        alarm(QUANTUM);
        pause();

        if (allEmpty())
        {
            break;
        }
    }

    puts("> End scheduler.");
    free(waitingIOQueue);
    shmdt (pointerToSharedMemory);
    shmctl (shmid, IPC_RMID, 0);

    return 0;
}