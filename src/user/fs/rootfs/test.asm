[BITS 64]

%include 'fs/rootfs/syscall.asm'

mov rax, 1
add rax, 5

push 55

mov rax, SYSCALL_CONSOLE_WRITE
mov rbx, rsp
syscall

mov rax, SYSCALL_PROCESS_EXIT
mov rbx, rsp
syscall

mov rax, 6
add rax, 9

loop:
jmp loop