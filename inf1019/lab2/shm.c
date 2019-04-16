#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char *argv[])
{
    int segmento, *p, id, pid, status;
    // aloca a memória compartilhada
    segmento = shmget (IPC_PRIVATE, sizeof (int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    
    //tratamento de erro
    if (segmento == -1)
    {
        puts ("Erro na alocação de novo espaço de memória compartilhada");
        exit (-2);
    }
    
    // associa a memória compartilhada ao processo
    p = (int *) shmat (segmento, 0, 0); // comparar o retorno com -1
    *p = 8752;
    
    //tratamento de erro da chamada fork
    if ((id = fork()) < 0)
    {

        puts ("Erro na criação do novo processo");
        exit (-2);

    } else if (id == 0)
    {
        for (int i = 0; i < 10; i++) {
            *p += 3;
            printf ("Processo filho = %d\n", *p);
        }
    } else 
    {
        for (int i = 0; i < 10; i++) {
            *p += 10;
            printf ("Processo pai = %d\n", *p);
        }
        //pid = wait (&status);
    }
    // libera a memória compartilhada do processo
    shmdt (p);
    // libera a memória compartilhada
    shmctl (segmento, IPC_RMID, 0);
    return 0;
}
