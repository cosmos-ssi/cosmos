//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/fs/fs_facade.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/objectinterface/objectinterface_filesystem.h>
#include <sys/obj/objectmgr/objectmgr.h>

void test_voh() {
    kprintf("Testing voh\n");

    struct object* vohdev_device = objectmgr_find_object_by_name("voh0");
    ASSERT_NOT_NULL(vohdev_device);

    struct filesystem_node* fsnode = fsfacade_get_fs_rootnode(vohdev_device);
    ASSERT_NOT_NULL(fsnode);
    ASSERT(fsnode->type == folder);
}