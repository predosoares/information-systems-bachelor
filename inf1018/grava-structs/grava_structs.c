/* Pedro Henrique Aragão Soares 1713186 3WA*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grava_structs.h"
#define MAX(a,b) ((a) >= (b)? (a) : (b))

static int ehmultiplo(int n, int m){
    if (n % m == 0) return 1;
    else /* n % m != 0 */
    	return 0;
}

static int tamanho_struct(char* campos){
	int tam = 0;
	while (*campos){
		switch (*campos){
        	case 'c':
        	    tam += 1;
        	    break;
        	case 's':
        	    tam += 2;
        	    break;
        	case 'i':
        	    tam += 4;
        	    break;
        	case 'l':
        	    tam += 8;
        	    break;
    	}
		campos++;
	}
	return tam;
}

static int tipo_tam(char tipo){
	int i;
	switch (tipo){
       	case 'c':
       		i = 1;
       	    break;
       	case 's':
       		i = 2;
       	    break;
       	case 'i':
       		i = 4;
       	    break;
       	case 'l':
       		i = 8;
       	    break;
	}
	return i;
}

static void escreve(unsigned char *v, unsigned long valor, int tam, int ini, char endian){
    int i;
    if (!endian){                   /* if (little-endian) */
        for(i = 0; i < tam; i++){
            v[ini+i] = valor;
            valor = (valor>>8);
        }
    } else {                        /* else (big-endian) */
        for(i = tam-1; i >= 0; i--){
            v[ini+i] = valor;
            valor = (valor>>8);
        }
    }
}

static void lecampos(unsigned char* v, unsigned char n, FILE* arquivo){
	int i, aux;
	
	for (i = 0; i < n; i++){
		if (i % 4 == 0) aux = fgetc(arquivo); /* pega o próximo byte com a descrição dos tipos */
		switch(aux & 0xc0){
			case 0x00:
				v[i] = 'c';
				break;
			case 0x40:
				v[i] = 's';
				break;
			case 0x80:
				v[i] = 'i';
				break;
			case 0xc0:
				v[i] = 'l';
				break;
		}
		aux <<= 2;
	}
}

static int cabecalho(int nstructs, char *campos, char ord, FILE *pArquivo){
    unsigned char preencheCabecalho[] = {0x00};
    int i, comprimento, maior = 0;
    
    comprimento = strlen(campos);
    
    fputc(nstructs, pArquivo);
    
    if ( ord == 'L'){
    	preencheCabecalho[0] = preencheCabecalho[0] | 0x80;
    	preencheCabecalho[0] = preencheCabecalho[0] | comprimento;
    	fputc(preencheCabecalho[0], pArquivo);
    }
    else{ /* ord == 'B' */
    	preencheCabecalho[0] = preencheCabecalho[0] | comprimento;
    	fputc(preencheCabecalho[0], pArquivo);
    }
    
    while(*campos){
    	preencheCabecalho[0] = 0x00; /* estraga o valor anterior */
    	i = 0;
    	while (i < 4 && i < comprimento){
    		switch (*campos){
        		case 'c':
            		preencheCabecalho[0] |= 0x00;
            		maior = MAX(maior,1);
            		break;
        		case 's':
            		preencheCabecalho[0] |= 0x01;
            		maior = MAX(maior,2);
            		break;
        		case 'i':
            		preencheCabecalho[0] |= 0x02;
            		maior = MAX(maior,4);
            		break;
        		case 'l':
            		preencheCabecalho[0] |= 0x03;
            		maior = MAX(maior,8);
            		break;
    		}
    		if (i < 3) preencheCabecalho[0] <<= 2;
    		campos++;
    		i++;
    	}
    	if (i == comprimento){
    		i = 3;
    		while( i > comprimento ){
    	 		preencheCabecalho[0] <<= 2;
    	 		i--;
    	 	}
    	}
    	comprimento -= i;
    	fputc(preencheCabecalho[0], pArquivo);
    }
    return maior;
}

int grava_structs (int nstructs, void *valores, char *campos, char ord, char *arquivo){
    int pos;
    int padding;
    int tam;
    int prox;
    int maior;
    int ini;
    unsigned long carrega;
    unsigned char *traducao;
    char endian = 0;
    char *aux;
    FILE* pf = fopen(arquivo,"wb");
    
    if(pf == NULL){
        printf("Problemas na abertura do arquivo\n");
        return -1;
    }
    
    tam = tamanho_struct(campos);
    
    traducao = (unsigned char*)malloc(tam*sizeof(unsigned char));
    
    maior = cabecalho(nstructs, campos, ord, pf);
    
    if (ord == 'B') endian = 1;
    
    while(nstructs){
    	aux = campos;
    	ini = 0;
    	pos = 0;
    	while(*aux){
    		if (pos == maior){
    			pos = 0;
    		}
            switch (*aux){
        		case 'c':
        			carrega = *(unsigned char*)valores;
        			escreve(traducao, carrega, sizeof(char), ini, endian);
        			ini += 1;
            		valores += 1;
            		pos += 1;
            		break;
        		case 's':
        			carrega = *(unsigned short*)valores;
        			escreve(traducao, carrega, sizeof(short), ini, endian);
        			ini += 2;        			
            		valores += 2;
            		pos += 2;
            		break;
        		case 'i':
        			carrega = *(unsigned int*)valores;
        			escreve(traducao, carrega, sizeof(int), ini, endian);
        			ini += 4;
            		valores += 4;
            		pos += 4;
            		break;
        		case 'l':
            		carrega = *(unsigned long*)valores;
        			escreve(traducao, carrega, sizeof(long), ini, endian);
        			ini += 8; 
        			valores += 8;
        			pos += 8;
            		break;
    		}
    		aux++;
    		if (*aux){
    			prox = tipo_tam(*aux);
    			if (!ehmultiplo(pos, prox)){
    				padding = prox - (pos % prox);
    		    	valores += padding;
    		    	pos += padding;
    			}
    		} else valores += (maior - pos);
        }
        fwrite(traducao,sizeof(unsigned char),tam,pf);
        nstructs--;
    }
    free(traducao);
    fclose(pf);
    return 0;
}

void dump_structs (char *arquivo){
	unsigned char nstructs;
	unsigned char endian;
	unsigned char ncampos;
	unsigned char *campos;
	unsigned char aux;
	int tam;
	int i;
	
	FILE *pf = fopen(arquivo,"rb");
	
	if(pf == NULL){
        printf("Problemas na abertura do arquivo\n");
        exit(-1);
    }
	
	nstructs = fgetc(pf);
	endian = fgetc(pf);
	ncampos = endian & 0x7F;
	
	if (endian & 0x80)  /* if (little-endian) */ 
		puts("L");
	else 				/* if (big-endian) */
		puts("B");
	
	printf("%d\n", nstructs);
	
	campos = (unsigned char*)malloc(ncampos*sizeof(unsigned char));
	
	lecampos(campos, ncampos, pf);
	
	while (nstructs){
		puts("*");
		for (i = 0; i < ncampos; i++){
			tam = tipo_tam(campos[i]);
			while(tam > 0){
				aux = fgetc(pf);
				printf("%02x ", aux);
				tam--;
			}
			puts("");
		}
		nstructs--;
	}
	puts("*");
	free(campos);
	fclose(pf);
}

