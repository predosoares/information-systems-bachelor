/* Pedro Henrique Aragão Soares 1713186 3wa */
/*---------------------------------------------Questão#1--------------------------------------------*/
/*
struct X {
  int val;
  struct X *next;
};

int add (struct X *x) {
  int a = 0;
  for (; x != NULL; x = x->next)
    a += x->val;
  return a;
}
*/
/*
.data

.text
.globl add
add:
  pushq %rbp
  movq %rsp, %rbp
  
  movl $0, %eax
L1:  
  cmpq $0, %rdi
  je L2
  addl (%rdi), %eax
  addq $8, %rdi
  movq (%rdi), %rdi
  jmp L1
  
L2:  
  leave
  ret
*/
/*---------------------------------------------Questão#2--------------------------------------------*/
/*
int add2 (struct X *x) {
  if (x == NULL) return 0;
  else return x->val + add2(x->next);
}
*/
/*
.data

.text
.globl add2
add2:
  pushq %rbp
  movq %rsp, %rbp
  subq $16, %rsp
  movq %rbx, -8(%rbp)
  
  cmpq $0, %rdi
  je L1
  movq %rdi, %rbx
  addq $8, %rdi
  movq (%rdi), %rdi
  call add2
  addl (%rbx), %eax
  jmp L2
L1:
  movl $0, %eax
L2:
  movq -8(%rbp), %rbx
  leave
  ret
*/
/*---------------------------------------------Questão#3--------------------------------------------*/
/*
struct X {
  int val1;
  int val2;
};

int f(int i, int v);

void boo (struct X *px, int n, int val) {
  while (n--) {
    px->val2 = f(px->val1, val);
    px++;
  }
}
*/
/*
.data
 
.text
.globl boo
boo:
  pushq %rbp
  movq %rsp, %rbp
  subq $16, %rsp
  movq %rbx, -8(%rbp)
  
  movl %esi, %ebx
  movl %edx, %esi
L1:
  cmpl $0, %ebx
  je L2

  call f
  addq $4, %rdi
  movl %eax, (%rdi)
  addq $4, %rdi
  decl %ebx
  jmp L1
  
L2:
  movq -8(%rbp), %rbx
  leave
  ret
*/
/*---------------------------------------------Questão#4--------------------------------------------*/
/*
int f(int x);

void map2 (int* um, int * outro, int n) {
  int i;
  for (i=0; i<n; i++)
    *(outro+i) = f(*(um+i));
}
*/

.data
 
.text
.globl map2
map2:
  pushq %rbp
  movq %rsp, %rbp
  subq $32, %rsp
  movq %r12, -32(%rbp)
  movq %r13, -24(%rbp)
  movq %rdx, -16(%rbp)
  movq %rbx, -8(%rbp)
  
  movl $0, %ebx 
  movq %rdi, %r12
  movq %rsi, %r13
L1:
  cmpl -16(%rbp), %ebx
  jae L2
  movl (%r12,%rbx,4), %edi
  call f
  movl %eax, (%r13,%rbx,4)
  incl %ebx
  jmp L1
L2:
  movq -32(%rbp), %r12
  movq -24(%rbp), %r13
  movq -8(%rbp), %rbx
  leave
  ret
  
