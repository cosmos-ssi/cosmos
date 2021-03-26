//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_serial.h>
#include <sys/obj/objectmgr/objectmgr.h>

#define SYSCALL_SERIAL_DEVICE "serial0"

uint64_t syscall_serial_readchar(uint64_t syscall_id, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    struct object* serial_obj = objectmgr_find_object_by_name(SYSCALL_SERIAL_DEVICE);
    if (0 != serial_obj) {
        struct objectinterface_serial* api = (struct objectinterface_serial*)serial_obj->api;
        return (*api->readchar)(serial_obj);
    }
    return 0;
}
uint64_t syscall_serial_writechar(uint64_t syscall_id, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    struct object* serial_obj = objectmgr_find_object_by_name(SYSCALL_SERIAL_DEVICE);
    if (0 != serial_obj) {
        struct objectinterface_serial* api = (struct objectinterface_serial*)serial_obj->api;
        (*api->writechar)(serial_obj, arg1);
    }
    return 0;
}
