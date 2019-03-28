#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define KEY 8765
#define SIZEARRAY 20

int main(int argc, char* argv[])
{
    int status;
    
    for (int i = 0; i < 4; i++) {
        if(fork() == 0) 
        { 
            printf("[son] pid %d from [parent] pid %d\n", getpid(), getppid()); 
            
            shmid = shmget (KEY, SIZEARRAY*(sizeof (char)), S_IRUSR );
            
            p = shmat (shmid, i*4, 0);
            
            
            
            exit(0);
        }
    }
    return 0;
}
