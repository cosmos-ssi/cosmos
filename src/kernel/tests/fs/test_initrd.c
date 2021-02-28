//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/initrd/initrd.h>
#include <sys/debug/assert.h>
#include <sys/fs/fs_facade.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objectmgr/objectmgr.h>
#include <sys/objecttype/objecttype_filesystem.h>
#include <sys/string/string.h>
#include <tests/fs/test_initrd.h>
#include <types.h>

void test_initrd() {
    kprintf("Testing initrd\n");

    // boot disk.....
    uint8_t devicename[] = {INITRD_DISK};

    struct object* dsk = objectmgr_find_object(devicename);
    if (0 != dsk) {
        //    kprintf("lba %llu\n", initrd_lba());

        struct object* initrd = initrd_attach(dsk, initrd_lba());
        ASSERT_NOT_NULL(initrd);

        struct filesystem_node* fs_root_node = fsfacade_get_fs_rootnode(initrd);
        ASSERT_NOT_NULL(fs_root_node);
        ASSERT(fs_root_node->type == folder);

        struct filesystem_node* fs_file_node = fsfacade_find_node_by_name(fs_root_node, "testdata.txt");
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

        // detach
        initrd_detach(initrd);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
