/*
 * wrapper.s - Entry point to system mode from user mode
 */

#include <asm.h>

ENTRY(write)
	pushl %ebp
	movl %esp, %ebp
	pushl %edx
	movl 8(%ebp), %edx
	movl 12(%ebp), %ecx
	movl 16(%ebp), %ebx
	movl $4, %eax
	int $0x80
	cmpl $0, %eax
	jge write_no_error
	negl %eax
	movl %eax, errno
	movl $-1, %eax
write_no_error:
	popl %edx
	popl %ebp
	ret

wr_return:
	popl %ebp
	addl $4, %esp
	popl %ebx
	popl %ecx
	cmpl $0, %eax
	jge wr_no_error
	negl %eax
	movl %eax, errno
	movl -1, %eax

wr_no_error:
	popl %ebp
	ret






