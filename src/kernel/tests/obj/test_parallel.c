//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/kprintf/kprintf.h>
#include <sys/objectinterface/objectinterface_parallel.h>
#include <sys/objectmgr/object.h>
#include <sys/objectmgr/objectmgr.h>
#include <tests/obj/test_parallel.h>
#include <types.h>

void test_parallel() {
    uint8_t devicename[] = {"par0"};

    struct object* parallel = objectmgr_find_object(devicename);
    if (0 != parallel) {
        struct objectinterface_parallel* parallel_api = (struct objectinterface_parallel*)parallel->api;

        (*parallel_api->write)(parallel, "hello", 6);

    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
