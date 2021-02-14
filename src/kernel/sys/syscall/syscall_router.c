//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/array/array.h>
#include <sys/debug/assert.h>
#include <sys/syscall/syscall_router.h>

struct array* syscalls;

uint64_t syscall_router_route(uint64_t syscall_id) {
    ASSERT_NOT_NULL(syscalls);
    ASSERT(syscall_id < SYSCALL_MAX_SYSCALLS);
    kprintf("syscall %llu\n", syscall_id);
    syscall_handler h = (syscall_handler)array_get(syscalls, syscall_id);
    ASSERT_NOT_NULL(h);
    return (*h)(syscall_id);
}

void syscall_router_init() {
    syscalls = array_new(SYSCALL_MAX_SYSCALLS);
    syscall_router_add(1, &syscall_print_console);
}

void syscall_router_add(uint64_t syscall_id, syscall_handler handler) {
    ASSERT_NOT_NULL(syscalls);
    ASSERT_NOT_NULL(handler);
    ASSERT(syscall_id < SYSCALL_MAX_SYSCALLS);
    array_set(syscalls, syscall_id, handler);
}
