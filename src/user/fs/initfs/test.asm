[BITS 64]

mov rax, 1
add rax, 5

push 55

mov rax, 1
mov rbx, rsp
syscall

mov rax, 6
add rax, 9

loop:
jmp loop