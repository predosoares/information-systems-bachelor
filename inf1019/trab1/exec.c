#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main (int argc, char * argv[])
{
	int cpuBound;

    printf("%d\n", argc);

    for (int i = 1; i < argc - 1; i++)
    {
        cpuBound = atoi(argv[i]);

        //simulates CPU bound
        for (int j = 1; j <= cpuBound; j++)
        {
    	    printf("> executing process: %d, seconds: %d/%d\n", getpid(), j, cpuBound);
            sleep(1);
        }

        if (i != argc - 1)
        {
            //simulates I/O bound
            kill(getppid(), SIGTTIN);
            raise(SIGSTOP);
        }
    }

    //send to the scheduler a signal, means the end of the process 
    kill(getppid(), SIGUSR1);
    
    return 0;
}
