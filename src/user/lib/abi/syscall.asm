[BITS 64]

global syscall;

; https://wiki.osdev.org/Calling_Conventions
; function parameters are rdi, rsi, rdx, rcx, r8, r9
; function return value in rax, rdx

syscall:
    push rbx        ; preserve rbx   

    mov rax, rdi    ; 1st param from userland function is in rdi: the syscall #
    mov rbx, rsi    ; put the 2nd param from userland function into rbx
                    ; rdx and rcx already contain userland function parameters 3, and 4
    syscall

    pop rbx         ; restore rbx
    ret             ; return value is in rax
