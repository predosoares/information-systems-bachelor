#include <stdio.h>
#include <stdlib.h>
#include "gera_codigo.h"

static char filename[] = "SB00";

void dump (void *p, int n) {
  int i = 0;
  unsigned char *p1 = p;
  while (n--) {
    printf("%d: %p - %02x\n", i, p1, *p1);
    p1++;
    i++;
  }
}

//void libera(void *p) {
  //free(p);
//}

int main(int argc, char *argv[]) {
  FILE *myfp;
  void *code;
  funcp funcaoSB;
  int i, first, last;
  int res, res_aluno;
  float nota_teste;

  float nota = 0.0;

  /* Obtém primeiro e último teste */
  if (argc < 3) {
    printf("Uso: %s <primeiro teste> <ultimo teste>\n", argv[0]);
    return 1;
  }
  if ((first = atoi(argv[1])) == 0) {
    printf(">>> Erro em %s: primeiro teste (%s) invalido\n", argv[0], argv[1]);
    return 1;
  }
  if ((last = atoi(argv[2])) == 0) {
    printf(">>> Erro em %s: ultimo teste (%s) invalido\n", argv[0], argv[2]);
    return 1;
  }
  if (first > last) {
    printf(">>> Erro em %s: intervalo de teste (%s-%s) invalido\n", 
            argv[0], argv[1], argv[2]);
    return 1;
  }
 
  for (i = first; i <= last; i++) {
    sprintf(filename, "SB%d", i);
    printf("\n  Teste %d (%s):\n", i, filename);
    
    /* Abre o arquivo fonte */
    if ((myfp = fopen(filename, "r")) == NULL) {
      perror(">>> Falha na abertura do arquivo fonte");
      continue;
    }

    /* Lê a nota do teste */
    if(fscanf(myfp, "%f\n", &nota_teste) != 1) {
      printf(">>> Erro na leitura da nota\n");
      continue;
    }

    /* Lê o resultado do teste */
    if(fscanf(myfp, "%d\n", &res) != 1) {
      printf(">>> Erro na leitura do resultado\n");
      continue;
    }

    /* compila a função SB */
    gera_codigo(myfp, &code, &funcaoSB);
    if ((code == NULL) || (funcaoSB == NULL)) {
      printf(">>> Erro na compilação\n");
      continue;
    }

#ifdef DEBUG
  printf("code = %p entry = %p\n",code, funcaoSB);
  printf("\n--------------\n");
  dump(code, 128);
  printf("\n--------------\n");
#endif

    /* Chama a função e testa o resultado */
    res_aluno = (*funcaoSB)(5);

    printf("   Resultado esperado %d obtido %d ", res, res_aluno);
    if (res == res_aluno) {
      nota = nota + nota_teste;
      printf(" (OK) teste = %4.2f acumulado = %4.2f\n", nota_teste, nota);
    }
    else {
      printf(" (NÃO OK) teste = %4.2f acumulado = %4.2f\n", nota_teste, nota);
    }

    libera_codigo(code);
  }
  printf("\n************* Nota final: %4.2f\n\n", nota);
  return 0;
}
