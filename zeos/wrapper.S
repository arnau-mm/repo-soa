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

ENTRY(gettime)
	pushl %ebp
	movl %esp, %ebp
	movl $10, %eax
	pushl %ecx
	pushl %edx
	pushl $return_gettime
	pushl %ebp
	movl %esp, %ebp
	sysenter
return_gettime:
	popl %ebp
	addl $4, %esp
	popl %edx
	popl %ecx
	cmpl $0, %eax
        jge gettime_no_error
        negl %eax
        movl %eax, errno
        movl $-1, %eax
gettime_no_error:
	popl %ebp
	ret
