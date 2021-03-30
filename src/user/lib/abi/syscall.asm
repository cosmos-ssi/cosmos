[BITS 64]

global syscall;

syscall:
    push rbx        ; preserve rbx   

    mov rax, rdi    ; 1st param from userland function is in rdi: the syscall #
    mov rbx, rsi    ; put the 2nd param from userland function into rbx
    syscall

    pop rbx         ; restore rbx
    ret             ; return value is in rax
