#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

// FILA nivel1
// FILA nivel2
// FILA nivel3

int main(int argc, char* argv[])
{
	signal(SIGSTOP, );
	signal(SIGCONT, );
	signal(SIGTTIN, );
	signal(SIGCHLD, );

	

	return 0;
}