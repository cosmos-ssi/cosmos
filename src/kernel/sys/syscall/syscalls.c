//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/kprintf/kprintf.h>
#include <sys/objectmgr/object.h>
#include <sys/objectmgr/objectmgr.h>
#include <sys/objecttype/objecttype_console.h>
#include <sys/syscall/syscalls.h>

uint64_t invalid_syscall(uint64_t syscall_id, void* args) {
    kprintf("Invalid syscall %llu\n", syscall_id);
    return 0;
}

uint64_t syscall_exit(uint64_t syscall_id, void* args) {
    // exit
    return 0;
}

uint64_t syscall_print_console(uint64_t syscall_id, void* args) {
    struct object* console_dev = objectmgr_find_object("console0");
    if (0 != console_dev) {
        struct objecttype_console* api = (struct objecttype_console*)console_dev->api;
        (*api->write)(console_dev, "Hello from console\n");
    }
    return 0;
}

uint64_t syscall_malloc(uint64_t syscall_id, void* args) {
    kprintf("syscall %llu\n not implemented", syscall_id);
    return 0;
}

uint64_t syscall_free(uint64_t syscall_id, void* args) {
    kprintf("syscall %llu\n not implemented", syscall_id);
    return 0;
}
uint64_t syscall_realloc(uint64_t syscall_id, void* args) {
    kprintf("syscall %llu\n not implemented", syscall_id);
    return 0;
}

uint64_t syscall_sleep(uint64_t syscall_id, void* args) {
    kprintf("syscall %llu\n not implemented", syscall_id);
    return 0;
}
