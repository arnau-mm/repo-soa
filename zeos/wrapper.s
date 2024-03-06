# 0 "wrapper.S"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "wrapper.S"




# 1 "include/asm.h" 1
# 6 "wrapper.S" 2

.globl write; .type write, @function; .align 0; write:
    pushl %ebp
    movl %esp, %ebp

    pushl %edx
    pushl %ecx
    pushl %ebx

    movl 8(%ebp), %edx
    movl 12(%ebp), %ecx
    movl 16(%ebp), %ebx
    movl $4, %eax

    int $0x80

    cmpl $0, %eax
    jl write_error

    popl %ebx
    popl %ecx
    popl %edx

    popl %ebp
    ret

write_error:
    negl %eax
    movl %eax, errno
    movl $-1, %eax

    popl %ebx
    popl %ecx
    popl %edx

    popl %ebp
    ret
