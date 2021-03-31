//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_filesystem.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/syscall/syscalls_filesystem.h>

uint64_t syscall_filesystem_get_root_node(uint64_t syscall_id, struct syscall_args* args) {
    struct object* filesystem_obj = objectmgr_find_object_by_handle(args->arg1);
    if (0 != filesystem_obj) {
        struct objectinterface_filesystem* api = (struct objectinterface_filesystem*)filesystem_obj->api;
        ASSERT_NOT_NULL(api);
        //   (*api->read)(filesystem_obj);
    }
    return 0;
}

uint64_t syscall_filesystem_read(uint64_t syscall_id, struct syscall_args* args) {
    struct object* filesystem_obj = objectmgr_find_object_by_handle(args->arg1);
    if (0 != filesystem_obj) {
        struct objectinterface_filesystem* api = (struct objectinterface_filesystem*)filesystem_obj->api;
        ASSERT_NOT_NULL(api);
        //   (*api->read)(filesystem_obj);
    }
    return 0;
}

uint64_t syscall_filesystem_write(uint64_t syscall_id, struct syscall_args* args) {
    struct object* filesystem_obj = objectmgr_find_object_by_handle(args->arg1);
    if (0 != filesystem_obj) {
        struct objectinterface_filesystem* api = (struct objectinterface_filesystem*)filesystem_obj->api;
        ASSERT_NOT_NULL(api);
        //   (*api->read)(filesystem_obj);
    }
    return 0;
}

uint64_t syscall_filesystem_open(uint64_t syscall_id, struct syscall_args* args) {
    struct object* filesystem_obj = objectmgr_find_object_by_handle(args->arg1);
    if (0 != filesystem_obj) {
        struct objectinterface_filesystem* api = (struct objectinterface_filesystem*)filesystem_obj->api;
        ASSERT_NOT_NULL(api);
        //   (*api->read)(filesystem_obj);
    }
    return 0;
}

uint64_t syscall_filesystem_close(uint64_t syscall_id, struct syscall_args* args) {
    struct object* filesystem_obj = objectmgr_find_object_by_handle(args->arg1);
    if (0 != filesystem_obj) {
        struct objectinterface_filesystem* api = (struct objectinterface_filesystem*)filesystem_obj->api;
        ASSERT_NOT_NULL(api);
        //   (*api->read)(filesystem_obj);
    }
    return 0;
}

uint64_t syscall_filesystem_find_node_by_id(uint64_t syscall_id, struct syscall_args* args) {
    struct object* filesystem_obj = objectmgr_find_object_by_handle(args->arg1);
    if (0 != filesystem_obj) {
        struct objectinterface_filesystem* api = (struct objectinterface_filesystem*)filesystem_obj->api;
        ASSERT_NOT_NULL(api);
        //   (*api->read)(filesystem_obj);
    }
    return 0;
}

uint64_t syscall_filesystem_list_directory(uint64_t syscall_id, struct syscall_args* args) {
    struct object* filesystem_obj = objectmgr_find_object_by_handle(args->arg1);
    if (0 != filesystem_obj) {
        struct objectinterface_filesystem* api = (struct objectinterface_filesystem*)filesystem_obj->api;
        ASSERT_NOT_NULL(api);
        //   (*api->read)(filesystem_obj);
    }
    return 0;
}
