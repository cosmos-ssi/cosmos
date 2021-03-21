[BITS 64]

%include 'fs/rootfs/syscall.asm'

push 55

mov rax, SYSCALL_CONSOLE_WRITE
mov rbx, rsp
syscall

mov rax, SYSCALL_PROCESS_EXIT
mov rbx, rsp
syscall

loop:
jmp loop