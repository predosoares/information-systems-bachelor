.data
tamBufferAberto: .int 0

.text
.globl myopen
myopen:
	movq %rdx, $tamBufferAberto
	movq $2, %rax
	syscall
	
	leave
	ret

.text
.globl myread
myread:
	movq $0, %rax
	movq $tamBufferAberto, %rdx
	syscall
	
	leave
	ret

.text
.globl mywrite
mywrite:
	movq $1, %rax
	syscall
	
	leave
	ret

.text
.globl myclose
myclose:
	movq $3, %rax
	syscall
	
	leave
	ret
