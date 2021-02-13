/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/devicemgr/devicemgr.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objects/objects.h>

void test_objects() {
    device_t* disk;

    disk = devicemgr_find_device("disk0");
    if (disk) {
        kprintf("disk0 found\n");
    }
    return;
}