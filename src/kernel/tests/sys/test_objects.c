/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/logical/fs/initrd/initrd.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_filesystem.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/fs/fs_facade.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objects/objects.h>

void test_objects() {
    /*device_t* disk;
    device_t* initrd;
    uint8_t idx;*/

    kprintf("Testing objects\n");

    /*disk = devicemgr_find_device("disk0");
    ASSERT_NOT_NULL(disk);

    initrd = initrd_attach(disk, initrd_lba());
    ASSERT_NOT_NULL(initrd);

    struct filesystem_node* fs_root_node = fsfacade_get_fs_rootnode(initrd);
    ASSERT_NOT_NULL(fs_root_node);
    struct filesystem_node* fs_file_node = fsfacade_find_node_by_name(fs_root_node, "test.bin");
    ASSERT_NOT_NULL(fs_file_node);

    kprintf("Size: %llu\n", fsfacade_size(fs_file_node));

    idx = fs_file_node->id;
    kprintf("Index: %hu\n", idx);

    object_handle_t pres_handle;

    pres_handle = object_presentation_create(disk, idx, "test.bin");
    object_handle_t exe_handle = object_executable_create_from_presentation(pres_handle);

    kprintf("pres_handle, exe_handle: %llu, %llu\n", pres_handle, exe_handle);

    object_executable_t* obj_exe = OBJECT_DATA(exe_handle, object_executable_t);
    kprintf("page start, len: %llX %llX\n", (uint64_t)obj_exe->page_base, (uint64_t)obj_exe->page_count);

    initrd_detach(initrd);*/

    
    return;
}