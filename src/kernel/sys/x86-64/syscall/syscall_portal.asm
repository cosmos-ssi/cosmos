[BITS 64]

global syscall_portal;

extern syscall_dispatcher;

syscall_portal:

    mov r12, rsp;      ; move rsp into r12
    push r12           ; save rsp
    push r11           ; save rflags
    push rcx           ; save RIP

    mov rdi, rax       ; move syscall number into 1st parameter
    mov rsi, rbx       ; move 1st parameter of user function into second parameter of kernel handler (this is a struct*)

    call syscall_dispatcher

    pop rcx             ; restore RIP
    pop r11             ; restore rflags
    pop r12             ; restore rsp into r12
    mov rsp, r12        ; restore rsp
    
    o64 sysret
         