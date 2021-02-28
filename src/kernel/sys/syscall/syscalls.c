//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/kprintf/kprintf.h>
#include <sys/objectinterface/objectinterface_console.h>
#include <sys/objectmgr/object.h>
#include <sys/objectmgr/objectmgr.h>
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
    struct object* console_obj = objectmgr_find_object("console0");
    if (0 != console_obj) {
        struct objectinterface_console* api = (struct objectinterface_console*)console_obj->api;
        (*api->write)(console_obj, "Hello from console\n");
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
