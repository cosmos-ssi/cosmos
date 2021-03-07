[BITS 64]

global task_jump;

task_jump:
         mov rax, [rdi + 8]   ; cr3
         mov cr3, rax

         ; We need to set the stack pointers first, to make sure that we're
         ; pushing onto the stack that iretq will be seeing later on

         mov rsp, [rdi + (8 * 11)]
         mov rbp, [rdi + (8 * 12)]

         ; We also need to save rsp in a register, so that the current
         ; rsp gets set after the iretq--since our pushes will modify it
         mov rax, rsp

         ; Now we push what iretq expects to find on the stack:
         push qword 0x23   ; SS descriptor + privilege level 3
         push rax          ; where we placed the saved rsp value earlier
         push qword [rdi + (8 * 22)]   ; RFLAGS
         push qword 0x1b   ; CS descriptor + privilege level 3
         push qword [rdi + (8 * 21)]   ; RIP

         ; Now we set the rest of the registers to their saved values

         mov rax, [rdi + (8 * 5)]
         mov rbx, [rdi + (8 * 6)]
         mov rcx, [rdi + (8 * 7)]
         mov rdx, [rdi + (8 * 8)]
         mov rsi, [rdi + (8 * 9)]

         ; skip rdi for now, since we need it as-is to continue filling out
         ; the rest of the registers
         mov r8, [rdi + (8 * 13)]
         mov r9, [rdi + (8 * 14)]
         mov r10, [rdi + (8 * 15)]
         mov r11, [rdi + (8 * 16)]
         mov r12, [rdi + (8 * 17)]
         mov r13, [rdi + (8 * 18)]
         mov r14, [rdi + (8 * 19)]
         mov r15, [rdi + (8 * 20)]
         
         iretq
ret