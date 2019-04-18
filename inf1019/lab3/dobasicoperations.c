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