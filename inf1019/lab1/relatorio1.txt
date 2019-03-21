Jonny Russo - 1610608
Pedro Henrique Soares - 1713186

*************************************************************************************************************

Questão 1) Faça um programa em que três processos executam em paralelo as seguintes 
ações:
Pai - Imprime os números de 0 a 99, com um intervalo de 1 segundo entre a 
impressão de cada número. Após imprimir todos os números, imprime a frase 
“Processo pai vai finalizar” e finaliza quando o filho terminar.
Filho - Imprime os números de 100 a 199, com um intervalo de 2 segundo entre a 
impressão cada número. Antes de imprimir os números, imprime a frase “Filho foi 
criado”.
Após imprimir todos os números, imprime a frase “processo filho vai finalizar” e finaliza 
quando o neto terminar.
Neto - filho do processo Filho (ou seja, neto do processo Pai). Imprime os números de 
200 a 299, com um intervalo de 3 segundos entre cada número. Antes de imprimir os 
números, imprime a frase “Neto foi criado”.
Após imprimir todos os números, imprime a frase “processo neto vai finalizar” e finaliza 
o processo.
Importante: Em cada printf os processos devem imprimir o seu pid e o pid do seu pai.
DICA: A chamada ao sistema sleep(1) bloqueia o processo por 1 segundo.
Pergunta-se: É possível observar os processos executando em paralelo? Como?

*************************************************************************************************************

// Lab 1 (Fork e Exec) - inf1019 - Sistemas de Computação

// Exer 1 - prática de fork

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUMBER 100

int main(int argc, char* argv[]){
    int i;
    int status;

    if ( fork() != 0 ){ /* Main process */
        /* Parent code */
        for ( i = 0; i < NUMBER; i++) {
            printf("PID pai: %d\t\t\tContagem: %d\n", getpid(), i);
            sleep(1);
        }
        waitpid(-1, &status, 0);
        puts("Processo pai finalizado");

    } else {
        if ( fork() != 0 ) {
            /* Child code */
            puts("Filho foi criado");
            for ( i = 100; i < NUMBER*2; i++) {
                printf("PID pai: %d\tPID filho: %d\tContagem: %d\n", getppid(), getpid(), i);
                sleep(2);
            }
            waitpid(-1, &status, 0);
            puts("Processo filho vai finalizar");

        } else {
            /* Grandson code */
            puts("Neto foi criado");
            for ( i = 200; i < NUMBER*3; i++) {
                printf("PID pai: %d\tPID neto: %d\tContagem: %d\n", getppid(), getpid(), i);
                sleep(3);
            }
            puts("Processo neto vai finalizar");
        }
    }
    
    return 0;
}

*************************************************************************************************************

Comando para compilação:
gcc -o lab1.1 lab1.1.c
./lab1.1

*************************************************************************************************************
RESULTADO:

PID pai: 3249			Contagem: 0
Filho foi criado
PID pai: 3249	PID filho: 3250	Contagem: 100
Neto foi criado
PID pai: 3250	PID neto: 3251	Contagem: 200
PID pai: 3249			Contagem: 1
PID pai: 3249			Contagem: 2
PID pai: 3249	PID filho: 3250	Contagem: 101
PID pai: 3249			Contagem: 3
PID pai: 3250	PID neto: 3251	Contagem: 201
PID pai: 3249			Contagem: 4
PID pai: 3249	PID filho: 3250	Contagem: 102
PID pai: 3249			Contagem: 5
PID pai: 3249	PID filho: 3250	Contagem: 103
PID pai: 3250	PID neto: 3251	Contagem: 202
PID pai: 3249			Contagem: 6
PID pai: 3249			Contagem: 7
PID pai: 3249	PID filho: 3250	Contagem: 104
PID pai: 3249			Contagem: 8
PID pai: 3250	PID neto: 3251	Contagem: 203
PID pai: 3249			Contagem: 9
PID pai: 3249	PID filho: 3250	Contagem: 105
PID pai: 3249			Contagem: 10
PID pai: 3249			Contagem: 11
PID pai: 3250	PID neto: 3251	Contagem: 204
PID pai: 3249	PID filho: 3250	Contagem: 106
PID pai: 3249			Contagem: 12
PID pai: 3249			Contagem: 13
PID pai: 3249	PID filho: 3250	Contagem: 107
PID pai: 3249			Contagem: 14
PID pai: 3250	PID neto: 3251	Contagem: 205
PID pai: 3249			Contagem: 15
PID pai: 3249	PID filho: 3250	Contagem: 108
PID pai: 3249			Contagem: 16
PID pai: 3249			Contagem: 17
PID pai: 3250	PID neto: 3251	Contagem: 206
PID pai: 3249	PID filho: 3250	Contagem: 109
PID pai: 3249			Contagem: 18
PID pai: 3249			Contagem: 19
PID pai: 3249	PID filho: 3250	Contagem: 110
PID pai: 3249			Contagem: 20
PID pai: 3250	PID neto: 3251	Contagem: 207
PID pai: 3249			Contagem: 21
PID pai: 3249	PID filho: 3250	Contagem: 111
PID pai: 3249			Contagem: 22
PID pai: 3249			Contagem: 23
PID pai: 3250	PID neto: 3251	Contagem: 208
PID pai: 3249	PID filho: 3250	Contagem: 112
PID pai: 3249			Contagem: 24
PID pai: 3249			Contagem: 25
PID pai: 3249	PID filho: 3250	Contagem: 113
PID pai: 3249			Contagem: 26
PID pai: 3250	PID neto: 3251	Contagem: 209
PID pai: 3249			Contagem: 27
PID pai: 3249	PID filho: 3250	Contagem: 114
PID pai: 3249			Contagem: 28
PID pai: 3249			Contagem: 29
PID pai: 3250	PID neto: 3251	Contagem: 210
PID pai: 3249	PID filho: 3250	Contagem: 115
PID pai: 3249			Contagem: 30
PID pai: 3249			Contagem: 31
PID pai: 3249	PID filho: 3250	Contagem: 116
PID pai: 3249			Contagem: 32
PID pai: 3250	PID neto: 3251	Contagem: 211
PID pai: 3249			Contagem: 33
PID pai: 3249	PID filho: 3250	Contagem: 117
PID pai: 3249			Contagem: 34
PID pai: 3249			Contagem: 35
PID pai: 3250	PID neto: 3251	Contagem: 212
PID pai: 3249	PID filho: 3250	Contagem: 118
PID pai: 3249			Contagem: 36
PID pai: 3249			Contagem: 37
PID pai: 3249	PID filho: 3250	Contagem: 119
PID pai: 3249			Contagem: 38
PID pai: 3250	PID neto: 3251	Contagem: 213
PID pai: 3249			Contagem: 39
PID pai: 3249	PID filho: 3250	Contagem: 120
PID pai: 3249			Contagem: 40
PID pai: 3249			Contagem: 41
PID pai: 3250	PID neto: 3251	Contagem: 214
PID pai: 3249	PID filho: 3250	Contagem: 121
PID pai: 3249			Contagem: 42
PID pai: 3249			Contagem: 43
PID pai: 3249	PID filho: 3250	Contagem: 122
PID pai: 3249			Contagem: 44
PID pai: 3250	PID neto: 3251	Contagem: 215
PID pai: 3249			Contagem: 45
PID pai: 3249	PID filho: 3250	Contagem: 123
PID pai: 3249			Contagem: 46
PID pai: 3249			Contagem: 47
PID pai: 3250	PID neto: 3251	Contagem: 216
PID pai: 3249	PID filho: 3250	Contagem: 124
PID pai: 3249			Contagem: 48
PID pai: 3249			Contagem: 49
PID pai: 3249	PID filho: 3250	Contagem: 125
PID pai: 3249			Contagem: 50
PID pai: 3250	PID neto: 3251	Contagem: 217
PID pai: 3249			Contagem: 51
PID pai: 3249	PID filho: 3250	Contagem: 126
PID pai: 3249			Contagem: 52
PID pai: 3249			Contagem: 53
PID pai: 3250	PID neto: 3251	Contagem: 218
PID pai: 3249	PID filho: 3250	Contagem: 127
PID pai: 3249			Contagem: 54
PID pai: 3249			Contagem: 55
PID pai: 3249	PID filho: 3250	Contagem: 128
PID pai: 3249			Contagem: 56
PID pai: 3250	PID neto: 3251	Contagem: 219
PID pai: 3249			Contagem: 57
PID pai: 3249	PID filho: 3250	Contagem: 129
PID pai: 3249			Contagem: 58
PID pai: 3249			Contagem: 59
PID pai: 3250	PID neto: 3251	Contagem: 220
PID pai: 3249	PID filho: 3250	Contagem: 130
PID pai: 3249			Contagem: 60
PID pai: 3249			Contagem: 61
PID pai: 3249	PID filho: 3250	Contagem: 131
PID pai: 3249			Contagem: 62
PID pai: 3250	PID neto: 3251	Contagem: 221
PID pai: 3249			Contagem: 63
PID pai: 3249	PID filho: 3250	Contagem: 132
PID pai: 3249			Contagem: 64
PID pai: 3249			Contagem: 65
PID pai: 3250	PID neto: 3251	Contagem: 222
PID pai: 3249	PID filho: 3250	Contagem: 133
PID pai: 3249			Contagem: 66
PID pai: 3249			Contagem: 67
PID pai: 3249	PID filho: 3250	Contagem: 134
PID pai: 3249			Contagem: 68
PID pai: 3250	PID neto: 3251	Contagem: 223
PID pai: 3249			Contagem: 69
PID pai: 3249	PID filho: 3250	Contagem: 135
PID pai: 3249			Contagem: 70
PID pai: 3249			Contagem: 71
PID pai: 3250	PID neto: 3251	Contagem: 224
PID pai: 3249	PID filho: 3250	Contagem: 136
PID pai: 3249			Contagem: 72
PID pai: 3249			Contagem: 73
PID pai: 3249	PID filho: 3250	Contagem: 137
PID pai: 3249			Contagem: 74
PID pai: 3250	PID neto: 3251	Contagem: 225
PID pai: 3249			Contagem: 75
PID pai: 3249	PID filho: 3250	Contagem: 138
PID pai: 3249			Contagem: 76
PID pai: 3249			Contagem: 77
PID pai: 3250	PID neto: 3251	Contagem: 226
PID pai: 3249	PID filho: 3250	Contagem: 139
PID pai: 3249			Contagem: 78
PID pai: 3249			Contagem: 79
PID pai: 3249	PID filho: 3250	Contagem: 140
PID pai: 3249			Contagem: 80
PID pai: 3250	PID neto: 3251	Contagem: 227
PID pai: 3249			Contagem: 81
PID pai: 3249	PID filho: 3250	Contagem: 141
PID pai: 3249			Contagem: 82
PID pai: 3249			Contagem: 83
PID pai: 3250	PID neto: 3251	Contagem: 228
PID pai: 3249	PID filho: 3250	Contagem: 142
PID pai: 3249			Contagem: 84
PID pai: 3249			Contagem: 85
PID pai: 3249	PID filho: 3250	Contagem: 143
PID pai: 3249			Contagem: 86
PID pai: 3250	PID neto: 3251	Contagem: 229
PID pai: 3249			Contagem: 87
PID pai: 3249	PID filho: 3250	Contagem: 144
PID pai: 3249			Contagem: 88
PID pai: 3249			Contagem: 89
PID pai: 3250	PID neto: 3251	Contagem: 230
PID pai: 3249	PID filho: 3250	Contagem: 145
PID pai: 3249			Contagem: 90
PID pai: 3249			Contagem: 91
PID pai: 3249	PID filho: 3250	Contagem: 146
PID pai: 3249			Contagem: 92
PID pai: 3250	PID neto: 3251	Contagem: 231
PID pai: 3249			Contagem: 93
PID pai: 3249	PID filho: 3250	Contagem: 147
PID pai: 3249			Contagem: 94
PID pai: 3249			Contagem: 95
PID pai: 3250	PID neto: 3251	Contagem: 232
PID pai: 3249	PID filho: 3250	Contagem: 148
PID pai: 3249			Contagem: 96
PID pai: 3249			Contagem: 97
PID pai: 3249	PID filho: 3250	Contagem: 149
PID pai: 3249			Contagem: 98
PID pai: 3250	PID neto: 3251	Contagem: 233
PID pai: 3249			Contagem: 99
PID pai: 3249	PID filho: 3250	Contagem: 150
PID pai: 3250	PID neto: 3251	Contagem: 234
PID pai: 3249	PID filho: 3250	Contagem: 151
PID pai: 3249	PID filho: 3250	Contagem: 152
PID pai: 3250	PID neto: 3251	Contagem: 235
PID pai: 3249	PID filho: 3250	Contagem: 153
PID pai: 3250	PID neto: 3251	Contagem: 236
PID pai: 3249	PID filho: 3250	Contagem: 154
PID pai: 3249	PID filho: 3250	Contagem: 155
PID pai: 3250	PID neto: 3251	Contagem: 237
PID pai: 3249	PID filho: 3250	Contagem: 156
PID pai: 3250	PID neto: 3251	Contagem: 238
PID pai: 3249	PID filho: 3250	Contagem: 157
PID pai: 3249	PID filho: 3250	Contagem: 158
PID pai: 3250	PID neto: 3251	Contagem: 239
PID pai: 3249	PID filho: 3250	Contagem: 159
PID pai: 3250	PID neto: 3251	Contagem: 240
PID pai: 3249	PID filho: 3250	Contagem: 160
PID pai: 3249	PID filho: 3250	Contagem: 161
PID pai: 3250	PID neto: 3251	Contagem: 241
PID pai: 3249	PID filho: 3250	Contagem: 162
PID pai: 3250	PID neto: 3251	Contagem: 242
PID pai: 3249	PID filho: 3250	Contagem: 163
PID pai: 3249	PID filho: 3250	Contagem: 164
PID pai: 3250	PID neto: 3251	Contagem: 243
PID pai: 3249	PID filho: 3250	Contagem: 165
PID pai: 3250	PID neto: 3251	Contagem: 244
PID pai: 3249	PID filho: 3250	Contagem: 166
PID pai: 3249	PID filho: 3250	Contagem: 167
PID pai: 3250	PID neto: 3251	Contagem: 245
PID pai: 3249	PID filho: 3250	Contagem: 168
PID pai: 3250	PID neto: 3251	Contagem: 246
PID pai: 3249	PID filho: 3250	Contagem: 169
PID pai: 3249	PID filho: 3250	Contagem: 170
PID pai: 3250	PID neto: 3251	Contagem: 247
PID pai: 3249	PID filho: 3250	Contagem: 171
PID pai: 3250	PID neto: 3251	Contagem: 248
PID pai: 3249	PID filho: 3250	Contagem: 172
PID pai: 3249	PID filho: 3250	Contagem: 173
PID pai: 3250	PID neto: 3251	Contagem: 249
PID pai: 3249	PID filho: 3250	Contagem: 174
PID pai: 3250	PID neto: 3251	Contagem: 250
PID pai: 3249	PID filho: 3250	Contagem: 175
PID pai: 3249	PID filho: 3250	Contagem: 176
PID pai: 3250	PID neto: 3251	Contagem: 251
PID pai: 3249	PID filho: 3250	Contagem: 177
PID pai: 3250	PID neto: 3251	Contagem: 252
PID pai: 3249	PID filho: 3250	Contagem: 178
PID pai: 3249	PID filho: 3250	Contagem: 179
PID pai: 3250	PID neto: 3251	Contagem: 253
PID pai: 3249	PID filho: 3250	Contagem: 180
PID pai: 3250	PID neto: 3251	Contagem: 254
PID pai: 3249	PID filho: 3250	Contagem: 181
PID pai: 3249	PID filho: 3250	Contagem: 182
PID pai: 3250	PID neto: 3251	Contagem: 255
PID pai: 3249	PID filho: 3250	Contagem: 183
PID pai: 3250	PID neto: 3251	Contagem: 256
PID pai: 3249	PID filho: 3250	Contagem: 184
PID pai: 3249	PID filho: 3250	Contagem: 185
PID pai: 3250	PID neto: 3251	Contagem: 257
PID pai: 3249	PID filho: 3250	Contagem: 186
PID pai: 3250	PID neto: 3251	Contagem: 258
PID pai: 3249	PID filho: 3250	Contagem: 187
PID pai: 3249	PID filho: 3250	Contagem: 188
PID pai: 3250	PID neto: 3251	Contagem: 259
PID pai: 3249	PID filho: 3250	Contagem: 189
PID pai: 3250	PID neto: 3251	Contagem: 260
PID pai: 3249	PID filho: 3250	Contagem: 190
PID pai: 3249	PID filho: 3250	Contagem: 191
PID pai: 3250	PID neto: 3251	Contagem: 261
PID pai: 3249	PID filho: 3250	Contagem: 192
PID pai: 3250	PID neto: 3251	Contagem: 262
PID pai: 3249	PID filho: 3250	Contagem: 193
PID pai: 3249	PID filho: 3250	Contagem: 194
PID pai: 3250	PID neto: 3251	Contagem: 263
PID pai: 3249	PID filho: 3250	Contagem: 195
PID pai: 3250	PID neto: 3251	Contagem: 264
PID pai: 3249	PID filho: 3250	Contagem: 196
PID pai: 3249	PID filho: 3250	Contagem: 197
PID pai: 3250	PID neto: 3251	Contagem: 265
PID pai: 3249	PID filho: 3250	Contagem: 198
PID pai: 3250	PID neto: 3251	Contagem: 266
PID pai: 3249	PID filho: 3250	Contagem: 199
PID pai: 3250	PID neto: 3251	Contagem: 267
PID pai: 3250	PID neto: 3251	Contagem: 268
PID pai: 3250	PID neto: 3251	Contagem: 269
PID pai: 3250	PID neto: 3251	Contagem: 270
PID pai: 3250	PID neto: 3251	Contagem: 271
PID pai: 3250	PID neto: 3251	Contagem: 272
PID pai: 3250	PID neto: 3251	Contagem: 273
PID pai: 3250	PID neto: 3251	Contagem: 274
PID pai: 3250	PID neto: 3251	Contagem: 275
PID pai: 3250	PID neto: 3251	Contagem: 276
PID pai: 3250	PID neto: 3251	Contagem: 277
PID pai: 3250	PID neto: 3251	Contagem: 278
PID pai: 3250	PID neto: 3251	Contagem: 279
PID pai: 3250	PID neto: 3251	Contagem: 280
PID pai: 3250	PID neto: 3251	Contagem: 281
PID pai: 3250	PID neto: 3251	Contagem: 282
PID pai: 3250	PID neto: 3251	Contagem: 283
PID pai: 3250	PID neto: 3251	Contagem: 284
PID pai: 3250	PID neto: 3251	Contagem: 285
PID pai: 3250	PID neto: 3251	Contagem: 286
PID pai: 3250	PID neto: 3251	Contagem: 287
PID pai: 3250	PID neto: 3251	Contagem: 288
PID pai: 3250	PID neto: 3251	Contagem: 289
PID pai: 3250	PID neto: 3251	Contagem: 290
PID pai: 3250	PID neto: 3251	Contagem: 291
PID pai: 3250	PID neto: 3251	Contagem: 292
PID pai: 3250	PID neto: 3251	Contagem: 293
PID pai: 3250	PID neto: 3251	Contagem: 294
PID pai: 3250	PID neto: 3251	Contagem: 295
PID pai: 3250	PID neto: 3251	Contagem: 296
PID pai: 3250	PID neto: 3251	Contagem: 297
PID pai: 3250	PID neto: 3251	Contagem: 298
PID pai: 3250	PID neto: 3251	Contagem: 299
Processo neto vai finalizar
Processo filho vai finalizar
Processo pai finalizado

*************************************************************************************************************
CONCLUSÃO:

Sim. Os processos ocorrem em paralelo assim que a primeira chamada a fork é realizada. Após a chamada
um novo processo iniciará concorrentemente. Caso as chamadas a waitpid fossem realizadas antes da execução
 dos loops elas teriam que aguardar a finalização do processo filho.

*************************************************************************************************************



Questão 2) Implemente os seguintes programas: o primeiro exibe a mensagem “alo 
mundo!”, o segundo implementa o programa echo do Unix, que exibe no terminal 
os argumentos do programa. Compile estes programas. Elabore um programa 
que crie e execute os dois programas que você escreveu: o alomundo e o echo. 
Utilize alguma função da família "execv" para realizar esta atividade.
DICA: Para saber os protótipos das funções disponíveis execute o comando 
“man” no Terminal (para acessar o manual: "man execv").
Pergunta1: O que você observou em termos de semelhanças e diferenças para 
executar o alomundo e o echo?
Pergunta2: Indique como você decidiu implementar os programas. Há 
concorrência? Há hierarquia entre os processos? Explique

*************************************************************************************************************

// Lab 1 (Fork e Exec) - inf1019 - Sistemas de Computação

// Exer 2 - prática de exec

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 

int main(int argc, char* argv[]){

    if ( fork() != 0 ){ 
        // exec olamundo
        execv(argv[1], argv);
    } else { 
        //exec echoo
        execv(argv[2], argv);
    }
    
    return 0;
}

*************************************************************************************************************
AUXILIARES:

#include <stdio.h>

int main(){

	puts("Alo mundo!");
	
	return 0;
}

#include <stdio.h>

int main(int argc, char* argv[]){

    for(int i = 1; i < argc; i++){
        printf("%s ", argv[i]);
    }
    puts("");
    
    return 0;
}

*************************************************************************************************************
COMPILAÇÃO E EXECUÇÃO:

gcc -o alomundo alomundo.c
gcc -o echoo echoo.c
gcc -o lab1.2 lab1.2.c

./lab1.2 olamundo echoo
*************************************************************************************************************
RESULTADO:

Alo mundo!
olamundo echoo

*************************************************************************************************************
CONCLUSÃO:

Pergunta1: Para a execução do echoo foi necessário passar os argumentos via argv já a função aceita string 
para exibir no terminal enquanto a execução de alomundo não necessitava de parâmetros.

Pergunta2: Na implementação foi decidido que o parâmetro argv[1] é executada pelo processo pai enquanto
o argv[2] é executado pelo processo filho. Há concorrência já que há uma chamada a fork, criando assim um
 outro processo. Existe hierarquia entre o processo pai e o filho.

