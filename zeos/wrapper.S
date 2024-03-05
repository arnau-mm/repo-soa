/*
 * wrapper.s - Entry point to system mode from user mode
 */

#include <asm.h>

ENTRY(write)
    pushl %ebp
    movl %esp, %ebp
    
    movl 8(%ebp), %edx     // Move file descriptor to %edx
    movl 12(%ebp), %ecx    // Move buffer address to %ecx
    movl 16(%ebp), %ebx    // Move size to %ebx
    movl $4, %eax          // System call number for write

    int $0x80              // Make the system call

    cmpl $0, %eax          // Check if there was an error
    jl  write_error        // Jump to write_error if there was an error

    popl %ebp              // Restore the stack frame
    ret

write_error:
    negl %eax              // Negate the error code
    movl %eax, errno       // Store the error code in errno
    movl $-1, %eax         // Set return value to -1
    popl %ebp              // Restore the stack frame
    ret







