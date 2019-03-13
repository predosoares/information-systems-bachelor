/* Pedro Henrique Aragão Soares 1713186 3wa */
/*---------------------------------------------Questão#1---------------------------------------------*/
/*
int add (int a, int b, int c) {
  return a+b+c;
}
*/
/*
.data

.text
.globl add
add:

 pushq %rbp
 movq %rsp, %rbp
 
 addl %edi, %esi
 addl %esi, %edx
 movl %edx, %eax

 leave
 ret
 */
 
/*---------------------------------------------Questão#2---------------------------------------------*/
/*
int fat (int n) {
  if (n==0) return 1;
  else return n*fat(n-1);
}
*/

.data

.text
.globl fat
fat:

 pushq %rbp
 movq %rsp, %rbp

 cmpl $0, %edi
 je L1
 subl $1, %edi
 call fat
 addl $1, %edi
 imull %edi, %eax
 jmp L2
L1:
 movl $1, %eax
L2:
 leave
 ret
 
/*---------------------------------------------Questão#3---------------------------------------------*/
/*
void foo (int a[], int n) {
  int i;
  int s = 0;
  for (i=0; i<n; i++) {
    s += a[i];
    if (a[i] == 0) {
      a[i] = s;
      s = 0;
    }
  }
}
*/
/*
.data
.text
.globl foo
foo:
  pushq %rbp
  movq %rsp, %rbp
  
  movq $0, %r12
  movl $0, %ebx
L1:
  cmpq %rsi, %r12
  je L3
  addl (%rdi,%r12,4), %ebx
  cmpl $0, (%rdi,%r12,4)
  jne L2
  movl %ebx, (%rdi,%r12,4)
  movl $0, %ebx
L2:
  incq %r12
  jmp L1
L3:
  leave
  ret
  
*/
