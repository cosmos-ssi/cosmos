[BITS 64]

global task_jump;

task_jump:
         mov rax, [rdi + 8]   ; cr3
         mov cr3, rax

         ; rflags
         mov rax, [rdi + (8 * 22)]
         push rax
         popfq

         ; We can't load rip directly, and we don't want to anyway because we
         ; still have work to do.  Can't put it in a register because that
         ; would clobber it, so we put it in a memory area we've specified
         ; so we can access it after we set the saved rdi value.  And since we
         ; also can't do a memory-to-memory mov, we move it into rax and then
         ; into that location

         mov rax, [rdi + (8 * 21)]
         mov [rel jmpaddr], rax

         mov rax, [rdi + (8 * 5)]
         mov rbx, [rdi + (8 * 6)]
         mov rcx, [rdi + (8 * 7)]
         mov rdx, [rdi + (8 * 8)]
         mov rsi, [rdi + (8 * 9)]

         ; skip rdi for now, since we need it as-is to continue filling out
         ; the rest of the registers
         mov rsp, [rdi + (8 * 11)]
         mov rbp, [rdi + (8 * 12)]
         mov r8, [rdi + (8 * 13)]
         mov r9, [rdi + (8 * 14)]
         mov r10, [rdi + (8 * 15)]
         mov r11, [rdi + (8 * 16)]
         mov r12, [rdi + (8 * 17)]
         mov r13, [rdi + (8 * 18)]
         mov r14, [rdi + (8 * 19)]
         mov r15, [rdi + (8 * 20)]

         ; now we can move rdi into place
         mov rdi, [rdi + (8 * 10)]

         jmp $

         jmpaddr: dq 0
ret