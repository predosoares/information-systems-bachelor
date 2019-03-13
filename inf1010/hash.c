#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define NUM 1000 /* número de placas */
#define SIZE_HASH 1300 /* tamanho da tabela de hash */
#define PRIME 1301  /* números primos que geram uma boa aleatoriedade: 127, 163, 193, 347 */
#define SIZE_STRING 8 /* tamanho da string de placa */
#define EMPTY -1 /* marcador de posição vazia */
#define ATTEMPTS 10000 /* vezes em que o bloco de instruções será realizado */

char hash_table_1[SIZE_HASH][SIZE_STRING];
char hash_table_2[SIZE_HASH][SIZE_STRING];
char hash_table_3[SIZE_HASH][SIZE_STRING];

long colisions_1 = 0;
long colisions_2 = 0;
long colisions_3 = 0;

void inicialize_random(void){
    srand((unsigned int)time(NULL));
    rand();
    rand();
    rand();
}

double media(double *v){
	double m = 0;
	int i;
	for(i = 0; i < ATTEMPTS; i++ )
		m += v[i];
	return m/ATTEMPTS;
}

void writePlate(FILE *pf){
    fprintf(pf,"%c%c%c%c%c%c%c\n", 'A'+rand()%25, 'A'+rand()%25, 'A'+rand()%25, '0'+rand()%10, '0'+rand()%10, '0'+rand()%10, '0'+rand()%10);
}

void inicialize_hash_table(char hash_table[][8]){
    int i;
    for ( i = 0; i < SIZE_HASH; i++)
        hash_table[i][0] = EMPTY;
}

void writeFile(FILE *pf){
 	int i;
 	for (i = 0; i < NUM; i++)
 		 writePlate(pf);
}

int hash_modular(char* key){
    unsigned long hashCode = 0;
 	while(*key){
  		hashCode = hashCode + *key;
  		key++;
 	}
    return (hashCode%SIZE_HASH);
}

int hash_mid_square(char* key){
 	unsigned long hashCode = 0;
 	while(*key){
  		hashCode = hashCode + *key;
  		key++;
 	}
 	hashCode = hashCode*hashCode;
 	hashCode = hashCode/10;

 	return (hashCode%SIZE_HASH);
}

int hash_folding(char* key){
 	unsigned long hashCode = 0;
 	unsigned char aux;
	while(*key){
		if (*key % 2 == 0){
			aux = *key / 10;
			*key = (*key % 10) * 10 ;
			*key = *key + aux;
		}
  		hashCode = hashCode + *key;
  		key++;
 	}
 	return (hashCode%SIZE_HASH);
}

void insert(int hashCode, char* key, char hash_table[][8], long* colisions){
    while (*hash_table[hashCode] != EMPTY){
        if (strcmp(hash_table[hashCode], key) == 0 )
            break;
        *colisions += 1;
        hashCode = (hashCode + 1) % SIZE_HASH;
    }
    if (*hash_table[hashCode] == EMPTY){
        strcpy(hash_table[hashCode], key);
    }
}

int main(void){
    FILE *pfile;
    clock_t ticks[2];
    double tries[ATTEMPTS];
    double time_took_1, time_took_2, time_took_3;
    char plate[8];
    int i, j;
    
	inicialize_random();
	
    
    
    /***************************TESTE-1**************************/
    for(j = 0; j < ATTEMPTS; j++){
    	pfile = fopen("placas.txt", "w");
    	if (pfile == NULL){
        	puts("Erro na abertura de pfile!");
        	return -1;
    	}
		writeFile(pfile);
        
    	fclose(pfile);
    	pfile = fopen("placas.txt","r");
    	inicialize_hash_table(hash_table_1);
    	ticks[0] = clock();
    	for(i = 0; i < NUM; i++){
    	    fscanf(pfile, "%s\n", plate);
    	    insert(hash_modular(plate), plate, hash_table_1, &colisions_1);
    	}
    	ticks[1] = clock();
    	tries[j] = (ticks[1] - ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
    	fclose(pfile);
    }
    time_took_1 = media(tries);
    colisions_1 = colisions_1 / ATTEMPTS;
    /***************************TESTE-2**************************/
    for(j = 0; j < ATTEMPTS; j++){
    	pfile = fopen("placas.txt", "w");
    	if (pfile == NULL){
        	puts("Erro na abertura de pfile!");
        	return -1;
    	}
		writeFile(pfile);
        
    	fclose(pfile);
    	pfile = fopen("placas.txt","r");
    	inicialize_hash_table(hash_table_2);
    	ticks[0] = clock();
    	for(i = 0; i < NUM; i++){
    	    fscanf(pfile, "%s\n", plate);
    	    insert(hash_mid_square(plate), plate, hash_table_2, &colisions_2);
    	}
    	ticks[1] = clock();
    	tries[j] = (ticks[1] - ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
    	fclose(pfile);
    }
    time_took_2 = media(tries);
    colisions_2 = colisions_2 / ATTEMPTS;
    /***************************TESTE-3**************************/
    for(j = 0; j < ATTEMPTS; j++){
    	pfile = fopen("placas.txt", "w");
    	if (pfile == NULL){
        	puts("Erro na abertura de pfile!");
        	return -1;
    	}
		writeFile(pfile);
        fclose(pfile);
    	pfile = fopen("placas.txt","r");
    	inicialize_hash_table(hash_table_3);
    	ticks[0] = clock();
    	for(i = 0; i < NUM; i++){
    	    fscanf(pfile, "%s\n", plate);
    	    insert(hash_mid_square(plate), plate, hash_table_3, &colisions_3);
    	}
    	ticks[1] = clock();
    	tries[j] = (ticks[1] - ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
    	fclose(pfile);
    }
    time_took_3 = media(tries);
    colisions_3 = colisions_3 / ATTEMPTS;
    /************************************************************/
    
    puts("---------------------------------------------------------------");
    printf("Tempo estimado para hash modular: %g ms - colisões: %ld\n", time_took_1, colisions_1);
    printf("Tempo estimado para hash mid-square: %g ms - colisões: %ld\n", time_took_2, colisions_2);
    printf("Tempo estimado para hash folding: %g ms - colisões: %ld\n", time_took_3, colisions_3);
    puts("---------------------------------------------------------------");
    
    return 0;
}
