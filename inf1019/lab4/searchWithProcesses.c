#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define SHMKEY 9876
#define SIZE 16000
#define NUM_PROCESSES 50

typedef struct array
{
    unsigned int array[SIZE];
    unsigned int bigger[NUM_PROCESSES];
    unsigned int smallest[NUM_PROCESSES];
    pid_t pid[NUM_PROCESSES];

} tpArray;

void searchInBlocks(tpArray *pStruct, int sector)
{
    int count = (SIZE/NUM_PROCESSES);
    int ini = count*sector;

    pStruct->pid[sector] = getpid();

    for (int i = ini; i < count + ini; i++)
    {
        if (pStruct->bigger[sector] < pStruct->array[i])
        {
            pStruct->bigger[sector] = pStruct->array[i];
        }
        else if (pStruct->smallest[sector] > pStruct->array[i])
        {
            pStruct->smallest[sector] = pStruct->array[i];
        }
    }
}


int main(int argc, char* argv[])
{
    double ticks[2], takenTime;
    struct timeval tim;
    int status,
        shmid,
        extra,
        processThatFoundBigger,
        processThatFoundSmallest;
    unsigned int bigger,
                 smallest;
    tpArray *pStruct;
    
    extra = 3*NUM_PROCESSES;
    
    gettimeofday(&tim, NULL);
    ticks[0] = tim.tv_sec+(tim.tv_usec/1000000.0);
    
    for (int i = 0; i < NUM_PROCESSES; i++) {
        if(fork() == 0) /* Only child process will execute this */ 
        {
            shmid = shmget (SHMKEY,(extra + SIZE)*(sizeof (int)), S_IRUSR );
            pStruct = (tpArray *) shmat (shmid, 0, 0);
            searchInBlocks (pStruct, i);
            shmdt (pStruct);
            exit(0);
        }
    }

    // Wait for all process to terminate
    for (int i = 0; i < NUM_PROCESSES; i++)
    {
        waitpid (-1, &status, 0);
    }

    shmid = shmget (SHMKEY,(extra + SIZE)*(sizeof (int)), S_IRUSR );
    pStruct = (tpArray *) shmat (shmid, 0, 0);
            
    bigger = pStruct->bigger[0];
    smallest = pStruct->smallest[0];

    for (int i = 1; i < NUM_PROCESSES; i++)
    {
        if ( pStruct->smallest[i] < smallest )
        {
            smallest = pStruct->smallest[i];
            processThatFoundSmallest = pStruct->pid[i];
        }
        if ( pStruct->bigger[i] > bigger )
        {
            bigger = pStruct->bigger[i];
            processThatFoundBigger = pStruct->pid[i];
        }
    }
 
    shmdt (pStruct); // detach the allocated memory to the process
    
    gettimeofday(&tim, NULL);
    ticks[1] = tim.tv_sec+(tim.tv_usec/1000000.0);
            
    takenTime = (double) (ticks[1] - ticks[0]);
    
    printf("Taken time: %f sec ---------------------------------------------------------\n", takenTime);
    printf("Process that found the bigger integer was (pid): %d - Bigger number: %u\n", processThatFoundBigger, bigger);
    printf("Process that found the smallest integer was (pid): %d - Smallest number: %u\n", processThatFoundSmallest, smallest);
    puts("----------------------------------------------------------------------------------");
            
    // free the shared allocated memory
    shmctl (shmid, IPC_RMID, 0);
    return 0;
}
