//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/fs/cfs/cfs.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objecttype/objecttype_block.h>
#include <tests/fs/test_cfs.h>

void test_cfs() {
    uint8_t devicename[] = {"disk1"};

    struct object* dsk = objectmgr_find_device(devicename);
    if (0 != dsk) {
        struct object* dev = cfs_attach(dsk);
        cfs_detach(dev);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
