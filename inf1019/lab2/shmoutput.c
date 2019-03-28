#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define SIZEOFSTRING 30

int main (int argc, char *argv[])
{
    int segmento;
    char mensagem[SIZEOFSTRING];
    char *p;
    // obtem o shmid no modo leitura para a chave 8765
    segmento = shmget (8765, SIZEOFSTRING*(sizeof (char)), S_IRUSR );
    
    //tratamento de erro para chamada shmget
    if (segmento == -1)
    {
        puts ("Erro na alocação de novo espaço de memória compartilhada");
        exit (-2);
    }
    
    // associa a memória compartilhada ao processo
    p = (char *) shmat (segmento, 0, 0); // comparar o retorno com -1
    
    //tratamento da chamada shmat
    if (p == (char*)-1)
    {
        puts ("Erro na anexação da memória ao processo");
        exit (-2);
    }
    
    strcpy(mensagem, p);
    printf("Mensagem alocada: %s\n", mensagem);
    
    // libera a memória a anexação com o processo compartilhado
    shmdt (p);
    // libera a memória compartilhada
    shmctl (segmento, IPC_RMID, 0);
    return 0;
}
