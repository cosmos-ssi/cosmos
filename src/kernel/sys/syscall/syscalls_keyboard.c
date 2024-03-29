//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_keyboard.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/syscall/syscalls_keyboard.h>

uint64_t syscall_keyboard_read(uint64_t syscall_id, struct syscall_args* args) {
    struct object* keyboard_obj = objectmgr_find_object_by_handle(args->arg1);
    if (0 != keyboard_obj) {
        struct objectinterface_keyboard* api = (struct objectinterface_keyboard*)keyboard_obj->api;
        (*api->read)(keyboard_obj);
    }
    return 0;
}
