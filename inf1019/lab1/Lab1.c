// Lab 1 (Fork e Exec) - inf1019 - Sistemas de Computação

// Exer 1 - prática de fork

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define NUMBER 100

char * frase;

int main(int argc, char* argv[])
{
    int i;
    int status,
        shmid;

    shmid = shmget(NUMBER, 3 * sizeof(char), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
    frase = (char *) shmat (shmid, 0, 0);
    frase[0] = 'O';
    frase[1] = 'K';
    frase[2] = '!';

    if ( fork() != 0 ){ /* Main process */
        /* Parent code */
        for ( i = 0; i < 10; i++) {
            printf("PID pai: %d\t\t\tContagem: %d\n", getpid(), i);
            sleep(1);
        }
        waitpid(-1, &status, 0);
        puts("Processo pai finalizado");

    } else {
        if ( fork() != 0 ) {
            /* Child code */
            puts(frase);
            puts("Filho foi criado");
            for ( i = 20; i < NUMBER/2; i++) {
                printf("PID pai: %d\tPID filho: %d\tContagem: %d\n", getppid(), getpid(), i);
                sleep(2);
            }
            waitpid(-1, &status, 0);
            puts("Processo filho vai finalizar");

        } else {
            /* Grandson code */
            puts(frase);
            puts("Neto foi criado");
            for ( i = 50; i < NUMBER-30; i++) {
                printf("PID pai: %d\tPID neto: %d\tContagem: %d\n", getppid(), getpid(), i);
                sleep(3);
            }
            puts("Processo neto vai finalizar");
        }
    }
    shmdt (frase);
    shmctl (shmid, IPC_RMID, 0);
    return 0;
}




