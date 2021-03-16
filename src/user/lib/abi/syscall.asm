
BITS 64
DEFAULT REL

global syscall

syscall:
    
    mov rax, rax   ; the syscall #
    mov rbx, rsp   ; stack pointer
    syscall

    ret
