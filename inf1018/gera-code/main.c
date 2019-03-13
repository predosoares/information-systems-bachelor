#include <stdio.h>
#include <stdlib.h>

typedef int (*funcp) (int x);

void gera_code (FILE *f, void **code, funcp *entry);
void libera_codigo (void *p);

int main (int argc, char* argv[]){
    FILE *myfp;
    funcp funcSBF;
    void *code = NULL;
    
    /* Abre o arquivo para leitura */
    if ((myfp = fopen (argv[1], "r")) == NULL) {
        perror ("nao conseguiu abrir arquivo!");
        return 1;
    }
    /* Gera o codigo */
    gera_code(myfp, &code, &funcSBF);
    if ((code == NULL) || (funcSBF == NULL)) {
        printf("Erro na geracao\n");
        return 1;
    }
    /* Fecha o arquivo */
    fclose(myfp);
    /* Chama a função gerada */
    printf("%d\n", (*funcSBF)(atoi(argv[2])));
    
    /* Libera a memória utilizada */
    libera_codigo(code);
    
    return 0;
}
