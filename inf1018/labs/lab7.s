/*
Nome: Pedro Henrique Aragão Soares
Matrícula: 1713186
*/

/*---------------------------------------------Questão#2--------------------------------------------*/
/*
char S2[] = {65, 108, 111, 32, 123, 103, 97, 108, 101, 114, 97, 125, 33, 0};
int main (void) {
  char *pc = S2;
  while ((*pc) && (*pc!=97))
    printf ("%c", *pc++);
  printf("\n");
  return 0;
}
*/
/*
.data
S2:    .byte 65, 108, 111, 32, 123, 103, 97, 108, 101, 114, 97, 125, 33, 0
Sf:  .string "%c"    
Sf2: .string "\n"    

.text
.globl  main
main:



  pushq   %rbp
  movq    %rsp, %rbp
  subq    $16, %rsp
  movq    %rbx, -8(%rbp)  
  movq    %r12, -16(%rbp)  


  movq  $S2, %r12  

L1:
  cmpb  $0, (%r12)  
  je  L2          
  cmpb  $97, (%r12) 
  je  L3

  movsbl  (%r12), %eax    

  movq    $Sf, %rdi    
  movl    %eax, %esi    
  movl  $0, %eax
  call  printf       

L3:
  addq  $1, %r12  
  jmp  L1         

L2:  
              
  movq    $Sf2, %rdi    
  movl  $0, %eax
  call  printf        



     
  movq  $0, %rax   
  movq    -16(%rbp), %r12 
  movq    -8(%rbp), %rbx  
  leave
  ret      
*/
/*---------------------------------------------Questão#3--------------------------------------------*/
/*
#include<stdio.h>
#define N 11

int main(void){
    int i, a;
    for(i = 1; i < N; i++){
        a = i*i;
        printf("%d\n", a);
    }
    return 0;
}
*/
/*
.data
Sf: .string "%d\n"

.text
.globl main
main:


  pushq   %rbp
  movq    %rsp, %rbp
  subq    $16, %rsp
  movq    %rbx, -8(%rbp)   
  movq    %r12, -16(%rbp)  

  movl $1, %ebx
  
L2:  
  cmpl $10, %ebx
  ja L1
  movl %ebx, %eax
  imull %eax, %eax


  movq    $Sf, %rdi    
  movl    %eax, %esi   
  movl  $0, %eax
  call  printf       
  
  incl %ebx
  jmp L2
  
L1:   
  movq  $0, %rax   
  movq    -16(%rbp), %r12 
  movq    -8(%rbp), %rbx  
  leave
  ret
*/
 
/*---------------------------------------------Questão#4-------------------------------------------*/
/*
#include <stdio.h>

int nums[4] = {65, -105, 111, 34};

int main (void) {
  int i;
  int s = 0;

  for (i=0;i<4;i++)
    s = s+nums[i];

  printf ("soma = %d\n", s);

  return 0;
}
*/
/*
.data
Sf: .string "soma = %d\n"
nums: .int 65,-105,111,34

.text
.globl main
main:

  pushq   %rbp
  movq    %rsp, %rbp
  subq    $16, %rsp
  movq    %rbx, -8(%rbp)   
  movq    %r12, -16(%rbp)  
  
  movq $0, %rbx 
  movq $nums, %r12
  movl $0, %eax
  
L2:
  cmpl $4, %ebx
  je L1 
  addl (%r12,%rbx,4), %eax
  incq %rbx  
  jmp L2
  
L1:
  movq    $Sf, %rdi    
  movl    %eax, %esi   
  movl  $0, %eax
  call  printf

  movq  $0, %rax   
  movq    -16(%rbp), %r12 
  movq    -8(%rbp), %rbx  
  leave
  ret
*/
/*---------------------------------------------Questão#5--------------------------------------------*/
/*
int nums[] = {10, -21, -30, 45};
int main() {
  int i, *p;
  for (i = 0, p = nums; i != 4; i++, p++)
    if (((*p % 2) == 0) && (*p > 0))
      printf("%d\n", *p);
  return 0;
}
*/
.data
nums: .int 10,-21,-30,45
Sf: .string "%d\n"

.text
.globl main
main:

  pushq   %rbp
  movq    %rsp, %rbp
  subq    $16, %rsp
  movq    %rbx, -8(%rbp)
  movq    %r12, -16(%rbp)

  movl $0, %ebx
  movq $nums, %r12
  
L2:
  movl (%r12), %eax
  cmpl $0, (%r12)
  je L1
  cmpl $0, %ecx
  jb L3
  andl $1, %ecx
  cmpl $1, %ecx
  je L3
  movl (%r12), %eax
  movq  $Sf, %rdi    
  movl  %eax, %esi   
  movl  $0, %eax
  call  printf
  
L3:
  jmp L2
  
L1:
  movq  $0, %rax   
  movq  -16(%rbp), %r12 
  movq  -8(%rbp), %rbx  
  leave
  ret
  
  
