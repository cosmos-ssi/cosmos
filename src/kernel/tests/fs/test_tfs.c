//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/fs/tfs/tfs.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objecttype/objecttype_block.h>
#include <sys/string/string.h>
#include <tests/fs/test_tfs.h>

uint8_t FILE1_CONTENT[] = {"This is the file i am i am, this is the file i am"};
uint8_t FILE1_NAME[] = {"Dave"};

void test_tfs() {
    uint8_t devicename[] = {"disk1"};

    struct object* dsk = objectmgr_find_device(devicename);
    if (0 != dsk) {
        struct object* dev = tfs_attach(dsk);
        tfs_detach(dev);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
