//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/fat/fat.h>
#include <sys/debug/assert.h>
#include <sys/fs/fs_facade.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/objectinterface/objectinterface_filesystem.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/string/string.h>
#include <tests/fs/test_fat.h>
#include <types.h>

void test_fat() {
    uint8_t devicename[] = {"fs0"};

    struct object* fatfs = objectmgr_find_object_by_name(devicename);
    if (0 != fatfs) {

        struct filesystem_node* fs_root_node = fsfacade_get_fs_rootnode(fatfs);
        ASSERT_NOT_NULL(fs_root_node);
        ASSERT(fs_root_node->type == folder);

        struct filesystem_node* fs_file_node = fsfacade_find_node_by_name(fs_root_node, "TESTDATA.TXT");
        ASSERT_NOT_NULL(fs_file_node);
        ASSERT(fs_file_node->type == file);

        uint32_t len = fsfacade_size(fs_file_node);
        ASSERT(len == 66);

        uint8_t data[len + 1];
        fsfacade_read(fs_file_node, data, len);
        data[len] = 0;
        //debug_show_memblock(data, len);
        //kprintf("data '%s'\n", data);
        ASSERT(0 == strcmp(data, "Do not modify this file.  It contains test data for test_initrd.c."));

    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
