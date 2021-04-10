[BITS 64]

global syscall;
; https://wiki.osdev.org/Calling_Conventions
; x86_64 calling convention uses these registers for the 1st 6 args: rdi, rsi, rdx, rcx, r8, r9
; we have 2 args syscall # in rdi and the address of the args struct in rsi

; x86_64 syscall uses rdi, rsi, rdx, r10, r8, r9; slightly different.  rax is teh syscall #.
; therefore rsi (the struct*) needs to be in rdi

syscall:
    ; save the "preserve" registers
    push rbx
    push rbp
    push rsp
    push r12
    push r13
    push r14
    push r15

    ; rax and rdi are scrap registers, so we can change them...
    ; the syscall will change rcx and r11, those are scrap registers too...
    mov rax, rdi    ; 1st param from userland function is in rdi: the syscall #
    mov rdi, rsi    ; put the 2nd param from userland function into rdi
    syscall

    pop r15
    pop r14
    pop r13
    pop r12
    pop rsp
    pop rbp
    pop rbx

    ret             ; return value is in rax
