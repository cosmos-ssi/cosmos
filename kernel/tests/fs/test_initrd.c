//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/fs/initrd/initrd.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_filesystem.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kprintf/kprintf.h>
#include <tests/fs/test_initrd.h>

void test_initrd() {
    kprintf("Testing initrd\n");

    // boot disk.....
    uint8_t devicename[] = {INITRD_DISK};

    struct device* dsk = devicemgr_find_device(devicename);
    if (0 != dsk) {
        kprintf("lba %llu\n", initrd_lba());

        struct device* initrd = initrd_attach(dsk, initrd_lba());
        ASSERT_NOT_NULL(initrd);

        initrd_dump_dir(initrd);

        // detach
        initrd_detach(initrd);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
