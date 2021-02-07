/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/asm/asm.h>
#include <sys/kprintf/kprintf.h>
#include <sys/x86-64/gdt/gdt.h>
#include <types.h>

void syscall_entry() {
    kprintf("System call!\n");

    asm volatile("sysret" ::: "rcx", "r11");
}

void syscall_init() {
    uint64_t reg_star;
    uint64_t reg_efer;

    reg_efer = asm_rdmsr(MSR_EFER);

    reg_efer |= 1;  // set bit 1 to enable SYSCALL/SYSRET
    asm_wrmsr(MSR_EFER, reg_efer);

    reg_star = 0xFFFFFFFF;

    reg_star |= ((uint64_t)GDT_KERNEL_CODE_SELECTOR << 32);
    reg_star |= ((((uint64_t)GDT_USER_CODE_SELECTOR - 16) | 3) << 48);

    asm_wrmsr(MSR_STAR, reg_star);

    asm_wrmsr(MSR_LSTAR, (uint64_t)&syscall_entry);

    // no flags
    asm_wrmsr(MSR_SFMASK, 0);

    asm volatile("syscall" ::: "rcx", "r11");

    kprintf("Back from syscall\n");
}