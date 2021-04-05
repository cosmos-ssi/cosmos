[BITS 64]

global syscall;
; x86_64 calling convention uses these registers for the 1st 6 args: rdi, rsi, rdx, rcx, r8, r9
; we have 2 args syscall # in rdi and the address of the args struct in rsi

; x86_64 syscall uses rdi, rsi, rdx, r10, r8, r9; slightly different.  rax is teh syscall #.
; therefore rsi (the struct*) needs to be in rdi

syscall:
    push rdi        ; preserve rdi   

    mov rax, rdi    ; 1st param from userland function is in rdi: the syscall #
    mov rdi, rsi    ; put the 2nd param from userland function into rbx
    syscall

    pop rdi         ; restore rdi
    ret             ; return value is in rax
