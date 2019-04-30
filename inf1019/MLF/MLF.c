#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include "queue.h"

// FILA nivel1
// FILA nivel2
// FILA nivel3

void sigttinHandler(int signal)
{

    sleep(3);
    /*
    pega o processo w4IO e espera 3 segundos para alocalo numa fila
    de maior prioridade
    */
}

void sigchldHandler(int signal)
{
    /*remove processo da fila e envia sinal para o próximo executar*/
}

int main(int argc, char* argv[])
{    
        
       
    if (argc != 2)
    {
        puts("Error");
        exit(-1);
    }
    
    FILE * p = fopen(argv[1], 'r');
    
    /*
    contar o número de linhas do arquivo de leitura
    declarar um vetor com o nome dos executaveis e os parametros passados
    */
    
	signal(SIGSTOP, sigstopHandler);
	signal(SIGCONT, sigcontHandler);
	signal(SIGTTIN, sigttinHandler);
	signal(SIGCHLD, sigchldHandler);

     for (int i = 0; i < numProcesses; i++)
     {
        if(fork() == 0)
        {
            /*adicionar ao primeiro nível da fila*/
            /*
            argv[1] = primeira rajada
            argv[2] = segunda rajada
            argv[3] = terceira rajada
            */
            execv(/*nome do executável*/"./exec",argv);
            exit(0);
        }
    }
    	

	return 0;
}
