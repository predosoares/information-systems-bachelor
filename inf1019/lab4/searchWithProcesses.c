#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define SHMKEY 9876
#define SIZE 1000
#define NUM_PROCESSES 5

void searchInBlocks(int *ptBlock, int sector, int key)
{
    int count = (SIZE/NUM_PROCESSES);
    int ini = count*sector;

    for (int i = ini; i < count + ini; i++)
    {
        if (ptBlock[i] == key)
        {
            printf ("We found %d at the sector: %d in the position: %d of the array\n", key, sector, i);
            exit (0);
        }
    }
}


int main(int argc, char* argv[])
{
    int status,
        shmid,
        key,
        *ptBlock;

    if (argc != 2)
    {
        puts ("execution guide: { ./shmsearch [key] } <~ key to be found");
        exit (-1);
    }

    key = atol(argv[1]);
    
    for (int i = 0; i < NUM_PROCESSES; i++) {
        if(fork() == 0) 
        { 
            printf ("[son] pid %d from [parent] pid %d\n", getpid(), getppid()); 
            
            shmid = shmget (SHMKEY, SIZE*(sizeof (int)), S_IRUSR );
            
            ptBlock = (int *) shmat (shmid, 0, 0);
            searchInBlocks (ptBlock, i, key);
            printf ("The key was not found at the sector: %d\n", i);
            
            shmdt (ptBlock);
            exit(0);
        }
        else 
        {
            wait (&status);
            // libera a memória compartilhada
            shmctl (shmid, IPC_RMID, 0);
        }
    }
    return 0;
}
