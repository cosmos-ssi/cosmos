[BITS 64]

global syscall;
; x86_64 calling convention uses these registers for the 1st 6 args: rdi, rsi, rdx, rcx, r8, r9
; we have 2 args syscall # in rdi and the address of the args struct in rsi

; x86_64 syscall uses rdi, rsi, rdx, r10, r8, r9; slightly different.  rax is teh syscall #.
; therefore rsi (the struct*) needs to be in rdi

syscall:
    push rdi        ; preserve rdi   
    push rdx
    push rcx
    push rbx
    push rsi
    push r10
    push r9
    push r8

    mov rax, rdi    ; 1st param from userland function is in rdi: the syscall #
    mov rdi, rsi    ; put the 2nd param from userland function into rbx
    mov rdx, 0
    mov r10, 0
    mov r8, 0
    mov r9, 0
    syscall

    pop r8
    pop r9
    pop r10
    pop rsi
    pop rbx
    pop rcx
    pop rdx
    pop rdi         ; restore rdi
    ret             ; return value is in rax
