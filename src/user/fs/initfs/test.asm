[BITS 64]

mov rax, 1
add rax, 5

mov rax, 0
mov rbx, 0
syscall

mov rax, 6
add rax, 9

loop:
jmp loop