[BITS 64]

%macro pushaq 0
push rax
push rbx
push rcx
push rdx
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
pop rdx
pop rcx
pop rbx
pop rax
%endmacro

global asm_lgdt;
global asm_ltr;
global asm_sgdt;

asm_lgdt:
         pushaq
         lgdt [rdi]
         popaq
         ret

asm_ltr:
         pushaq
         ltr di
         popaq
         ret

asm_sgdt:
         pushaq
         sgdt [rel sgdt_val]
         popaq
         mov rax, sgdt_val
         ret   

         sgdt_val:   db 10