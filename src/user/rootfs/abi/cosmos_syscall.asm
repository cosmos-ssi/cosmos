
BITS 64
DEFAULT REL

global syscall

syscall:
    pop rax;        // the syscall #

    sysenter

    retf
