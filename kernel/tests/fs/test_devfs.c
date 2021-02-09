//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/fs/fs_helper.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/string.h>
#include <tests/fs/test_devfs.h>

void test_devfs() {
    kprintf("Testing devfs\n");

    struct device* devfs_device = devicemgr_find_device("devfs0");
    ASSERT_NOT_NULL(devfs_device);

    struct filesystem_node* fsnode = fshelper_get_fs_rootnode(devfs_device);
    ASSERT_NOT_NULL(fsnode);
}