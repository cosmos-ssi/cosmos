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
#include <sys/syscall/syscalls_bga.h>
#include <sys/syscall/syscalls_console.h>
#include <sys/syscall/syscalls_filesystem.h>
#include <sys/syscall/syscalls_keyboard.h>
#include <sys/syscall/syscalls_memory.h>
#include <sys/syscall/syscalls_objectmgr.h>
#include <sys/syscall/syscalls_process.h>
#include <sys/syscall/syscalls_serial.h>
#include <types.h>

syscall_handler syscall_table[SYSCALL_MAX];

uint64_t syscall_dispatcher(uint64_t syscall_num, struct syscall_args* args) {

    //    kprintf("Syscall %llu\n", syscall_num);
    //    kprintf("Arg1 %#llX, Arg2 %#llX, Arg3 %#llX\n", args->arg1, args->arg2, args->arg3);

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
    // process
    syscall_add(SYSCALL_PROCESS_EXIT, &syscall_process_exit);
    syscall_add(SYSCALL_PROCESS_SLEEP, &syscall_process_sleep);
    // bga
    syscall_add(SYSCALL_BGA_GET_RESOLUTION, &syscall_bga_get_resolution);
    syscall_add(SYSCALL_BGA_SET_RESOLUTION, &syscall_bga_set_resolution);
    syscall_add(SYSCALL_BGA_GET_BUFFERSIZE, &syscall_bga_get_buffersize);
    syscall_add(SYSCALL_BGA_BLT, &syscall_bga_blt);
    // console
    syscall_add(SYSCALL_CONSOLE_WRITE, &syscall_console_write);
    // filesystem
    syscall_add(SYSCALL_FILESYSTEM_GET_ROOT_NODE, &syscall_filesystem_get_root_node);
    syscall_add(SYSCALL_FILESYSTEM_READ, &syscall_filesystem_read);
    syscall_add(SYSCALL_FILESYSTEM_WRITE, &syscall_filesystem_write);
    syscall_add(SYSCALL_FILESYSTEM_OPEN, &syscall_filesystem_open);
    syscall_add(SYSCALL_FILESYSTEM_CLOSE, &syscall_filesystem_close);
    syscall_add(SYSCALL_FILESYSTEM_LIST_DIRECTORY, &syscall_filesystem_list_directory);
    syscall_add(SYSCALL_FILESYSTEM_FIND_NODE_BY_ID, &syscall_filesystem_find_node_by_id);
    // serial
    syscall_add(SYSCALL_SERIAL_READCHAR, &syscall_serial_readchar);
    syscall_add(SYSCALL_SERIAL_WRITECHAR, &syscall_serial_writechar);
    // memory
    syscall_add(SYSCALL_MEMORY_MALLOC, &syscall_memory_malloc);
    syscall_add(SYSCALL_MEMORY_FREE, &syscall_memory_free);
    syscall_add(SYSCALL_MEMORY_REALLOC, &syscall_memory_realloc);
    // objectmgr
    syscall_add(SYSCALL_OBJMGR_GET_DEVICE_BY_NAME, &syscall_objectmgr_get_device_by_name);
    syscall_add(SYSCALL_OBJMGR_GET_DEVICE_BY_HANDLE, &syscall_objectmgr_get_device_by_handle);
    // keyboard
    syscall_add(SYSCALL_KEYBOARD_READ, &syscall_keyboard_read);
}