#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include "queue.h"

#define SHMKEY 4406
#define EVER ;;
#define NUM_OF_PRIORITY_QUEUES 3

typedef enum {
    RunningState,
    SleepingState,
    w4ioState
} processState;

typedef enum {
    high,
    medium,
    low
} priority;

typedef struct process
{
    pid_t pid;
    processState state;
    priority p;
} Process;

typedef struct program
{
    char executable[20];
    char cpuBound1[5];
    char cpuBound2[5];
    char cpuBound3[5];
} Program;

Queue * highPriorityQueue, * meniumPriorityQueue, * lowPriorityQueue; 

void sigttinHandler(int signal)
{
    Process * curr = (Process *) deQueue(highPriorityQueue);
    Process * next = (Process *) getFront(highPriorityQueue);
    kill(curr->pid, SIGSTOP);
    kill(next->pid, SIGCONT);
    sleep(3);
    enQueue(highPriorityQueue, (void *)curr);
    /*
    pega o processo w4IO e espera 3 segundos para alocalo numa fila
    de maior prioridade
    */
}

void sigchldHandler(int signal)
{
    Process * curr = (Process *) deQueue(highPriorityQueue);
    Process * next = (Process *) getFront(highPriorityQueue);
    free(curr);
    kill(next->pid, SIGCONT);
}

void sigalrmHandler(int signal)
{

}

int main(int argc, char* argv[])
{    
    FILE * pFile;
    Program programs[NUM_OF_ELEMENTS];
    Queue * pointerToSharedMemory;
    Process * curr;
    pid_t pid;
    char program[20], burst1[5], burst2[5], burst3[5];
    int quantum = 5, status, shmid, numProcesses = 0;

    if (argc != 2)
    {
        puts("> Error at the argument statements!");
        exit(-1);
    }
    
    shmid = shmget (SHMKEY,(NUM_OF_PRIORITY_QUEUES*sizeof(Queue)), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
    
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

    puts("> The memory was correctly attached to the main process");

    highPriorityQueue = pointerToSharedMemory;
    newQueue(highPriorityQueue);

    pointerToSharedMemory++;
    meniumPriorityQueue = pointerToSharedMemory;
    newQueue(meniumPriorityQueue);
    
    pointerToSharedMemory++;
    lowPriorityQueue = pointerToSharedMemory;
    newQueue(lowPriorityQueue);
    
    puts("> The priority queues was inicialized");

    pFile = fopen(argv[1], "r");

    while (fscanf(pFile,"%s %s %s %s", &program, &burst1, &burst2, &burst3) != EOF)
    {
        strcpy(programs[numProcesses].executable, program);
        strcpy(programs[numProcesses].cpuBound1, burst1);
        strcpy(programs[numProcesses].cpuBound2, burst2);
        strcpy(programs[numProcesses].cpuBound3, burst3);
        numProcesses++;
    }

    fclose(pFile);

    puts("> The file was read and the data was stored");

	signal(SIGTTIN, sigttinHandler);
	signal(SIGCHLD, sigchldHandler);
    signal(SIGALRM, sigalrmHandler);
    
    for (int i = 0; i < numProcesses; i++)
    {
        if (i > 0)
        {
            waitpid(-1, &status, WUNTRACED);
            puts("> Waiting to sleep\n");
        }

        if((pid = fork()) == 0) // Child code
        {
            Process * new = (Process *) malloc (sizeof(Process));
            
            if (new == NULL)
            {
                puts("> Error allocating memory for process!");
                exit(-1);
            }

            new->pid = getpid();
            new->state = SleepingState;
            new->p = high;
            puts("> Putting in the queue\n");
            enQueue(highPriorityQueue, (void *)new);

            argv[0] = programs[i].executable;
            argv[1] = programs[i].cpuBound1;
            argv[2] = programs[i].cpuBound2;
            argv[3] = programs[i].cpuBound3;
            puts("> Putting to sleep\n");
            raise(SIGSTOP);
            execv(argv[0], argv);

        } else if (pid == -1)
        {
            puts("> Error making a child process!");
            exit(-1);
        }
    }

    // Espera todos os processos filhos estarem suspensos
    //for (int i = 0; i < numProcesses; i++) waitpid(-1, &status, WUNTRACED);
    
    puts("> Start scheduler");
    // Start
    curr = (Process *) getFront(highPriorityQueue);
    curr->state = RunningState;
    kill(curr->pid, SIGCONT);

    for(EVER) alarm(quantum);

    shmdt (pointerToSharedMemory);
    shmctl (shmid, IPC_RMID, 0);

	return 0;
}
