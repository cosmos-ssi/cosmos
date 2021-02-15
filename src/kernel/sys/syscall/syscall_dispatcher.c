/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/debug/assert.h>
#include <sys/kprintf/kprintf.h>
#include <sys/panic/panic.h>
#include <sys/syscall/syscall.h>
#include <types.h>

uint64_t (*syscall_table[SYSCALL_MAX])(void* args);

void invalid_syscall() {
    PANIC("Invalid system call!");

    return;
}

uint64_t syscall_dispatcher(uint64_t syscall_num, void* args) {

    /*
     * I'm not using ASSERT macro here--my understanding is that those macros will
     * expand to nothing in a release; since at this point we're dealing with
     * data from userspace, we can't trust it.
     */
    if (syscall_num >= SYSCALL_MAX) {
        invalid_syscall();
    }

    kprintf("Syscall, args: %llu, %llX\n", syscall_num, (uint64_t)args);

    return syscall_table[syscall_num](args);
}

void syscall_dispatcher_init() {
    syscall_table[SYSCALL_EXIT] = sys_exit;

    return;
}