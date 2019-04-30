#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_THREADS 2

int i;

void *contCrescente(void *threadid)
{
    for (i = 1; i <= 50; i++)
    {
        printf("> %d\n", i);
        sleep(1);
    }
    /* do other things */
    pthread_exit(NULL); /*not necessary*/

}

void *contDecrescente(void *threadid)
{
    for (i = 30; i >= 1; i--)
    {
        printf("\t\t> %d\n", i);
        sleep(2);
    }
    pthread_exit(NULL); /*not necessary*/

}

int main()
{
    pthread_t threads[NUM_THREADS];
    int t;
    
    printf("Crescente\tDecrescente\n");
    pthread_create(&threads[t], NULL, contCrescente, (void *)t);
    pthread_create(&threads[t], NULL, contDecrescente, (void *)t);
    
    for(t=0; t < NUM_THREADS; t++)
    {
        pthread_join(threads[t],NULL); /* wait for all the threads to terminate*/
    }
}
