//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/syscall/syscalls_objectmgr.h>

#define SYSCALL_SERIAL_DEVICE "serial0"

uint64_t syscall_objectmgr_get_device_by_name(uint64_t syscall_id, struct syscall_args* args) {
    struct object* obj = objectmgr_find_object_by_name(SYSCALL_SERIAL_DEVICE);
    if (0 != obj) {
        return obj->handle;
    }
    return 0;
}

uint64_t syscall_objectmgr_get_device_by_handle(uint64_t syscall_id, struct syscall_args* args) {
    kprintf("syscall %llu\n not implemented", syscall_id);
    return 0;
}
