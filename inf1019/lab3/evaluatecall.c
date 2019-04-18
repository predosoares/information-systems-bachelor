#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

#define EVER ;;

void call(int signal);
clock_t startTime;

int main(int argc, char const *argv[])
{
	puts("INSTRUCTIONS------------------------------");
	puts("\t> To start a call: kill -s SIGUSR1 pid");
	puts("\t> To end a call: kill -s SIGUSR2 pid");
	puts("--------------------------------------------");

	signal(SIGUSR1, call);
	signal(SIGUSR2, call);

	for(EVER);

	return 0;
}

void call(int signal)
{
	int takenTime;
	float value;

	switch(signal)
	{
		case SIGUSR1 :
			startTime = clock();
			puts("> You are on a call.");
			break;

		case SIGUSR2 :
			value = 0.0;
			takenTime = (int) (clock() - startTime) / CLOCKS_PER_SEC;

			value = takenTime <= 60 ? takenTime * 0.02 : 1.20 + ((takenTime - 60) * 0.01);

			puts("> The call is over.");
			printf("> Your call costed R$ %.2f and lasted for %d seconds.\n", value, takenTime);
	}
}