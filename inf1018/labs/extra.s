/*
int foo (int a[], int tam) {
  int i;
  int b[tam];
  for (i = 0; i<tam; i++)
    b[i] = calcula (&(a[i]), tam-i);
  return calcula (b, tam);
}
*/
.data
.text
.globl foo
foo:
	pushq %rbp
	movq %rsp, %rbp
	subq $32, %rsp
	movq %r12, -32(%rbp)
	movl %q13d, -16(%rbp)
	movl %esi, -12(%rbp)
	movq %r14, -8(%rbp)
	
	movq %rdi, %r14
	movq $0, %r13
L1:
	cmpl -12(%rbp), %r13d
	je L2
	leaq (%r14,%r13,4), %rdi
	movl -12(%rbp), %esi
	subl %r13d, %esi
	call calcula
	
	movl %eax, (%r12,%r13,4)
	
	incq %r13
	jmp L1
L2:
	
	leave
	ret
