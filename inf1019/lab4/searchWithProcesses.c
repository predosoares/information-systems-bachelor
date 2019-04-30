#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define SHMKEY 9876
#define SIZE 16000
#define NUM_PROCESSES 20

typedef struct array
{
    int array[SIZE];
    int bigger[NUM_PROCESSES];
    int smallest[NUM_PROCESSES];

} tpArray;

void searchInBlocks(tpArray *pStruct, int sector)
{
    int count = (SIZE/NUM_PROCESSES);
    int ini = count*sector;

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
    clock_t ticks[2];
    double takenTime;
    int status,
        shmid,
        bigger,
        smallest,
        processThatFoundBigger,
        processThatFoundSmallest;
    tpArray *pStruct;
    
    
    
    ticks[0] = clock();
    
    for (int i = 0; i < NUM_PROCESSES; i++) {
        if(fork() == 0) 
        { 
            printf ("[son] pid %d from [parent] pid %d\n", getpid(), getppid()); 
            
            shmid = shmget (SHMKEY,(20 + SIZE)*(sizeof (int)), S_IRUSR );
            
            pStruct = (tpArray *) shmat (shmid, 0, 0);
            searchInBlocks (pStruct, i);
            
            shmdt (pStruct);
            exit(0);
        }
    }
    
    waitpid (-1, &status, 0);
    
    shmid = shmget (SHMKEY,(20 + SIZE)*(sizeof (int)), S_IRUSR );
    pStruct = (tpArray *) shmat (shmid, 0, 0);
            
    bigger = pStruct->bigger[0];
    smallest = pStruct->smallest[0];
            
    for (int i = 1; i < NUM_PROCESSES; i++)
    {
        if ( pStruct->smallest[i] < smallest )
        {
            smallest = pStruct->smallest[i];
            processThatFoundSmallest = i;
        }
        if ( pStruct->bigger[i] > bigger )
        {
            bigger = pStruct->bigger[i];
            processThatFoundBigger = i;
        }
    }
 
    shmdt (pStruct);
            
    ticks[1] = clock();
            
    takenTime = (double) (ticks[1] - ticks[0]) / CLOCKS_PER_SEC;
    
    printf("Taken time: %f sec -----------------------------------------------\n", takenTime);
    printf("Process that found the bigger integer: %d - Bigger number: %d\n", processThatFoundBigger, bigger);
    printf("Process that found the smallest integer: %d - Smallest number: %d\n", processThatFoundSmallest, smallest);
    puts("-----------------------------------------------------------------------");
            
    // libera a memória compartilhada
    shmctl (shmid, IPC_RMID, 0);
    return 0;
}
