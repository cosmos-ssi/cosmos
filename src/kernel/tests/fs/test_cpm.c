//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/cpm/cpm.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <types.h>

void test_cpm() {
    uint8_t devicename[] = {"disk3"};  // blank.img

    struct object* dsk = objectmgr_find_object(devicename);
    if (0 != dsk) {
        struct object* obj = cpm_attach(dsk);

        // format to CPM
        //   struct objectinterface_filesystem* api = (struct objectinterface_filesystem*)obj->api;
        //  (*api->format)(obj);

        // detach
        cpm_detach(obj);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
