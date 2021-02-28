//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/fs/fat/fat.h>
#include <sys/debug/debug.h>
#include <sys/objectmgr/objectmgr.h>

#include <sys/kprintf/kprintf.h>
#include <tests/fs/test_fat.h>

void test_fat() {
    uint8_t devicename[] = {"disk1"};

    struct object* dsk = objectmgr_find_device(devicename);
    if (0 != dsk) {
        struct object* dev = fat_attach(dsk);
        fat_detach(dev);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
