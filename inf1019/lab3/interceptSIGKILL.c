#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	// ignorar SIGKILL 
    if (signal(SIGKILL, SIG_IGN) == SIG_ERR){
    	puts("Não é possível ignorar o sinal SIGKILL!");
    }

    if (signal(SIGINT, SIG_IGN) == SIG_ERR){
    	puts("Não foi possível ignorar SIGINT!");
    } else {
    	puts("Este processo foi configurado para ignorar o SIGINT!");
    }

    //kill(getpid(), SIGKILL);
    //raise(SIGKILL);

    sleep(5);
    puts("O processo dormiu por 5 segundos");

    return 0;
}