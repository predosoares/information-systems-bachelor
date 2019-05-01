# Laboratório 3 - Sinais do SO

---

>QUESTÃO 1) Explique o funcionamento do programa 
filhocídio.c

---

### CÓDIGO-FONTE

----

filhocidio:

```
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#define EVER ;;

void childhandler(int signo);
int delay;

int main(int argc, char * argv[])
{
    pid_t pid;

    signal(SIGCHLD, childhandler);
    
    if ((pid = fork()) < 0)
    {
        fprintf(stderr, "Erro ao criar filho\n");
        exit(-1);
    }
    
    if (pid == 0) /* child */
        for(EVER); /* or sleep(3) */
    else /* parent */
    {
        sscanf(argv[1], "%d", &delay); /* read delay from command line */
        sleep(delay);
        printf("Program %s exceeded limit of %d seconds!\n", argv[2], delay);
        kill(pid, SIGKILL);
    }
    return 0;
}


void childhandler(int signo) /* Executed if child dies before parent */
{
    int status;
    pid_t pid = wait(&status);
    printf("Child %d terminated within %d seconds com estado %d.\n", pid, delay, status);
    exit(0);
}
```

---

### COMPILAÇÃO E EXECUÇÃO

---

>$ gcc -o filhocidio filhocidio.c
>$ ./filhocidio <tempo (em segundos) que o processo pai vai esperar para enviar um sinal p/ matar o processo filho> <nome do processo filho>

>exemplo:
$ ./filhocidio 4 filho 

---

### RESULTADO

---

Program filho exceeded limit of 4 seconds!

---

### CONCLUSÃO

---

Ao ser executado o programa irá criar uma cópia do processo através da chamada fork, que será executado por um
tempo indefinido já que o bloco de código que será executado é um loop infinito: "for(;;)". Enquanto isso o 
processo pai lerá um número definido na execução que corresponderá ao tempo que o processo pai irá dormir, até 
que acorde e execute a chamada kill, que irá enviar um sinal SIGKILL para o processo filho, forçando o fim do processo.

---

>QUESTÃO 2) Altere o programa filhocidio.c para que o 
filho, em vez de executar o for(EVER) dê um 
exec() no programa sleep5 e depois no 
programa sleep15 indicados a seguir. 
Execute cada um e explique o funcionamento 
desta nova versão do programa filhocidio.c

---

### CÓDIGO-FONTE

---

filhocidio.c:

```
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#define EVER ;;

void childhandler(int signo);
int delay;

int main(int argc, char * argv[])
{
    pid_t pid;

    signal(SIGCHLD, childhandler);
    
    if ((pid = fork()) < 0)
    {
        fprintf(stderr, "Erro ao criar filho\n");
        exit(-1);
    }
    
    if (pid == 0) /* child */
        execv("sleep5", argv); /* chama sleep15 no executável filhocidio15*/
    else /* parent */
    {
        sscanf(argv[1], "%d", &delay); /* read delay from command line */
        sleep(delay);
        printf("Program %s exceeded limit of %d seconds!\n", argv[2], delay);
        kill(pid, SIGKILL);
    }
    return 0;
}


void childhandler(int signo) /* Executed if child dies before parent */
{
    int status;
    pid_t pid = wait(&status);
    printf("Child %d terminated within %d seconds com estado %d.\n", pid, delay, status);
    exit(0);
}
```

---

AUXILIAR

---

sleep5.c:

```
#include <stdio.h>
#include <unistd.h>

int main (void)
{
    fprintf(stdout, "indo dormir...\n");
    sleep(5); 
    fprintf(stdout, "Acordei!\n");
    
    return 0;
}
```

sleep15.c:

```
#include <stdio.h>
#include <unistd.h>

int main (void)
{
    fprintf(stdout, "indo dormir...\n");
    sleep(15); 
    fprintf(stdout, "Acordei!\n");
    
    return 0;
}
```

---

### COMPILAÇÃO E EXECUÇÃO

---

>$ gcc -o sleep5 sleep5.c
$ gcc -o filhocidio5 filhocidio.c
$ gcc -o sleep15 sleep15.c
$ gcc -o filhocidio15 filhocidio.c
$ ./filhocidio <tempo (em segundos) que o processo pai vai esperar para enviar um sinal p/ matar o processo filho> <nome do processo filho>

---

### RESULTADO

---

./filhocidio5 6 filho 
indo dormir...
Acordei!
Child 11955 terminated within 6 seconds com estado 0.

./filhocidio15 6 filho
indo dormir...
Program filho exceeded limit of 6 seconds!

---

### CONCLUSÃO

---

Nessa nova versão do programa o processo filho fará uma chamada exec() que, em uma das versões, chama sleep5 
(um programa que faz o processo dormir por 5 segundos). Portanto, se ao executar o programa passarmos o valor 6
o processo filho acabará antes do processo pai, o sinal SIGCHLD será enviado ao processo pai e a função de 
tratamento ao sinal será chamda.

---

QUESTÃO 3) Tente fazer um programa para interceptar o 
sinal SIGKILL. Você conseguiu? Explique.

---

### CÓDIGO-FONTE

---

```
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    if (signal(SIGKILL, SIG_IGN) == SIG_ERR){
    	puts("Não é possível ignorar o sinal SIGKILL!");
    }

    //formas de enviar um sinal há um processo via chamadas ao sistema 
    //kill(getpid(), SIGKILL);
    //raise(SIGKILL);

    sleep(5);
    puts("O processo dormiu por 5 segundos");

    return 0;
}
```

---

### COMPILAÇÃO E EXECUÇÃO

---

>$ gcc -o interceptSIGKILL interceptSIGKILL.c
$ ./interceptSIGKILL &

---

### RESULTADO

---

```
$ ./interceptSIGKILL &                                  
[3] 13128
Não é possível ignorar o sinal SIGKILL!

$ kill -s SIGKILL 13128
[3]  + 13128 killed     ./interceptSIGKILL
```

---

### CONCLUSÃO

---

Não. O sinal SIGKILL não pode ser interrompido ou ignorado por qualquer processo.

---

QUESTÃO 4) Faça um programa que leia 2 números reais 
e imprima o resultado das 4 operações 
básicas sobre estes 2 números
	- Verifique o que acontece se o 2º. número 
	da entrada for 0 (zero)
	- Capture o sinal de erro de floating point 
	(SIGFPE) e repita a experiência anterior

---

### CÓDIGO-FONTE

---

dobasicoperations.c:

```
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void floatsignalhandler(int signo)
{
	puts("-----------------");
	puts("A floating-point exception signal was received!");
	exit(0);
}

int main(int argc, char const *argv[])
{
	int number1, number2, sum, subtrahend, product, dividend;

	if (argc != 3)
	{
		puts ("try this: ./dobasicoperations <number1> <number2>");
		return -1;
	}

	signal(SIGFPE, floatsignalhandler);

	number1 = atoll(argv[1]);
	number2 = atoll(argv[2]);

	puts("RESULTS----------");
	sum = number1 + number2;
	printf("Sum:\t\t%d\n", sum);

	subtrahend = number1 - number2;
	printf("Subtrahend:\t%d\n", subtrahend);

	product = number1 * number2;
	printf("Product:\t%d\n", product);

	dividend = number1 / number2;
	printf("Dividend:\t%d\n", dividend);
	puts("-----------------");

	return 0;
}
```

---

### COMPILAÇÃO E EXECUÇÃO

---

>gcc -o dobasicoperations dobasicoperations.c
./dobasicoperations <number1> <number2>

---

### RESULTADO

---

```
./dobasicoperations 9.321 0
RESULTS----------
Sum:		9
Subtrahend:	9
Product:	0
-----------------
A floating-point exception signal was received!
```

---

### CONCLUSÃO

---

Antes de ter desenvoldido a função para o tratamento de sinal para o sinal SIGFPE o processo finalizava com uma floating point exception.

---

QUESTÃO 5) Faça um programa para monitorar e informar o 
preço de chamadas telefônicas. O programa 
deverá ser executado em background.
	- O início e o término de uma chamada são  
	informados através dos sinais USR1 e USR2, 
	respectivamente.
	- O custo da ligação é de 2 centavos por 
	segundo, para ligações de até 1 minuto ou de 1 
	centavo por segundo a partir do 2º. minuto, ou 
	seja, uma ligação de 1m30s custa R$1,50.

---

### CÓDIGO-FONTE

---

evaluatecall.c:

```
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

#define EVER ;;

void call(int signal);
clock_t startTime;

int main(int argc, char const *argv[])
{
	puts("INSTRUCTIONS-----------------------------------------");
	puts("\t> To start a call: kill -s SIGUSR1 <process id>");
	puts("\t> To end a call: kill -s SIGUSR2 <process id>");
	puts("-----------------------------------------------------");

	//chamadas assincronas, não seguem o fluxo padrão de execução de um código
	signal(SIGUSR1, call);
	signal(SIGUSR2, call);

	for(EVER);

	return 0;
}

void call(int signal)
{
	int takenTime;
	float value;

	switch(signal)
	{
		case SIGUSR1 :
			startTime = clock();
			puts("> You are on a call.");
			break;

		case SIGUSR2 :
			value = 0.0;
			takenTime = (int) (clock() - startTime) / CLOCKS_PER_SEC;

			value = takenTime <= 60 ? takenTime * 0.02 : 1.20 + ((takenTime - 60) * 0.01);

			puts("RESULT---------------------------------------------------");
			puts("\t> The call is over.");
			printf("\t> Your call costed R$ %.2f and lasted for %d seconds.\n", value, takenTime);
			puts("---------------------------------------------------------");
	}
}
```

---

### COMPILAÇÃO E EXECUÇÃO

---

>$ gcc -o evaluatecall evaluatecall.c
$ ./evaluatecall &

---

### RESULTADO

---

```
$ ./evaluatecall &
[1] 10246
INSTRUCTIONS-----------------------------------------
	> To start a call: kill -s SIGUSR1 <process id>
	> To end a call: kill -s SIGUSR2 <process id>
-----------------------------------------------------

$ kill -s SIGUSR1 10246
-> You are on a call.

$ kill -s SIGUSR2 10246                                   
RESULT---------------------------------------------------
	> The call is over.
	> Your call costed R$ 0.12 and lasted for 6 seconds.
---------------------------------------------------------
```

---

### CONCLUSÃO

---

Para se executar processos no background é necessário executar o programa passando um "&" como parâmetro. O 
início e o fim da chamda telefônica são feitos atráves de sinais enviados via terminal, com SIGUSR1 sendo o
sinal que indica o inicio da chamada, e o SIGUSR2 sendo o final do fim. Quando o SIGUSR1 é enviado ao processo 
ele trata de calcular a diferença em segundos de quanto tempo a chamada levou e realiza o calculo do preço. 
Instruções e resultados da chamada são exibidos na tela do terminal, e enquanto não for enviado algum sinal 
para terminar o processo ele será executado (caso a sessão do terminal seja finalizada o processo será destruído).

---

