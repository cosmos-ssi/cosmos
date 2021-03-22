[BITS 64]

global syscall0;
global syscall1;
global syscall2;

; https://wiki.osdev.org/Calling_Conventions
; function parameters are rdi, rsi, rdx, rcx, r8, r9
; function return value in rax, rdx

syscall0:
    push rbx        ; preserve rbx

    mov rax, rdi    ; 1st param from userland function is in rdi: the syscall #
    mov rbx, rsp    ; put the stack pointer into rbx
    syscall

    pop rbx         ; restore rbx
    ret             ; return value is in rax

syscall1:
    push rbx        ; preserve rbx
    push rsi        ; push the second parameter of the user function.  This is the 1st parameter of the syscall

    mov rax, rdi    ; 1st param from userland function is in rdi: the syscall #
    mov rbx, rsp    ; put the stack pointer into rbx
    syscall

    pop rsi         ; remove the parameter from the stack
    pop rbx         ; restore rbx
    ret             ; return value is in rax

syscall2:
    push rbx        ; preserve rbx
    push rsi        ; push the second parameter of the user function.  This is the 1st parameter of the syscall
    push rdx

    mov rax, rdi    ; 1st param from userland function is in rdi: the syscall #
    mov rbx, rsp    ; put the stack pointer into rbx
    syscall

    pop rdx
    pop rsi         ; remove the parameter from the stack
    pop rbx         ; restore rbx
    ret             ; return value is in rax
