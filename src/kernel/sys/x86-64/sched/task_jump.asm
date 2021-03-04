global task_jump;

task_jump:
         mov rax, [rdi + 8]   ; cr3
         mov cr3, rax

         mov rax, [rdi + (8 * 22)]
         push rax
         popfq

         jmp $
ret