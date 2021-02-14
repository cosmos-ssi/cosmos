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

void test_vfs() {
    kprintf("Testing vfs\n");

    struct device* vfsdev_device = devicemgr_find_device("vfs0");
    ASSERT_NOT_NULL(vfsdev_device);

    struct filesystem_node* fsnode = fshelper_get_fs_rootnode(vfsdev_device);
    ASSERT_NOT_NULL(fsnode);
}