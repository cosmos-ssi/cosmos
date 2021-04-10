[BITS 64]

global syscall_portal;

extern syscall_dispatcher;

; https://wiki.osdev.org/Calling_Conventions
; x86_64 calling convention uses these registers for the 1st 6 args: rdi, rsi, rdx, rcx, r8, r9
; we have 2 args syscall # in rdi and the address of the args struct in rsi

; x86_64 syscall uses rdi, rsi, rdx, r10, r8, r9; slightly different.  rax is teh syscall #.
; therefore rsi (the struct*) needs to come from rdi

syscall_portal:

    mov r12, rsp;      ; move rsp into r12
    push r12           ; save rsp
    push r11           ; save rflags
    push rcx           ; save RIP

    mov rsi, rdi       ; move 1st parameter of user function into second parameter of kernel handler (this is a struct*)
    mov rdi, rax       ; move syscall number into 1st parameter

    call syscall_dispatcher

    pop rcx             ; restore RIP
    pop r11             ; restore rflags
    pop r12             ; restore rsp into r12
    mov rsp, r12        ; restore rsp
    
    o64 sysret
         