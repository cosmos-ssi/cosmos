[BITS 64]

global syscall0;
global syscall1;
global tge;

; https://wiki.osdev.org/Calling_Conventions
; function parameters are rdi, rsi, rdx, rcx, r8, r9
; function return value in rax, rdx
syscall0:
    mov rax, rdi
    mov rbx, rsp
    syscall
    ret

syscall1:
    push rax
    mov rax, rdi
    mov rbx, rsp
    syscall
    pop rax
    ret
