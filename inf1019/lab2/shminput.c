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
    // aloca a memória compartilhada (chave de identificação 8765)
    segmento = shmget (8765, SIZEOFSTRING*(sizeof (char)), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    
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
    
    printf("Digite uma mensagem: ");
    scanf("%s", &mensagem);
    strcpy(p, mensagem);
    
    // libera a memória a anexação com o processo compartilhado
    shmdt (p);
    
    return 0;
}
