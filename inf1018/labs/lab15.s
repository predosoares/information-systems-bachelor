/* Pedro Henrique Aragão Soares 1713186 3wa */
/*---------------------------------------------Questão#1---------------------------------------------*/
/*
float foo (double a, float b) {
  return (a+b)*(a-b);
}
*/

.data
.text
.globl foo
foo:
    pushq %rbp
    movq %rsp, %rbp
    
    cvtsd2ss %xmm0, %xmm0
    movss %xmm0, %xmm2
    addss %xmm1, %xmm2
    subss %xmm1, %xmm0
    mulss %xmm2, %xmm0
    
    leave
    ret
/*---------------------------------------------Questão#2---------------------------------------------*/
/*
float foo1 (float a, float b) {
  return sin(a) + b;
}
*/

.data
.text
.globl foo1
foo1:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    movq %xmm1, -16(%rbp)
    
    cvtss2sd %xmm0, %xmm0
    call sin
    cvtsd2ss %xmm0, %xmm0
    addss -16(%rbp), %xmm0
    
    movq -16(%rbp), %xmm1
    leave
    ret
    
/*---------------------------------------------Questão#3---------------------------------------------*/
/*
double foo2 (float a, float b) {
  return sin(a) + cos(b);
}*/

.data
.text
.globl foo2
foo2:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    movq %xmm1, -16(%rbp)
    
    cvtss2sd %xmm0, %xmm0
    call sin
    cvtsd2ss %xmm0, %xmm0
    
    movq %xmm0, -8(%rbp)
    
    cvtss2sd -16(%rbp), %xmm0
    call cos
    cvtsd2ss %xmm0, %xmm0
    
    addss -8(%rbp), %xmm0
    
    movq -16(%rbp), %xmm1
    leave
    ret
    
    
/*---------------------------------------------Questão#4---------------------------------------------*/
/*    
double foo3 (double *a, int n) {
  int i;
  double r = 0.0;
  for (i=0; i<n; i++) {
    r += sin(*a);
    a++;
  }
  return r;
}
*/

.data
.text
.globl foo3
foo3:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp
    movq %r12, -16(%rbp) 
    movl %esi, -8(%rbp)
    movq %rbx, -32(%rbp)
    
    movq %rdi, %r12
    movl $0, %ebx
    CVTSI2SD %ebx, %xmm1
    movsd %xmm1, -24(%rbp)
L1:
    cmpl -8(%rbp), %ebx
    je L2
    
    movq (%r12), %xmm0
    call sin
    movsd  -24(%rbp), %xmm1
    addsd %xmm0, %xmm1
    movsd %xmm1, -24(%rbp)
    
    addq $8, %r12
    incl %ebx
    jmp L1
L2: 
    movsd -24(%rbp), %xmm0
    
    movq -16(%rbp), %r12 
    movq -32(%rbp), %rbx
    leave
    ret
    
    
    
    
    
    
    
    
    
    
    
    
    
    
   
