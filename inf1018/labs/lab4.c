/* Questão#1
#include <stdio.h>

void dump (void *p, int n) {
  unsigned char *p1 = p;
  while (n--) {
    printf("%p - %02x\n", p1, *p1);
    p1++;
  }
}

int main (void) {
  char c = 150;
  short s = -3;
  int i = -151;
  printf("dump de c: \n");
  dump(&c, sizeof(c));
  printf("dump de s: \n");
  dump(&s, sizeof(s));
  printf("dump de i: \n");
  dump(&i, sizeof(i));
  return 0;
}
*/
/* Questão#2
#include <stdio.h>

void dump (void *p, int n) {
  unsigned char *p1 = p;
  while (n--) {
    printf("%p - %02x\n", p1, *p1);
    p1++;
  }
}

int main (void) {
  short l = -32765;
  unsigned short k = 32771;
  printf("l=%d, k=%u \n", l, k);
  printf("dump de l: \n");
  dump(&l, sizeof(l));
  printf("dump de k: \n");
  dump(&k, sizeof(k));
  return 0;
}

Explicação:
Em nenhum dos casos há overflow portanto o valor 0x8003 exibido na execução do programa corresponde 
a representação de l em complemento a dois de acordo com a conversão feita em meu caderno enquanto o mesmo valor 0x8003 encontrado para k apesar de ter o mesma representação em hexadecimal trata-se de um tipo unsigned.

*/

/* Questão#3
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef unsigned packed_t;

int string2num (char *s, int base) {
  int a = 0;
  for (; *s; s++) {
    if(isdigit(*s))
      a = a*base + (*s - '0');
    else if((*s >= 'A') && (*s < (base-10+'A')))
      a = a*base + ((*s - 'A') + 10);
    else if((*s >= 'a') && (*s < (base-10+'a')))
      a = a*base + ((*s - 'a') + 10);
    else {
      printf("pane: numero invalido! \n");
      exit(1);
    }
  }
  return a;
}

int xbyte (packed_t word, int bytenum) {
  return (int)(word<<(8*(3-bytenum)))>>24;
}

int main (int argc, char **argv) {
  int x;
  if (argc != 3) {
    printf ("uso: %s <word (em hexadecimal)> <bytenum>\n", argv[0]);
    exit(1);
  }

  x = xbyte(string2num(argv[1], 16), atoi(argv[2]));
  printf ("%08x  %d\n", x, x);
  return 0;
}
*/

/* Questão#4
 programa 1
#include <stdio.h>

int main (void) {
  int x = 0xffffffff;
  int y = 2;
  printf("x=%d, y=%d\n", x, y);
  printf("x é menor do que y? %s\n", (x<y)?"sim":"nao");
  return 0;
}

 programa 2
#include <stdio.h>

int main (void) {
  unsigned int x = 0xffffffff;
  unsigned int y = 2;
  printf("x=%d, y=%d\n", x, y);
  printf("x é menor do que y? %s\n", (x<y)?"sim":"nao");
  return 0;
}


 programa 3
#include <stdio.h>

int main (void) {
  int x = 0xffffffff;
  unsigned int y = 2;
  printf("x=%d, y=%d\n", x, y);
  printf("x é menor do que y? %s\n", (x<y)?"sim":"nao");
  return 0;
}

Explicação:
programa 1 - Os resultados foram os esperados já que as formatações no printf foram corretas.
programa 2 - Por se tratar de uma comparação entre dois valores do tipo unsigned os operador "menor que" interpreta os valores como unsigned portanto mesmo o pritf exibindo a linha "x=-1 e y=2" esse valores não se tratam de inteiros com sinal e sim de valores unsigned.
programa 3 - O operando relacional ao ser usado numa comparação entre um signed int e um unsigned int
 interpreta os dois valores como unsigned, logo mesmo se tratando de um valor real menor(x) que y a máquina não vai dar o valor esperado segundo a aritmética humana.
*/

/*
#include <stdio.h>

void dump (void *p, int n) {
  unsigned char *p1 = p;
  while (n--) {
    printf("%p - %02x\n", p1, *p1);
    p1++;
  }
}

int main(void) {
    signed char sc = -1;
    unsigned int ui = sc;
    
    printf("dump de ui: \n");
    dump(&ui,sizeof(ui));
}
*/
