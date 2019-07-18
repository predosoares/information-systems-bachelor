/***************************************************************************
*  $MCI Módulo de implementação: ESC  Escalonador de Processos por Fila de 
*                                    Prioridade
*
*  Arquivo gerado:              ESCALONADOR.C
*  Letras identificadoras:      ESC
*
*  Projeto: INF 1019 Escalonador de Processos por Fila de Prioridade em C
*  Autores: phs - Pedro Henrique Soares	
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*     1.0.0    phs  18/jun/2019 Documentação do código-fonte
*
***************************************************************************/

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

#define ANSI_COLOR_GREEN "\e[0;32m"
#define ANSI_COLOR_RED  "\e[1;31m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define CLEAR_SCREEN puts("\x1b[H\x1b[2J")

#define EVER ;;
#define NUM_OF_PRIORITY_QUEUES 3
#define QUANTUM 1
#define WAIT 3

/***********************************************************************
*
*  $TC Tipo de dados: ESC Elemento de Vetor de Programas
*
***********************************************************************/

typedef struct program
{
    char executable[20] ;
        /* Cadeira de caracteres que descreve o nome do executável */

    char cpuBound[10][5] ;
        /* Cadeira de caracteres numéricos que irão representar
         * os segundos que o processo ficará em CPU-bound */

    int numCpuBounds ;
        /* Número de CPU-bounds */
} Program ;

/***** Protótipos das funções encapuladas no Módulo *****/

static void printPriorityQueue( void ) ;

static int power( int x , unsigned int y ) ;

static void processTerminated( Process p ) ;

static void startNextProcess( void ) ;

static void sigusr1Handler( int signal ) ;

static void handlePriority( bool up ) ;

static void insertInWaitingQueue( Process curr ) ;

static void checkIfDoneIOAndPutInReady( void ) ;

static void sigttinHandler( int signal ) ;

static void sigalrmHandler( int signal ) ;

static void printProcess( Process p ) ;

static Process createNewProcessId( void ) ;

static int allEmpty( void ) ;

static void readAllProcessess( char * filename, int * numProcesses, Program * programs ) ;

/*****  Dados encapsulados no módulo  *****/

static priority_t priorityOfCurrentProcess ;
                /* Prioridade do processo que está em 
                 * execução */ 

static unsigned short quantuns,
                /* Contador de nº de quantuns de CPU que faltam
                 * para a fila de prioridade em execução terminar
                 * seu delta de execução */
                  allQueuesAreEmpty = 0;
                /* Flag que indica se todas as filas administradas
                 * pelo escalonador estão vazias */

Queue * priorityQueues[NUM_OF_PRIORITY_QUEUES];
                /* Filas de prioridade manipuladas pelo escalonador
                 * Para cada fila deve haver um valor de prioridade
                 * que deve ser registrado no enum de prioridades*/

Queue * waitingIOQueue;
                /* Fila de processos em espera de I/O */



static void printPriorityQueue(void)
{
    CLEAR_SCREEN;
    printf("PRIORITY QUEUES - CPU QUANTUM : %d sec\n", QUANTUM);
    
    for(int i = 0; i < NUM_OF_PRIORITY_QUEUES; i++)
    {
        puts("---------------------------------------------------------------------------------------");
        printf("NIVEL %d :", i + 1);

        if (priorityQueues[i]->rear >= priorityQueues[i]->front) 
        { 
            for (int j = priorityQueues[i]->front; j <= priorityQueues[i]->rear && j != -1; j++) 
                printf("| pid : %s%d%s |", ANSI_COLOR_GREEN, priorityQueues[i]->array[j].pid, ANSI_COLOR_RESET);
        } 
        else
        { 
            for (int j = priorityQueues[i]->front; j < priorityQueues[i]->size && j != -1; j++) 
                printf("| pid : %s%d%s |", ANSI_COLOR_GREEN, priorityQueues[i]->array[j].pid, ANSI_COLOR_RESET);
            for (int j = 0; j <= priorityQueues[i]->rear && j != -1; j++) 
                printf("| pid : %s%d%s |", ANSI_COLOR_GREEN, priorityQueues[i]->array[j].pid, ANSI_COLOR_RESET);
        } 
        puts("");
        puts("---------------------------------------------------------------------------------------");
    }

    puts("---------------------------------------------------------------------------------------");
    printf("WAITING IO :");
    if (waitingIOQueue->rear >=waitingIOQueue->front) 
    { 
        for (int j = waitingIOQueue->front; j <= waitingIOQueue->rear && j != -1; j++) 
            printf("| pid : %s%d%s |", ANSI_COLOR_RED, waitingIOQueue->array[j].pid, ANSI_COLOR_RESET);
    } 
    else
    { 
        for (int j = waitingIOQueue->front; j < waitingIOQueue->size && j != -1; j++) 
            printf("| pid : %s%d%s |", ANSI_COLOR_RED, waitingIOQueue->array[j].pid, ANSI_COLOR_RESET);    
        for (int j = 0; j <= waitingIOQueue->rear && j != -1; j++) 
            printf("| pid : %s%d%s |", ANSI_COLOR_RED, waitingIOQueue->array[j].pid, ANSI_COLOR_RESET);
    } 
    puts("");
    puts("---------------------------------------------------------------------------------------");
}

static int power( int x , unsigned int y )
{
    int temp ;

    if ( y == 0 ) { return 1 ; }

    temp = power ( x , y / 2 ) ;

    if ( (y % 2) == 0 ) { return temp * temp ; }
    else { return x * temp * temp ; }
}

static void processTerminated( Process p )
{
    printf(">> Process %d terminated!\n" , p.pid ) ;
}

static void startNextProcess( void )
{
    Process next;

    for (int i = 0; i < NUM_OF_PRIORITY_QUEUES; i++)
    {
        if (!isEmpty(priorityQueues[i]))
        {
            next = getFront(priorityQueues[i]);
            next.state = RunningState;
            priorityOfCurrentProcess = next.priority;
            quantuns = (unsigned short) power( 2, priorityOfCurrentProcess ) ;
            kill(next.pid, SIGCONT);
            break;
        }
    }
}

static void sigusr1Handler( int signal )
{
    Process curr = deQueue( priorityQueues[priorityOfCurrentProcess] ) ;

    processTerminated( curr ) ;

    startNextProcess( ) ;
    printPriorityQueue( ) ;
}

static void handlePriority( bool up )
{
    unsigned short gap;
    Process curr = up ? deQueue(waitingIOQueue) : deQueue(priorityQueues[priorityOfCurrentProcess]);
    curr.state = SleepingState;
    char priority[7];

    switch(up)
    {
        case true:
            gap = (curr.priority == high) ? 0 : 1;
            curr.priority = (priority_t) curr.priority - gap;
            enQueue(priorityQueues[curr.priority], curr);
            break;
        case false:
            gap = (curr.priority == low) ? 0 : 1;
            curr.priority = (priority_t) curr.priority + gap;
            enQueue(priorityQueues[curr.priority], curr);
            break;
    }

    if (allQueuesAreEmpty == 1)
    {
        priorityOfCurrentProcess = curr.priority;
        allQueuesAreEmpty = 0;
    }

    printPriorityQueue();
}

static void insertInWaitingQueue( Process curr )
{
    time_t start;
    time(&start);

    curr.start = start;

    enQueue(waitingIOQueue, curr);
    printPriorityQueue();
}

static void checkIfDoneIOAndPutInReady( void )
{
    Process waiting, ready;
    time_t seconds;
    int numOfWaitingProcesses;
    int diff_t;
    
    allQueuesAreEmpty = (isEmpty(priorityQueues[0]) && isEmpty(priorityQueues[1]) && isEmpty(priorityQueues[2])) ? 1 : 0;
    
    if (!isEmpty(waitingIOQueue))
    {
        numOfWaitingProcesses = numberOfNodes(waitingIOQueue);
        time(&seconds);

        for (int i = 0; i <= numOfWaitingProcesses; i++)
        {
            waiting = getFront(waitingIOQueue);
            diff_t = difftime(seconds, waiting.start);

            if ( diff_t >= WAIT )
            {
                handlePriority(true);
                printPriorityQueue();
            }
            else
            {
                break;
            }
        }
    }
}

static void sigttinHandler( int signal )
{
    Process curr = deQueue(priorityQueues[priorityOfCurrentProcess]);
    printf("> The process %d realized a I/O operation\n", curr.pid);
    
    checkIfDoneIOAndPutInReady();
    insertInWaitingQueue(curr);
    
    startNextProcess();
    printPriorityQueue();
}

static void sigalrmHandler( int signal )
{
    Process curr;
    checkIfDoneIOAndPutInReady();

    if(allQueuesAreEmpty == 1)
    {
        curr.pid = -1;
    }
    else
    {
        curr = getFront(priorityQueues[priorityOfCurrentProcess]);
    }

    quantuns--;

    if ((quantuns <= 0) && (curr.pid != -1))
    {
        kill(curr.pid, SIGSTOP);
        handlePriority(false); //para descer
        startNextProcess();
    }
    printPriorityQueue();
}

static void printProcess( Process p )
{
    printf("> %d %d %d \n", p.pid, p.state, p.priority);
}

static Process createNewProcessId( void )
{
    Process newProcess;

    newProcess.pid = getpid();
    newProcess.state = SleepingState;
    newProcess.priority = high;

    return newProcess;
}

static int allEmpty( void )
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

static void readAllProcessess( char * filename, int * numProcesses, Program * programs )
{
    FILE * fd;
    char buffer[100], * word ;
    int i ;

    fd = fopen( filename , "r" ) ;

    while ( fgets( buffer , sizeof(buffer), fd ) != NULL )
    {
        i = 0 ;

        word = strtok( buffer , " ") ;
        strcpy( programs[ *numProcesses ].executable , word) ;
        
        while( *(word = strtok( NULL , " " ) ) != '.' )
        {
            strcpy( programs[ *numProcesses ].cpuBound[i] , word ) ;
            i++ ;
        }

        programs[ *numProcesses ].numCpuBounds = i ;

        (*numProcesses)++ ;
    }

    fclose( fd ) ;
}

int main( int argc , char * argv[] )
{
    Program programs[NUM_OF_ELEMENTS];
    Queue * pointerToSharedMemory;
    Process curr;
    int i;
    pid_t pid;
    int status, shmid, numProcesses = 0;

    if(argc != 2)
    {
        puts("Error - Devem ser passados 2 parâmetros") ;
        puts("Try: ./MLF <filename>") ;
        exit(-1);
    }
    
    shmid = shmget ( IPC_PRIVATE,
                     ( NUM_OF_PRIORITY_QUEUES* sizeof( Queue ) ),
                     IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR ) ;
    
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

    for (int i = high; i < NUM_OF_PRIORITY_QUEUES; i++)
    {
       priorityQueues[i] = pointerToSharedMemory;
       newQueue(priorityQueues[i], ( priority_t ) i) ;
       pointerToSharedMemory++;
    }
    
    puts("> The priority queues was inicialized.");

    waitingIOQueue = ( Queue * ) malloc ( sizeof( Queue ) ) ;
    newQueue( waitingIOQueue, high ) ;

    puts("> The waiting for I/O queue was inicialized.");

    readAllProcessess( argv[1] , &numProcesses , programs );

    puts("> The file was read and the data was stored.");
    printf("> There is a total of %d processes.\n", numProcesses);

    signal( SIGTTIN , sigttinHandler ) ;
    signal( SIGUSR1 , sigusr1Handler ) ;
    signal( SIGALRM , sigalrmHandler ) ;
    
    for (i = 0; i < numProcesses; i++)
    {
        
        puts("> Creating a child process.");

        if((pid = fork()) == 0) // Child code
        {
            char * argv[50];
            int j;

            Process ini = createNewProcessId();

            printf("> Putting in the queue [pid: %d]\n", ini.pid);
            
            pointerToSharedMemory = (Queue *) shmat (shmid, 0, 0);
            
            enQueue(pointerToSharedMemory, ini);

            shmdt (pointerToSharedMemory);
            
            argv[0] = programs[i].executable;

            for ( j = 1; j <= programs[i].numCpuBounds; j++)
            {
                argv[j] = programs[i].cpuBound[j - 1];
            }
            
            argv[j] = '\0';
            
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
     
    puts("     _____ __             __ ");
    puts("    / ___// /_____ ______/ /_");
    puts("    \\__ \\/ __/ __ `/ ___/ __/");
    puts("   ___/ / /_/ /_/ / /  / /_  ");
    puts("  /____/\\__/\\__,_/_/   \\__/");
    
    puts("\t   ____    __          __     __     "); 
    puts("\t  / __/___/ /  ___ ___/ /_ __/ /__ ____");
    puts("\t _\\ \\/ __/ _ \\/ -_) _  / // / / -_) __/");
    puts("\t/___/\\__/_//_/\\__/\\_,_/\\_,_/_/\\__/_/   ");
                                    
    puts("");                  
    
    curr = getFront( priorityQueues[0] ) ;
    curr.state = RunningState ;
    priorityOfCurrentProcess = high ;
    quantuns = 1 ;
    kill( curr.pid , SIGCONT ) ;

    for(EVER)
    {
        alarm(QUANTUM);
        pause();

        if (allEmpty())
        {
            break;
        }
    }

    puts("---------------------------------------------------------------------------------------");
    puts("\t\t\t\t\tEnd scheduler.");

    free(waitingIOQueue);
    shmdt(pointerToSharedMemory);
    shmctl(shmid, IPC_RMID, 0);

    return 0;
}
