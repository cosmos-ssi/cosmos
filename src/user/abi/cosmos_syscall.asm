
BITS 64
DEFAULT REL

global syscall

syscall:
    
    mov rax, rdi   ; the syscall #
    mov rbx, rsi   ; stack pointer
    syscall

    ret
