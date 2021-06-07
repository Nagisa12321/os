.section .text
.global switch_to, ret_to_user_model

ESP0 = 4
KERNAL_STACK = 12

switch_to:
    pushl %ebp
    movl %esp, %ebp
    # pushfl
    pushl %ecx
    pushl %ebx
    pushl %eax

    movl 8(%ebp), %ebx
    cmpl %ebx, current
    je 1f

    # switch the pcb
    movl %ebx, %eax
    xchgl %eax, current

    # change tss's esp0
    movl init_tss, %ecx
    addl $4096, %ebx
    movl %ebx, ESP0(%ecx)

    # change kernal stack
    movl %esp, KERNAL_STACK(%eax)
    movl 8(%ebp), %ebx
    movl KERNAL_STACK(%ebx), %esp

    # change LDT(next)
    movl 12(%ebp), %ecx
    lldt %cx

    movl $0x17, %ecx
    mov %cx, %fs

    cmpl %eax, last_task_used_math 
    jne 1f
    clts
    
1:  
    popl %eax
    popl %ebx
    popl %ecx
    # popfl
    popl %ebp

    ret

ret_to_user_model:
    # addl $0x02, %esp
    popl %eax
    popl %ebp
    popl %edi
    popl %esi
    pop %gs
    popl %ebx
    popl %ecx
    popl %edx
    pop %fs
    pop %es
    pop %ds
    iret
