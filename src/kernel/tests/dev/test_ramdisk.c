//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/ramdisk/ramdisk.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kprintf/kprintf.h>
#include <tests/dev/test_blockdevice.h>
#include <tests/dev/test_ramdisk.h>

#define RAMDISK_SECTOR_SIZE 512
#define RAMDISK_TOTAL_SECTORS 1000

void test_ramdisk() {
    kprintf("Testing RAM Disk\n");

    // attach the ramdisk
    struct device* ramdisk_device = ramdisk_attach(RAMDISK_SECTOR_SIZE, RAMDISK_TOTAL_SECTORS);

    struct device* ramdisk = devicemgr_find_device(ramdisk_device->name);
    if (0 != ramdisk) {
        test_block_device(ramdisk);
    } else {
        kprintf("Unable to find %s\n", ramdisk_device->name);
    }

    // detach the ramdisk
    ramdisk_detach(ramdisk_device);
}
