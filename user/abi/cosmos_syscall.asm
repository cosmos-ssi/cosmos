
BITS 64
DEFAULT REL

global cosmos_syscall

cosmos_syscall:
    push rax;
    push rbx;
    push rcx;
    push rdx;

    sysenter

    pop rdx;
    pop rcx;
    pop rbx;
    pop rax;

    retf    
