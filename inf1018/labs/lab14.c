/* Pedro Henrique Aragão Soares 1713186 3wa */
/*---------------------------------------------Questão#3---------------------------------------------*/
/*
int foo (int x) {
  return x+1;
}
*/
/*
.data
.text
.globl foo
foo:
    movl %edi, %eax
    addl $1, %eax
    ret
*/
#include <stdio.h>

unsigned char codigo[] = {0xe9,0x00,0x00,0x00,0x00,0xc3};

typedef int (*funcp) (int x);

int add (int x) {
  return x+1;
}

int main(void){
    int i;
    unsigned int endereco = (long)&add - (long)&codigo[5];
    unsigned char *percorre = (unsigned char *) &endereco;
    
    codigo[1] = *(percorre);
    codigo[2] = *(percorre+1);
    codigo[3] = *(percorre+2);
    codigo[4] = *(percorre+3);
    
    funcp f = (funcp)codigo;
    i = (*f)(10);
    
    printf("%d\n", i);
    return 0;
}

