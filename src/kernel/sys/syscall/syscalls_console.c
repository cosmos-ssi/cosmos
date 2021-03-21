//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_console.h>
#include <sys/obj/objectmgr/objectmgr.h>

uint64_t syscall_console_write(uint64_t syscall_id, void* args) {
    struct object* console_obj = objectmgr_find_object_by_name("con0");
    if (0 != console_obj) {
        struct objectinterface_console* api = (struct objectinterface_console*)console_obj->api;
        (*api->write)(console_obj, "Hello from console\n");
    }
    return 0;
}
