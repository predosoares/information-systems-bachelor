/* Pedro Henrique Aragão Soares 1713186 3wa */

#include <stdio.h>
#include <stdlib.h>

#define NUM_MAX_FUNCOES 25
#define NUM_MAX_RETCONDICIONAIS 48
#define TAMANHO_MAX 1000

typedef int (*funcp) (int a);

static void error (const char *msg, int line) {
    fprintf(stderr, "erro %s na linha %d\n", msg, line);
    exit(EXIT_FAILURE);
}

/*
	Descricao:
		A funcao geraPrologo escreve o codigo de maquina do registro de
		ativacao.
	Parametros:
		@codigo : ponteiro vetor de codigo de maquina.
		@posicao : inteiro correspondente a próxima posicao a ser
		preenchida no vetor codigo.
	Retorna:
		Inteiro correspondente a proxima posicao a ser preenchida no vetor
		codigo.
	Observacao:
		Foram reservados 32 bytes na pilha para armazenar as 5 variaveis
		locais e o valor do parametro (%edi).	
*/
static int geraPrologo (unsigned char* codigo, int posicao) { // -- OK!
	codigo[posicao]      = 0x55;     // push   %rbp
	codigo[posicao + 1]  = 0x48;     
	codigo[posicao + 2]  = 0x89;
	codigo[posicao + 3]  = 0xe5;     // mov    %rsp,%rbp
	codigo[posicao + 4]  = 0x48;
	codigo[posicao + 5]  = 0x83;
	codigo[posicao + 6]  = 0xec;
	codigo[posicao + 7]  = 0x20;     // sub    $0x20,%rsp
	codigo[posicao + 8]  = 0x89;
	codigo[posicao + 9]  = 0x7d;
	codigo[posicao + 10] = 0xe8;     // mov    %edi,-0x18(%rbp)
	return posicao + 11;
}

/* 
	Descricao:
		A funcao passaInteiro escreve o codigo de maquina de um valor 
		inteiro.
	Parametros:
		@codigo : ponteiro vetor de codigo de maquina.
		@posicao : inteiro correspondente a próxima posicao a ser
		preenchida no vetor codigo.
		@idx : valor inteiro.
	Retorna:
		Inteiro correspondente a proxima posicao a ser preenchida no vetor
		codigo.
*/
static int passaInteiro(unsigned char* codigo, int posicao, int idx) {
	codigo[posicao]     = *((unsigned char*)&idx);
	codigo[posicao + 1] = *((unsigned char*)&idx + 1);
	codigo[posicao + 2] = *((unsigned char*)&idx + 2);
	codigo[posicao + 3] = *((unsigned char*)&idx + 3);
	return posicao + 4;
}

/* 
	Descricao:
		A funcao passaebx escreve o codigo de maquina para passar varpc
		para o registrador auxiliar %ebx.
	Parametros:
		@codigo : ponteiro vetor de codigo de maquina.
		@posicao : inteiro correspondente a próxima posicao a ser
		preenchida no vetor codigo.
		@var : caracter identificador (variavel, parametro ou constante).
		@idx : inteiro identificador (indice da variavel ou valor inteiro)
	Retorna:
		Inteiro correspondente a proxima posicao a ser preenchida no vetor
		codigo.
*/
static int passaebx (unsigned char* codigo, int posicao, char var, int idx) {
    switch(var){
        case 'v':{
            codigo[posicao]     = 0x8b;
            codigo[posicao + 1] = 0x5d;
            codigo[posicao + 2] = 0xfc - (0x04)*idx; // mov idx0(%rbp), %ebx
            posicao += 3;
            break;
        }
        case 'p':{
            codigo[posicao]     = 0x8b;
            codigo[posicao + 1] = 0x5d; 
            codigo[posicao + 2] = 0xe8; // mov -0x18(%rbp), %ebx 
            posicao += 3;
            break;
        }
        case '$':{
            codigo[posicao]     = 0xbb;
            posicao = passaInteiro(codigo, posicao + 1, idx); // mov idx0, %ebx
            break;
        }
    }
    return posicao;
}

/* 
	Descricao:
		A funcao gera_code lê um arquivo texto na linguagem SBF e traduz
		para código de máquina para que possa ser executada.
	Parametros:
		@f : ponteiro para o arquivo em SBF em modo leitura.
		@code : ponteiro para o espaco de memoria que sera alocado
		dinamicamente para ser preenchido com codigo de maquina.
		@entry : ponteiro para a ultima funcao lida que sera chamada 
		externamente.
*/
void gera_code (FILE *f, void **code, funcp *entry) {
	unsigned char *codigo, enderecoJe[NUM_MAX_RETCONDICIONAIS];
	unsigned int enderecoFuncoes[NUM_MAX_FUNCOES], endereco;
	int linha, posicao, funcao, zret;
	char c;
	
	*code = (unsigned char*)malloc(TAMANHO_MAX); //número de bytes de código de máquina 
	codigo = (unsigned char*)*code;
	linha = 1;
	funcao = 0;
	posicao = 0;
	
	while((c = fgetc(f)) != EOF){
	
	    switch (c) {
            case 'f': { /* function - OK! */
                char c0;
                if (fscanf(f, "unction%c", &c0) != 1)
                    error("comando invalido", linha);
                    
                zret = 0;
                *entry = (funcp)&codigo[posicao]; // atribui a entry o endereco da ultima funcao lida
                enderecoFuncoes[funcao] = (long)&codigo[posicao];
                funcao++;                                   
                posicao = geraPrologo(codigo, posicao);
                break;
            }
            case 'e': { /* end - OK! */
                char c0;
                if (fscanf(f, "nd%c", &c0) != 1)
                    error("comando invalido", linha);
                    
                for (int i = 0; i < zret; i++){ // calcula os enderecos de todos os zret's
                	endereco = (unsigned long)&codigo[posicao] - (unsigned long)&codigo[enderecoJe[i] + 4];
                	codigo[enderecoJe[i]]     = *((unsigned char*)&endereco);
                    codigo[enderecoJe[i] + 1] = *((unsigned char*)&endereco + 1);
                    codigo[enderecoJe[i] + 2] = *((unsigned char*)&endereco + 2);
                    codigo[enderecoJe[i] + 3] = *((unsigned char*)&endereco + 3);
                }
                codigo[posicao]     = 0xc9; // leaveq
				codigo[posicao + 1] = 0xc3; // retq
				posicao += 2;
                break;
            }
            case 'r': {  /* retorno incondicional - OK!*/
                int idx0;
                char var0;
                if (fscanf(f, "et %c%d", &var0, &idx0) != 2) 
                    error("comando invalido", linha);
                
                switch (var0){
                    case 'v': {
                        codigo[posicao]     = 0x8b;
                        codigo[posicao + 1] = 0x45;
						codigo[posicao + 2] = 0xfc - (0x04)*idx0; // mov idx0(%rbp), %eax
						posicao += 3;
                        break;
                    }
                    case 'p': {
                    	codigo[posicao]     = 0x8b;                       
                        codigo[posicao + 1] = 0x45; 
                        codigo[posicao + 2] = 0xe8; // mov -0x18(%rbp), %eax 
                        posicao += 3;
                        break;
                    }
                    case '$': {      
                    	codigo[posicao] = 0xb8;                 
                        posicao = passaInteiro(codigo, posicao + 1, idx0); // mov idx0, %eax
                        break;
                    }
                }
                break;
            }
            case 'z': {  /* retorno condicional - OK! */
                int idx0, idx1;
                char var0, var1;
                if (fscanf(f, "ret %c%d %c%d", &var0, &idx0, &var1, &idx1) != 4)
                    error("comando invalido", linha);
                
                switch (var1){
                    case 'v': {
                    	codigo[posicao]     = 0x8b;
                        codigo[posicao + 1] = 0x45;
                        codigo[posicao + 2] = 0xfc - (0x04)*idx1; // mov idx0(%rbp), %eax
                        posicao += 3;
                        break;
                    }
                    case 'p': {                    
                    	codigo[posicao]     = 0x8b;   
                        codigo[posicao + 1] = 0x45; 
                        codigo[posicao + 2] = 0xe8; // mov -0x18(%rbp), %eax 
                        posicao += 3;
                        break;
                    }
                    case '$': {  
                    	codigo[posicao]     = 0xb8;                      
                        posicao = passaInteiro(codigo, posicao + 1, idx1); // mov idx0, %eax
                        break;
                    }
                }
                posicao = passaebx(codigo, posicao, var0, idx0);
                codigo[posicao] = 0x83;  //cmpl $0, %ebx
                codigo[posicao + 1] = 0xfb;
                codigo[posicao + 2] = 0x00;
                codigo[posicao + 3] = 0x0f; //je <fim>
                codigo[posicao + 4] = 0x84;  
                enderecoJe[zret++] = posicao + 5;
                posicao += 9;
                break;
            }
            case 'v': {  /* atribuicao - OK! */
                int idx0;
                char c0;
                if (fscanf(f, "%d = %c",&idx0, &c0) != 2)
                    error("comando invalido",linha);
                    
                if (c0 == 'c') { /* call - OK! */
                    int fu, idx1;
                    char var1;
                    if (fscanf(f, "all %d %c%d\n", &fu, &var1, &idx1) != 3)
                        error("comando invalido",linha);
                    
                    switch (var1){      //movl varpc, %edi
                        case 'v':{ 
                            codigo[posicao]     = 0x8b;
                            codigo[posicao + 1] = 0x7d;
							codigo[posicao + 2] = 0xfc - (0x04)*idx1;
							posicao += 3;
                            break;
                        }
                        case 'p':{ 
                        	codigo[posicao]     = 0x8b;
                            codigo[posicao + 1] = 0x7d;
                            codigo[posicao + 2] = 0xe8;
                            posicao += 3;
                            break;
                        }
                        case '$':{      
                            codigo[posicao]     = 0xbf;
                            posicao = passaInteiro(codigo, posicao + 1, idx1);
                            break;
                        }
                    }
                    endereco = (unsigned long)enderecoFuncoes[fu] - (unsigned long)&codigo[posicao + 5];
                    
                    codigo[posicao]     = 0xe8;     //call 00 00 00 00
                    codigo[posicao + 1] = *((unsigned char*)&endereco);
                    codigo[posicao + 2] = *((unsigned char*)&endereco + 1);
                    codigo[posicao + 3] = *((unsigned char*)&endereco + 2);
                    codigo[posicao + 4] = *((unsigned char*)&endereco + 3); 
                    codigo[posicao + 5] = 0x89;   // movl %eax, idx0(%rbp) 
                    codigo[posicao + 6] = 0x45;
                    codigo[posicao + 7] = 0xfc - (0x04)*idx0;
                    posicao += 8;
                }
                else { /* operacao aritmetica - OK!*/
                    int idx1, idx2;
                    char var1 = c0, var2, op;
                    if (fscanf(f, "%d %c %c%d", &idx1, &op, &var2, &idx2) != 4)
                        error("comando invalido", linha);

                    posicao = passaebx(codigo, posicao, var1, idx1);
                    switch (op){
                    	case '+':{ // OK!
                    		switch (var2){
                    		    case 'v':{      //addl idx2(%rbp), %ebx 
                    		    	codigo[posicao] = 0x03;  
                    		        codigo[posicao + 1] = 0x5d;
									codigo[posicao + 2] = 0xfc - (0x04)*idx2;
									posicao += 3;
                    		        break;
                    		    }
                    		    case 'p':{      //addl -0x18(%rbp), %ebx                     		            
                    		        codigo[posicao] = 0x03; 
                    		        codigo[posicao + 1] = 0x5d;
                    		        codigo[posicao + 2] = 0xe8;
                    		        posicao += 3;              
                    		        break;
                    		    }
                    		    case '$':{      //addl idx2, %ebx    
                    		        codigo[posicao] = 0x81; 
                    		        codigo[posicao + 1] = 0xc3;
     								posicao = passaInteiro(codigo, posicao + 2, idx2);
                    		        break;
                    		    }
                    		}
                    		break;
                    	}
                    	case '-':{ // OK!
                    		switch (var2){
                    		    case 'v':{      // subl idx2(%rbp), %ebx
                    		        codigo[posicao]     = 0x2b;     
                    		        codigo[posicao + 1] = 0x5d;
                    		        codigo[posicao + 2] = 0xfc - (0x04)*idx2;            
                    		        break;
                    		    }
                    		    case 'p':{      // subl -0x18(%rbp), %ebx   
                                    codigo[posicao]     = 0x2b;     
                    		        codigo[posicao + 1] = 0x5d;
                    		        codigo[posicao + 2] = 0xe8;
                    		        posicao += 3; 
                    		        break;
                    		    }
                    		    case '$':{      // subl $idx2, %ebx
                    		        codigo[posicao]     = 0x81;     
                    		        codigo[posicao + 1] = 0xeb;
                    		        posicao = passaInteiro(codigo, posicao + 2, idx2);         
                    		        break;
                    		    }
                    		}
                    		break;
                    	}
                    	case '*':{ // OK!
                    		switch (var2){
                    		    case 'v':{      //imull idx2(%rbp), %ebx
                    		        codigo[posicao]     = 0x0f;
                    	    		codigo[posicao + 1] = 0xaf;
                    		        codigo[posicao + 2] = 0x5d;
                    		        codigo[posicao + 3] = 0xfc - (0x04)*idx2;
                    		        posicao += 4;           
                    		        break;
                    		    }
                    		    case 'p':{      //imull -0x18(%rbp), %ebx  
                    		        codigo[posicao]     = 0x0f;
                    	    		codigo[posicao + 1] = 0xaf;
                    		        codigo[posicao + 2] = 0x5d;
                    		        codigo[posicao + 3] = 0xe8;
                    		        posicao += 4; 
                    		        break;
                    		    }
                    		    case '$':{      //imull idx2, %ebx
                    		    	codigo[posicao]     = 0x0f;
                    	    		codigo[posicao + 1] = 0xaf;
                    	    		codigo[posicao + 2] = 0x1c;
                    	    		codigo[posicao + 3] = 0x25;
                    		        posicao = passaInteiro(codigo, posicao + 4, idx2);           
                    		        break;
                    		    }
                    		}
                    		break;
                    	}
                    }
                    codigo[posicao]     = 0x89;
					codigo[posicao + 1] = 0x5d;
					codigo[posicao + 2] = 0xfc - (0x04)*idx0; // mov    %ebx, idx0(%rbp)
                	posicao += 3;
                }
                break;
            }
            default: error("comando desconhecido", linha);
        }
        linha ++;
        fscanf(f, "\n");
    }
}


/* 
	Descricao:
		A funcao libera_codigo libera a memoria alocada para armazenamento
	   	do codigo de maquina.
	Parametros:
		@p : ponteiro para o espaco de memoria alocado dinamicamente.
*/
void libera_codigo (void *p) {
	free(p);
}

