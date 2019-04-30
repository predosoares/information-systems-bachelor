#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main (int argc, char * argv[])
{
	if (argc != 4)
	{
		puts("Error this program must me executed with 4 parameters only!");
		puts("Try this: ./exec <time in sec> <time in sec> <time in sec>");
		exit(-1);
	}

	int cpuBound1 = atol(argv[1]),
		cpuBound2 = atol(argv[2]),
		cpuBound3 = atol(argv[3]);

	//simulates CPU bound
    for (int i = 0; i < cpuBound1; i++)
    {
    	printf("%d\n", getpid());
    	sleep(1);
    }

    //simulates I/O bound
    kill(getppid(), SIGTTIN);

    //simulates CPU bound
    for (int i = 0; i < cpuBound2; i++)
    {
    	printf("%d\n", getpid());
    	sleep(1);
    }

    //simulates I/O bound
    kill(getppid(), SIGTTIN);

    //simulates CPU bound
    for (int i = 0; i < cpuBound3; i++)
    {
    	printf("%d\n", getpid());
    	sleep(1);
    }
    
    return 0;
}