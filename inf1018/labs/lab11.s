/* Pedro Henrique Aragão Soares 1713186 3wa */
/*---------------------------------------------Questão#1--------------------------------------------*/
/*
int novonum(void) {
  int minhalocal;
  printf("numero: ");
  scanf("%d",&minhalocal);
  return minhalocal;
}
*/
/*
.data
Sf1: .string "numero: "
Sf2: .string "%d"

.text
.globl novonum
novonum:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    
    movq $Sf1, %rdi
    call printf
    
    movq $Sf2, %rdi
    leaq -16(%rbp), %rsi
    call scanf
    
    movl -16(%rbp), %eax
    
    leave
    ret
*/
/*---------------------------------------------Questão#2--------------------------------------------*/   
/*    
int bemboba (int num) {
  int local[4];
  int *a;
  int i;

  for (i=0,a=local;i<4;i++) {
    *a = num;
    a++;
  }
  return addl (local, 4);
}
*/
.data
.text
.globl bemboba
bemboba:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp
    movq %r12, -16(%rbp)
    leaq -32(%rbp), %r12
    
    movl $0, %ecx
L1:
    cmpl $4, %ecx
    je L2
    movl %edi, (%r12)
    addq  $4, %r12
    incl %ecx
    jmp L1
L2: 
    leaq -32(%rbp), %rdi
    movl $4, %esi
    call addl
    
    movq -16(%rbp), %r12
    leave
    ret
    
    
    
    
    
    
    
    
    
