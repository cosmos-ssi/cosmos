//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/cfs/cfs.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/objectinterface/objectinterface_block.h>
#include <tests/fs/test_cfs.h>
#include <types.h>

void test_cfs() {
    uint8_t devicename[] = {"disk1"};

    struct object* dsk = objectmgr_find_object(devicename);
    if (0 != dsk) {
        struct object* obj = cfs_attach(dsk);
        cfs_detach(obj);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
