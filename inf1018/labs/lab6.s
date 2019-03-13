/*
int nums[] = {10, -21, -30, 45};
int main() {
  int i, *p;
  for (i = 0, p = nums; i != 4; i++, p++)
    printf("%d\n", *p);
  return 0;
}
*/


/* Questão 2 */

/*
int nums[] = {10, -21, -30, 45};
int main() {
  int i, *p;
  int sum = 0;
  for (i = 0, p = nums; i != 4; i++, p++)
    sum += *p;
  printf("%d\n", sum);
  return 0;
}
*/
/* TRECHO ALTERADO

  movl  $0, %ebx  
  movq  $nums, %r12  
  movl  $0, %eax 

L1:
  cmpl  $4, %ebx  
  je  L2          

  addl  (%r12), %eax    

  addl  $1, %ebx  
  addq  $4, %r12  
  jmp  L1         
L2:
*/

/* Questão 3 */
/*
int nums[] = {10, -21, -30, 45};
int main() {
  int i, *p;
  for (i = 0, p = nums; i != 4; i++, p++)
    if ((*p % 2) == 0)
      printf("%d\n", *p);
  return 0;
}
*/

/* TRECHO ALTERADO

  movl  $0, %ebx 
  movq  $nums, %r12  
L1:
  cmpl  $4, %ebx  
  je  L2         
  
  movl  (%r12), %eax
  andl  $1, %eax
  cmpl  $1, %eax
  Je L3
  movl  (%r12), %eax

  movq    $Sf, %rdi    
  movl    %eax, %esi   
  call  printf       

L3:
  addl  $1, %ebx  
  addq  $4, %r12  
  jmp  L1         

L2: 
*/

/* Questão 4 */

.data
nums:  .byte  10, -21, -30, 45
Sf:  .string "%d\n"    # string de formato para printf

.text
.globl  main
main:

/********************************************************/
/* mantenha este trecho aqui e nao mexa - prologo !!!   */
  pushq   %rbp
  movq    %rsp, %rbp
  subq    $16, %rsp
  movq    %rbx, -8(%rbp)
  movq    %r12, -16(%rbp)
/********************************************************/

  movl  $0, %ebx  /* ebx = 0; */
  movq  $nums, %r12  /* r12 = &nums */

L1:
  cmpl  $4, %ebx  /* if (ebx == 4) ? */
  je  L2          /* goto L2 */

  movsbl  (%r12), %eax    /* eax = *r12 */

/*************************************************************/
/* este trecho imprime o valor de %eax (estraga %eax)  */
  movq    $Sf, %rdi    /* primeiro parametro (ponteiro)*/
  movl    %eax, %esi   /* segundo parametro  (inteiro) */
  call  printf       /* chama a funcao da biblioteca */
/*************************************************************/

  addl  $1, %ebx  /* ebx += 1; */
  addq  $1, %r12  /* r12 += 1; */
  jmp  L1         /* goto L1; */

L2:  
/***************************************************************/
/* mantenha este trecho aqui e nao mexa - finalizacao!!!!      */
  movq  $0, %rax  /* rax = 0  (valor de retorno) */
  movq  -8(%rbp), %rbx
  movq  -16(%rbp), %r12
  leave
  ret      
/***************************************************************/

