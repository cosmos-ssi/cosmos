/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/logical/fs/initrd/initrd.h>
#include <sys/debug/assert.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objects/objects.h>

void test_objects() {
    device_t* disk;
    device_t* initrd;
    uint8_t idx;

    kprintf("Testing objects\n");

    disk = devicemgr_find_device("disk0");
    ASSERT_NOT_NULL(disk);

    initrd = initrd_attach(disk, initrd_lba());
    ASSERT_NOT_NULL(initrd);

    ASSERT_NOT_NULL(initrd_find_file(initrd, "test.bin", &idx));
    kprintf("Index: %hu\n", idx);

    object_handle_t pres_handle = object_create_presentation(initrd, idx, "test.bin");
    object_handle_t exe_handle = object_create_executable_from_presentation(pres_handle);

    kprintf("pres_handle, exe_handle: %llu, %llu\n", pres_handle, exe_handle);

    return;
}