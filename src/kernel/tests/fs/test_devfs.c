//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/obj/objectmgr/objectmgr.h>

#include <sys/fs/fs_facade.h>
#include <sys/kprintf/kprintf.h>
#include <tests/fs/test_devfs.h>

void test_devfs() {
    kprintf("Testing devfs\n");

    //   struct object* objfs_device = objectmgr_find_object("devfs0");
    //   ASSERT_NOT_NULL(devfs_device);

    //    struct filesystem_node* fsnode = fsfacade_get_fs_rootnode(devfs_device);
    //  ASSERT_NOT_NULL(fsnode);
}