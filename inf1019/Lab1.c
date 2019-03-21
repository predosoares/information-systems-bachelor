// Lab 1 (Fork e Exec) - inf1019 - Sistemas de Computação

// Exer 1 - prática de fork

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUMBER 100

int main(int argc, char* argv[]){
    int i;
    int status;

    if ( (fork()) != 0 ){ /* Main process */
        /* Parent code */
        for ( i = 0; i < NUMBER; i++) {
        printf("PID pai: %d\tContagem: %d\n", getpid(), i);
        sleep(1);
        }
        waitpid(-1, &status, 0);
        puts("Processo pai finalizado");

    } else {
        if ( (fork()) != 0 ) {
            /* Child code */
            puts("Filho foi criado");
            for ( i = 100; i < NUMBER*2; i++) {
                printf("PID pai: %d\tPID filho: %d\tContagem: %d\n", getppid(), getpid(), i);
                sleep(2);
            }
            waitpid(-1, &status, 0);
            puts("Processo filho vai finalizar");

        } else {
            /* Grandson code */
            puts("Neto foi criado");
            for ( i = 200; i < NUMBER*3; i++) {
                printf("PID pai: %d\tPID neto: %d\tContagem: %d\n", getppid(), getpid(), i);
                sleep(3);
            }
            puts("Processo neto vai finalizar");
        }
    }
}

// Exer 2 - prática de exec
