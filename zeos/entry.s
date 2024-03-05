# 0 "entry.S"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "entry.S"




# 1 "include/asm.h" 1
# 6 "entry.S" 2
# 1 "include/segment.h" 1
# 7 "entry.S" 2
# 1 "include/errno.h" 1
# 8 "entry.S" 2
# 73 "entry.S"
.globl clock_handler; .type clock_handler, @function; .align 0; clock_handler:
   pushl %gs; pushl %fs; pushl %es; pushl %ds; pushl %eax; pushl %ebp; pushl %edi; pushl %esi; pushl %ebx; pushl %ecx; pushl %edx; movl $0x18, %edx; movl %edx, %ds; movl %edx, %es
   movb $0x20, %al ; outb %al, $0x20 ;
   call clock_routine
   pop %edx; pop %ecx; pop %ebx; pop %esi; pop %edi; pop %ebp; pop %eax; pop %ds; pop %es; pop %fs; pop %gs;
   iret


.globl keyboard_handler; .type keyboard_handler, @function; .align 0; keyboard_handler:
   pushl %gs; pushl %fs; pushl %es; pushl %ds; pushl %eax; pushl %ebp; pushl %edi; pushl %esi; pushl %ebx; pushl %ecx; pushl %edx; movl $0x18, %edx; movl %edx, %ds; movl %edx, %es
   call keyboard_routine
   movb $0x20, %al ; outb %al, $0x20 ;
   pop %edx; pop %ecx; pop %ebx; pop %esi; pop %edi; pop %ebp; pop %eax; pop %ds; pop %es; pop %fs; pop %gs;
   iret

.globl p_f_handler; .type p_f_handler, @function; .align 0; p_f_handler:
   call p_f_routine
# 129 "entry.S"
.globl system_call_handler; .type system_call_handler, @function; .align 0; system_call_handler:
      pushl %gs; pushl %fs; pushl %es; pushl %ds; pushl %eax; pushl %ebp; pushl %edi; pushl %esi; pushl %ebx; pushl %ecx; pushl %edx; movl $0x18, %edx; movl %edx, %ds; movl %edx, %es
      cmpl $0, %eax
      jl err
      cmpl $MAX_SYSCALL, %eax
      jg err
      call *sys_call_table(, %eax, 0x04)
      jmp fin
err:
      movl $-38, %eax
fin:
      movl %eax, 0x18(%esp)
      pop %edx; pop %ecx; pop %ebx; pop %esi; pop %edi; pop %ebp; pop %eax; pop %ds; pop %es; pop %fs; pop %gs;
      iret
