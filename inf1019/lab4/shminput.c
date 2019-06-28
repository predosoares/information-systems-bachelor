#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define SHMKEY 9876
#define SIZE 16000
#define NUM_PROCESSES 50

void iniciar_aleatorio(void)
{
    srand((unsigned int)time(NULL));
    rand();
    rand();
    rand();
}

unsigned int aleatorio(unsigned int a, unsigned int b)
{
    double r;
    r = (double) rand()/RAND_MAX;
    return (unsigned int)(a + r*(b-a));
}

int main (int argc, char *argv[])
{
    int segmento;
    int *p;
    int extra = 3 * NUM_PROCESSES;
    
    iniciar_aleatorio();
    
    // aloca a memória compartilhada (chave de identificação)
    segmento = shmget (SHMKEY, (extra + SIZE)*(sizeof (int)), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    
    //tratamento de erro para chamada shmget
    if (segmento == -1)
    {
        puts ("Erro na alocação de novo espaço de memória cosmpartilhada");
        exit (-2);
    }
    
    // associa a memória compartilhada ao processo
    p = (int *) shmat (segmento, 0, 0); 
    
    //tratamento da chamada shmat
    if (p == (int*)-1)
    {
        puts ("Erro na anexação da memória ao processo");
        exit (-2);
    }
    
    for (int i = 0; i < SIZE; i++)
    {
        p[i] = aleatorio(0, UINT_MAX);
    }
    
    for (int i = SIZE; i < SIZE + extra; i++)
    {
        if (i < SIZE + (extra*2)/3 )
        {
            p[i] = p[0];
        } else
        {
            p[i] = -1;
        }
    }
    
    // desanexa a memória compartilhado ao processo
    shmdt (p);
    
    return 0;
}
