BITS 64

; we do not push or pop rax, since that's where the return value goes
; we do not push rsp b/c we save it and pop it and then popaq overwrites it...
%macro pushaq 0
push rbx
push rcx
push rdx
;push rsp
push rbp
push rdi
push rsi
push r8
push r9
push r10
push r11
push r12
push r13
push r14
push r15
%endmacro

%macro popaq 0
pop r15
pop r14
pop r13
pop r12
pop r11
pop r10
pop r9
pop r8
pop rsi
pop rdi
pop rbp
;pop rsp
pop rdx
pop rcx
pop rbx
%endmacro

global syscall_portal;

extern syscall_dispatcher;

syscall_portal:
    pushaq

    mov rdi, rax       ; move syscall number into 1st parameter
    mov rsi, rbx       ; move 1st parameter of user function into second parameter of kernel handler
    mov rax, rsp       ; move stack pointer into rax
    mov rsp, 0         ; stack pointer zero
    push rax           ; save stack pointer

    call syscall_dispatcher
    
    ;no need to move return value--SysV ABI will place return from
    ;syscall_dispatcher into rax, which is also where it goes in
    ;CosmOS syscall ABI

    pop rsp            ; get back the stack pointer
    
    popaq

    o64 sysret
         