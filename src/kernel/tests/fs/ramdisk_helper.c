//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/ramdisk/ramdisk.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kprintf/kprintf.h>
#include <tests/fs/ramdisk_helper.h>
#include <sys/devicemgr/device.h>

/*
* make a ram disk
*/
struct device* ramdisk_helper_create_rd() {
    // attach the ramdisk
    struct device* ramdisk_device = ramdisk_attach(RAMDISK_SECTOR_SIZE, RAMDISK_TOTAL_SECTORS);

    struct device* ramdisk = devicemgr_find_device(ramdisk_device->name);
    if (0 != ramdisk) {
        return ramdisk;
    } else {
        kprintf("Unable to find %s\n", ramdisk_device->name);
        return 0;
    }
}

/*
* remove the ram disk
*/
void ramdisk_helper_remove_rd(struct device* rd) {
    ramdisk_detach(rd);
}