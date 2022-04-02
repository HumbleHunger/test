	.file	"test.c"
	.text
	.globl	count
	.bss
	.align 4
	.type	count, @object
	.size	count, 4
count:
	.zero	4
	.comm	mutex,40,32
	.comm	cond,48,32
	.section	.rodata
.LC0:
	.string	"This is thread_entry"
.LC1:
	.string	"child count: %d\n"
	.text
	.globl	thread_entry
	.type	thread_entry, @function
thread_entry:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	leaq	.LC0(%rip), %rdi
	call	puts@PLT
.L4:
	leaq	mutex(%rip), %rdi
	call	pthread_mutex_lock@PLT
	movl	count(%rip), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	count(%rip), %eax
	addl	$1, %eax
	movl	%eax, count(%rip)
	leaq	mutex(%rip), %rdi
	call	pthread_mutex_unlock@PLT
	movl	count(%rip), %eax
	cmpl	$100, %eax
	jle	.L4
	leaq	cond(%rip), %rdi
	call	pthread_cond_signal@PLT
	movl	count(%rip), %eax
	cltq
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	thread_entry, .-thread_entry
	.section	.rodata
	.align 8
.LC2:
	.string	"Thread %d is waiting for cond\n"
.LC3:
	.string	"Thread %d is awakened\n"
	.text
	.globl	pthread_cond_test
	.type	pthread_cond_test, @function
pthread_cond_test:
.LFB1:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	leaq	mutex(%rip), %rdi
	call	pthread_mutex_lock@PLT
	movl	count(%rip), %eax
	cmpl	$99, %eax
	jg	.L7
	call	pthread_self@PLT
	movq	%rax, %rsi
	leaq	.LC2(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	mutex(%rip), %rsi
	leaq	cond(%rip), %rdi
	call	pthread_cond_wait@PLT
.L7:
	call	pthread_self@PLT
	movq	%rax, %rsi
	leaq	.LC3(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	mutex(%rip), %rdi
	call	pthread_mutex_unlock@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	pthread_cond_test, .-pthread_cond_test
	.section	.rodata
.LC4:
	.string	"This is pthread_test"
.LC5:
	.string	"argv[%d]= %s\n"
.LC6:
	.string	"create thread failed!"
.LC7:
	.string	"parent count: %d\n"
.LC8:
	.string	"thread exit with %x!\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movl	%edi, -52(%rbp)
	movq	%rsi, -64(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	.LC4(%rip), %rdi
	call	puts@PLT
	movl	$0, -36(%rbp)
	jmp	.L9
.L10:
	movl	-36(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-64(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rdx
	movl	-36(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC5(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -36(%rbp)
.L9:
	movl	-36(%rbp), %eax
	cmpl	-52(%rbp), %eax
	jl	.L10
	movl	$0, %esi
	leaq	mutex(%rip), %rdi
	call	pthread_mutex_init@PLT
	movl	$0, %esi
	leaq	cond(%rip), %rdi
	call	pthread_cond_init@PLT
	leaq	-32(%rbp), %rax
	movl	$0, %ecx
	leaq	pthread_cond_test(%rip), %rdx
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_create@PLT
	testl	%eax, %eax
	jns	.L11
	leaq	.LC6(%rip), %rdi
	call	puts@PLT
	movl	$-1, %eax
	jmp	.L16
.L11:
	leaq	-24(%rbp), %rax
	movl	$0, %ecx
	leaq	thread_entry(%rip), %rdx
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_create@PLT
	testl	%eax, %eax
	jns	.L13
	leaq	.LC6(%rip), %rdi
	call	puts@PLT
	movl	$-1, %eax
	jmp	.L16
.L13:
	leaq	mutex(%rip), %rdi
	call	pthread_mutex_lock@PLT
	movl	count(%rip), %eax
	movl	%eax, %esi
	leaq	.LC7(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	count(%rip), %eax
	addl	$1, %eax
	movl	%eax, count(%rip)
	leaq	mutex(%rip), %rdi
	call	pthread_mutex_unlock@PLT
	movl	count(%rip), %eax
	cmpl	$100, %eax
	jg	.L19
	jmp	.L13
.L19:
	nop
	movq	-32(%rbp), %rax
	leaq	-16(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	pthread_join@PLT
	movq	-16(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC8(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
.L16:
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L17
	call	__stack_chk_fail@PLT
.L17:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
