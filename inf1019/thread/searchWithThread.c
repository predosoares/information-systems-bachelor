#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define NUM_THREADS 5
#define SIZE 200

int bigger = -1;
int threadThatFound;

typedef struct array
{
    int * array;
    int lenght;

} tpArray;

tpArray * listOfNumbers = NULL;

void * search(void * threadID)
{
    int threadId, interval;

    threadId = *((int *)threadID);
    interval = SIZE / NUM_THREADS;
    
    for (int i = threadId*interval; i < (1 + threadId)*interval; i++)
    {
        if (listOfNumbers->array[i] > bigger)
        {
            bigger = listOfNumbers->array[i];
            threadThatFound = threadId;
        }
    }

    pthread_exit(NULL); /* not necessary */
}

void iniciar_aleatorio(void)
{
    srand((unsigned int)time(NULL));
    rand();
    rand();
    rand();
}

int aleatorio(int a, int b)
{
    double r;
    r = (double) rand()/RAND_MAX;
    return (int)(a + r*(b-a));
}

void inicializeArray()
{
    listOfNumbers = (tpArray * ) malloc (sizeof(tpArray));

     if (listOfNumbers == NULL)
    {
        puts("Erro de memória!");
        exit(-1);
    }

    listOfNumbers->array = (int *) malloc (SIZE * sizeof(int));

    if (listOfNumbers->array == NULL)
    {
        puts("Erro de memória!");
        exit(-1);
    }

    listOfNumbers->lenght = SIZE;
       
    for (int i = 0; i < listOfNumbers->lenght; i++)
    {
        listOfNumbers->array[i] = aleatorio(0, 10000);
    }
}


int main(int argc, char * argv[])
{
    pthread_t threads[NUM_THREADS];

    int * t = (int *) malloc (NUM_THREADS*sizeof(int));
    int i;

    iniciar_aleatorio();
    inicializeArray();
    
    for (i = 0 ; i < NUM_THREADS ; i++)
    {
        *t = i;
        printf("Creating thread %d\n", *t);
        pthread_create(&threads[*t], NULL, search, (void *)t);
        t++;
    }
    
    for(i = 0 ; i < NUM_THREADS ; i++)
    {
        pthread_join(threads[i],NULL); /* wait for all the threads to terminate */
    }

    printf("Thread that found: %d - Bigger number: %d\n", threadThatFound, bigger);
    return 0;
}