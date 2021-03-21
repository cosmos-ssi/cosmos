/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/debug/assert.h>
#include <sys/kprintf/kprintf.h>
#include <sys/syscall/syscall.h>
#include <sys/syscall/syscall_dispatcher.h>
#include <sys/syscall/syscalls_console.h>
#include <sys/syscall/syscalls_memory.h>
#include <sys/syscall/syscalls_process.h>
#include <types.h>

syscall_handler syscall_table[SYSCALL_MAX];

uint64_t syscall_dispatcher(uint64_t syscall_num, void* args) {

    /*
     * I'm not using ASSERT macro here--my understanding is that those macros will
     * expand to nothing in a release; since at this point we're dealing with
     * data from userspace, we can't trust it.
     */
    if (syscall_num >= SYSCALL_MAX) {
        return invalid_syscall(syscall_num, args);
    }

    if (0 != syscall_table[syscall_num]) {
        return syscall_table[syscall_num](syscall_num, args);
    } else {
        return invalid_syscall(syscall_num, args);
    }
}

void syscall_add(uint64_t syscall_num, syscall_handler handler) {
    ASSERT(syscall_num < SYSCALL_MAX);
    ASSERT_NOT_NULL(handler);
    syscall_table[syscall_num] = handler;
}

void syscall_dispatcher_init() {
    syscall_add(SYSCALL_PROCESS_EXIT, &syscall_process_exit);
    syscall_add(SYSCALL_PROCESS_SLEEP, &syscall_process_sleep);
    syscall_add(SYSCALL_CONSOLE_WRITE, &syscall_console_write);
    syscall_add(SYSCALL_MEMORY_MALLOC, &syscall_memory_malloc);
    syscall_add(SYSCALL_MEMORY_FREE, &syscall_memory_free);
    syscall_add(SYSCALL_MEMORY_REALLOC, &syscall_memory_realloc);
}