#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include "queue.h"

#define EVER ;;

Queue firstLevel;
Queue secondLevel;
Queue thirdLevel;

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

void sigttinHandler(int signal)
{
    Process * curr = (Process *) deQueue(/*a definir*/);
    Process * next = (Process *) getValue(/*a definir*/);
    kill(curr->pid, SIGSTOP);
    kill(next->pid, SIGCONT);
    sleep(3);
    enQueue(/*a definir*/, curr);
    /*
    pega o processo w4IO e espera 3 segundos para alocalo numa fila
    de maior prioridade
    */
}

void sigchldHandler(int signal)
{
    Process * curr = (Process *) deQueue(/*a definir*/);
    Process * next = (Process *) getValue(/*a definir*/);
    free(curr);
    kill(next->pid, SIGCONT);
}

int main(int argc, char* argv[])
{    
    FILE * p;
    int quantum = 5,
        status;

    if (argc != 2)
    {
        puts("Error");
        exit(-1);
    }
    
    p = fopen(argv[1], 'r');
    
    /*
    contar o número de linhas do arquivo de leitura
    declarar um vetor com o nome dos executaveis e os parametros passados
    */
    
	signal(SIGTTIN, sigttinHandler);
	signal(SIGCHLD, sigchldHandler);
    signal(SIGALRM, sigalrmHandler);

    for (int i = 0; i < numProcesses; i++)
    {
        if(fork() == 0)
        {
            Process * new = (Process *) malloc (sizeof(Process));
            
            if (new == NULL)
            {
                puts(Error allocating memory);
                exit(-1);
            }

            new->pid = getpid();
            new->state = SleepingState;
            new->p = high;

            enQueue(firstLevel, new);
            /*
            argv[1] = primeira rajada
            argv[2] = segunda rajada
            argv[3] = terceira rajada
            */
            raise(SIGSTOP);
            execv(/*nome do executável*/"./exec",argv);
            exit(0);
        }
    }

    for (int i = 0; i < count; i++) waitpid(-1, &status, 0);

    for(EVER) alarm(quantum);

    /*
    Se as filas ficarem todas vazias o escalanador finaliza o processo
    */

	return 0;
}
