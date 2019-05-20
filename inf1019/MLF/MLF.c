#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include "queue.h"
#include "sem.h"

#define EVER ;;
#define SHMKEY 8847
#define NUM_OF_PRIORITY_QUEUES 3
#define QUANTUM 5

typedef struct program
{
    char executable[20];
    char cpuBound1[5];
    char cpuBound2[5];
    char cpuBound3[5];
} Program;

int lastToExecute, semid;
Queue * priorityQueues[NUM_OF_PRIORITY_QUEUES];

void processTerminated(Process p)
{
    printf(">> Process %d termineted!\n", p.pid);
}

void sigttinHandler(int signal)
{
    semaforoP(semid);
    Process next, curr = deQueue(priorityQueues[0]);
    
    if (!isEmpty(priorityQueues[0]))
    {
        next = getFront(priorityQueues[0]);
    }
    else
    {
        next = curr;
    }
    
    printf("> The process %d realized a I/O operation\n", curr.pid);

    switch(curr.bound)
    {
        case first:
            curr.bound = second;
            break;
        case second:
            curr.bound = third;
            break;
        default:
            break;
    }

    kill(next.pid, SIGCONT);
    enQueue(priorityQueues[0], curr);
    semaforoV(semid);
}

void sigchldHandler(int signal)
{
    Process curr, next;
    curr = getFront(priorityQueues[0]);

    switch(curr.bound)
    {
        case 0:
            break;
        case third:
            semaforoP(semid);
            if (lastToExecute == curr.pid)
            {
                curr = deQueue(priorityQueues[0]);
                processTerminated(curr);
                if (!isEmpty(priorityQueues[0]))
                {
                    next = getFront(priorityQueues[0]);
                    kill(next.pid, SIGCONT);
                    lastToExecute = next.pid;
                }
            }
            semaforoV(semid);
            break;
    }
}

void sigalrmHandler(int signal)
{
    semaforoP(semid);
   
    Process curr = deQueue(priorityQueues[0]);
    Process next = getFront(priorityQueues[0]);
    kill(curr.pid, SIGSTOP);
    printf("> Passaram %d segundos\n", QUANTUM);
    kill(next.pid, SIGCONT);
    lastToExecute = next.pid;
    enQueue(priorityQueues[0], curr);
    
    semaforoV(semid);
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

    semid = semget (IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    setSemValue(semid);

    /*
    if (argc != 2)
    {
        puts("> Error at the argument statements!");
        exit(-1);
    }
    */
    
    shmid = shmget (SHMKEY,
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
       newQueue(priorityQueues[i]);
       displayQueue(priorityQueues[i]);
       pointerToSharedMemory++;
    }
    
    puts("> The priority queues was inicialized.");

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
    signal(SIGCHLD, sigchldHandler);
    signal(SIGALRM, sigalrmHandler);
    
    for (int i = 0; i < numProcesses; i++)
    {
        semaforoP(semid);
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
            
            semaforoV(semid);
            
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
    kill(curr.pid, SIGCONT);
    lastToExecute = curr.pid;

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
    delSemValue(semid);
    shmdt (pointerToSharedMemory);
    shmctl (shmid, IPC_RMID, 0);

    return 0;
}