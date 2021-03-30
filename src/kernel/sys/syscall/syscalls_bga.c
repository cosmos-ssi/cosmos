//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_bga.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/syscall/syscalls_bga.h>

uint64_t syscall_bga_get_resolution(uint64_t syscall_id, struct syscall_args* args) {
    struct object* bga_obj = objectmgr_find_object_by_handle(args->arg1);
    if (0 != bga_obj) {
        struct objectinterface_resolution* res = (struct objectinterface_resolution*)args->arg2;
        struct objectinterface_bga* api = (struct objectinterface_bga*)bga_obj->api;
        (*api->get_resolution)(bga_obj, res);
    }
    return 0;
}

uint64_t syscall_bga_set_resolution(uint64_t syscall_id, struct syscall_args* args) {
    struct object* bga_obj = objectmgr_find_object_by_handle(args->arg1);
    if (0 != bga_obj) {
        struct objectinterface_resolution* res = (struct objectinterface_resolution*)args->arg2;
        struct objectinterface_bga* api = (struct objectinterface_bga*)bga_obj->api;
        (*api->set_resolution)(bga_obj, res);
    }
    return 0;
}

uint64_t syscall_bga_get_buffersize(uint64_t syscall_id, struct syscall_args* args) {
    struct object* bga_obj = objectmgr_find_object_by_handle(args->arg1);
    if (0 != bga_obj) {
        struct objectinterface_bga* api = (struct objectinterface_bga*)bga_obj->api;
        return (*api->get_buffersize)(bga_obj);
    }
    return 0;
}

uint64_t syscall_bga_blt(uint64_t syscall_id, struct syscall_args* args) {
    struct object* bga_obj = objectmgr_find_object_by_handle(args->arg1);
    if (0 != bga_obj) {
        struct objectinterface_bga* api = (struct objectinterface_bga*)bga_obj->api;
        (*api->blt)(bga_obj, (uint8_t*)args->arg2, args->arg3);
    }
    return 0;
}
