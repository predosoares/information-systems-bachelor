Jonny Russo - 1610608
Pedro Henrique Soares - 1713186

*************************************************************************************************************

QUESTÃO 1) O que acontece se alterarmos o local da
chamada waitpid() no programa (para depois
da printf).

*************************************************************************************************************

CÓDIGO-FONTE

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

    }
    
    else if (id == 0)
    {
        for (int i = 0; i < 5; i++) {
            *p += 5;
        }
        printf ("Processo filho = %d\n", *p);

    }
    else
    {

        for (int i = 0; i < 5; i++) {
            *p += 10;
        }
        printf ("Processo pai = %d\n", *p);
        pid = wait (&status);

    }
    // libera a memória compartilhada do processo
    shmdt (p);
    // libera a memória compartilhada
    shmctl (segmento, IPC_RMID, 0);
    return 0;
}


*************************************************************************************************************

COMPILAÇÃO E EXECUÇÃO

$ gcc -o shm shm.c
$ ./shm

*************************************************************************************************************

RESULTADO

Processo pai = 8802
Processo filho = 8827


*************************************************************************************************************

CONCLUSÃO

Com a memória sendo compartilhada entre o processo pai e o processo fiho é 
possível visualizar como os dois iteram o valor simultaneamente no espaço 
de memória compartilhado alocado.

*************************************************************************************************************



QUESTÃO 2) Faça um programa que:
	> Leia uma mensagem do dia
	> Crie uma memória compartilhada com a chave 8765
	> Salve a mensagem na memória

	> Faça um outro programa que utilize a
	mesma chave (8765) e exiba a mensagem
	do dia para o usuário

*************************************************************************************************************

CÓDIGO-FONTE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define SHMKEY 8765
#define SIZEOFSTRING 30

int main (int argc, char *argv[])
{
    int segmento;
    char mensagem[SIZEOFSTRING];
    char *p;
    
    // aloca a memória compartilhada (chave de identificação 8765)
    segmento = shmget (SHMKEY, SIZEOFSTRING*(sizeof (char)), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    
    // tratamento de erro para chamada shmget
    if (segmento == -1)
    {
        puts ("Erro na alocação de novo espaço de memória compartilhada");
        exit (-2);
    }
    
    // associa a memória compartilhada ao processo
    p = (char *) shmat (segmento, 0, 0); // comparar o retorno com -1
    
    // tratamento de erro da chamada shmat
    if (p == (char*)-1)
    {
        puts ("Erro na anexação da memória ao processo");
        exit (-2);
    }
    
    printf ("Digite uma mensagem: ");
    scanf ("%s", &mensagem);
    strcpy (p, mensagem);
    
    // desassocia a memória compartilhada ao processo
    shmdt (p);
    
    return 0;
}


*************************************************************************************************************

AUXILIAR

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define SHMKEY 8765
#define SIZEOFSTRING 30

int main (int argc, char *argv[])
{
    int segmento;
    char mensagem[SIZEOFSTRING];
    char *p;
    
    // obtem o shmid no modo leitura para a chave 8765
    segmento = shmget (SHMKEY, SIZEOFSTRING*(sizeof (char)), S_IRUSR );
    
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
    
    // desassocia a memória compartilhada ao processo
    shmdt (p);
    // libera a memória compartilhada
    shmctl (segmento, IPC_RMID, 0);
    return 0;
}


*************************************************************************************************************

COMPILAÇÃO E EXECUÇÃO

$ gcc -o shminput shminput.c
$ gcc -o shmoutput shmoutput.c
$ ./shminput
$ Digite uma mensagem: alomundo <~ colocar texto que deseja gravar na área de até 30 bytes
$ ./shmoutput 

*************************************************************************************************************

RESULTADO

Mensagem alocada: alomundo

*************************************************************************************************************

CONCLUSÃO

Com as chamadas de compartilhamento de memória (shmget, shmat, shmdt, shmctl) é possível escrever dados num 
espaço de memória associado a uma chave de identificação. A partir da chave é possível por outros processos 
realizar acessos ao espaço de memória manipulado anteriormente.

*************************************************************************************************************


QUESTÃO 3) Faça um programa paralelo para localizar
uma chave em um vetor.
	
	> Crie uma memória compartilhada com dados
	desordenados e a divida pelo número de
	processos
	
	> Cada processo deve procurar o dado na sua área
	de memória e informar a posição onde o dado foi
	localizado.

*************************************************************************************************************

CÓDIGO-FONTE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define SHMKEY 8765
#define SIZEARRAY 30
#define NUMBEROFSONPROCESSES 5

void searchInBlocks(char *ptBlock, int sector, char key)
{
    int count = (SIZEARRAY/NUMBEROFSONPROCESSES);
    int ini = count*sector;

    for (int i = ini; i < count + ini; ++i)
    {
        if (ptBlock[i] == key)
        {
            printf ("We found %d at the sector: %d in the position: %d of the array\n", key, sector, i);
            shmdt (ptBlock);
            exit (0);
        }
    }
}


int main(int argc, char* argv[])
{
    int status, shmid;
    char key;
    char *ptBlock;

    if (argc != 2)
    {
        puts ("execution guide: { ./shmsearch [key] } <~ key to be found");
        exit (-1);
    }

    key = argv[1][0];
    
    for (int i = 0; i < NUMBEROFSONPROCESSES; i++) {
        if(fork() == 0) 
        { 
            printf ("[son] pid %d from [parent] pid %d\n", getpid(), getppid()); 
            
            shmid = shmget (SHMKEY, SIZEARRAY*(sizeof (char)), S_IRUSR );
            
            ptBlock = (char *) shmat (shmid, 0, 0);
            searchInBlocks (ptBlock, i, key);
            printf ("The key was not found at the sector: %d\n", i);
            
            shmdt (ptBlock);
            exit(0);
        }
        else
        {
            wait (&status);
            // libera a memória compartilhada
            shmctl (shmid, IPC_RMID, 0);
        }
    }
    return 0;
}


*************************************************************************************************************

AUXILIAR

*************************************************************************************************************

COMPILAÇÃO E EXECUÇÃO

$ gcc -o shmsearch shmsearch.c
$ ./shminput
$ Digite uma mensagem: aaaaaaaabaaaaaaaaaaaaaaaaaaaaa
$ ./shmsearch b

*************************************************************************************************************

RESULTADO

[son] pid 15017 from [parent] pid 15016
The key was not found at the sector: 0
[son] pid 15018 from [parent] pid 15016
We found 98 at the sector: 1 in the position: 8 of the array
[son] pid 15019 from [parent] pid 15016
The key was not found at the sector: 2
[son] pid 15020 from [parent] pid 15016
The key was not found at the sector: 3
[son] pid 15021 from [parent] pid 15016
The key was not found at the sector: 4


*************************************************************************************************************

CONCLUSÃO

A partir da execução do shminput escrevemos os dados no vetor que será lido no processo de busca em paralelo. 

************************************************************************************************************************************************************************************************************************

