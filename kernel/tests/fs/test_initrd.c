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
#include <tests/fs/ramdisk_helper.h>
#include <tests/fs/test_initrd.h>

void test_initrd() {
    kprintf("Testing initrd\n");

    // make rd
    struct device* rd = ramdisk_helper_create_rd();
    ASSERT_NOT_NULL(rd);

    struct device* initrd = initrd_attach(rd);
    ASSERT_NOT_NULL(initrd);
    struct deviceapi_filesystem* fs_api = (struct deviceapi_filesystem*)initrd->api;
    ASSERT_NOT_NULL(fs_api);
    (*fs_api->format)(initrd);

    initrd_detach(initrd);

    // remove rd
    ramdisk_helper_remove_rd(rd);
}
