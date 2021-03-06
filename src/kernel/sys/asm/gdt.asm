[BITS 64]

global asm_lgdt;
global asm_sgdt;

asm_lgdt:
         lgdt [rdi]
         ret

asm_sgdt:
         sgdt [rel sgdt_val]
         mov rax, sgdt_val
         ret   

         sgdt_val:   db 10