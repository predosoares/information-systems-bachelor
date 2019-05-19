#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <unistd.h>
#include "queue.h"

#define EVER ;;
#define SHMKEY 8842
#define NUM_OF_PRIORITY_QUEUES 3
#define QUANTUM 5

typedef struct program
{
    char executable[20];
    char cpuBound1[5];
    char cpuBound2[5];
    char cpuBound3[5];
} Program;

union semun
{
    int val;
    struct semid_ds *buf;
    ushort *array;
};

int lastToExecute, semid;
Queue * highPriorityQueue, * mediumPriorityQueue, * lowPriorityQueue; 

int setSemValue(int semId)
{
    union semun semUnion;
    semUnion.val = 1;
    return semctl(semId, 0, SETVAL, semUnion);
}

void delSemValue(int semId)
{
    union semun semUnion;
    semctl(semId, 0, IPC_RMID, semUnion);
}

int semaforoP(int semId)
{
    struct sembuf semB;
    semB.sem_num = 0;
    semB.sem_op = -1;
    semB.sem_flg = SEM_UNDO;
    while(semop(semId, &semB, 1) != 0);
    return 0;
}

int semaforoV(int semId)
{
    struct sembuf semB;
    semB.sem_num = 0;
    semB.sem_op = 1;
    semB.sem_flg = SEM_UNDO;
    while(semop(semId, &semB, 1) != 0);
    return 0;
}

void processTerminated(Process p)
{
    printf(">> Process %d termineted!\n", p.pid);
}

void sigttinHandler(int signal)
{
    semaforoP(semid);
    Process next, curr = deQueue(highPriorityQueue);
    
    if (!isEmpty(highPriorityQueue))
    {
        next = getFront(highPriorityQueue);
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
    //lastToExecute = next.pid;
    enQueue(highPriorityQueue, curr);
    semaforoV(semid);
}

void sigchldHandler(int signal)
{
    Process curr, next;
    curr = getFront(highPriorityQueue);

    switch(curr.bound)
    {
        case 0:
            break;
        case third:
            semaforoP(semid);
            if (lastToExecute == curr.pid)
            {
                curr = deQueue(highPriorityQueue);
                processTerminated(curr);
                if (!isEmpty(highPriorityQueue))
                {
                    next = getFront(highPriorityQueue);
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
   
    Process curr = deQueue(highPriorityQueue);
    Process next = getFront(highPriorityQueue);
    kill(curr.pid, SIGSTOP);
    printf("> Passaram %d segundos\n", QUANTUM);
    kill(next.pid, SIGCONT);
    lastToExecute = next.pid;
    enQueue(highPriorityQueue, curr);
    
    semaforoV(semid);
}

void printParametros(Program * p, int num)
{
    for (int i = 0; i < num; i++)
    {
        printf("> %s %s %s %s\n", p[i].executable, p[i].cpuBound1, p[i].cpuBound2, p[i].cpuBound3);
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
    return isEmpty(highPriorityQueue) && isEmpty(mediumPriorityQueue) && isEmpty(lowPriorityQueue);
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

    highPriorityQueue = pointerToSharedMemory;
    newQueue(highPriorityQueue);
    displayQueue(highPriorityQueue);

    pointerToSharedMemory++;
    mediumPriorityQueue = pointerToSharedMemory;
    newQueue(mediumPriorityQueue);
    displayQueue(mediumPriorityQueue);
    
    pointerToSharedMemory++;
    lowPriorityQueue = pointerToSharedMemory;
    newQueue(lowPriorityQueue);
    displayQueue(lowPriorityQueue);
    
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
    signal(SIGSTOP, SIG_IGN);
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
    
    displayQueue(highPriorityQueue);

    puts("> Start scheduler.");
    
    curr = getFront(highPriorityQueue);
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