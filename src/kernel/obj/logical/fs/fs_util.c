//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/block_util.h>
#include <obj/logical/fs/fat/fat.h>
#include <obj/logical/fs/fs_util.h>
#include <obj/logical/fs/tfs/tfs.h>
#include <obj/logical/partition/partition.h>
#include <obj/logical/partition_table/guid_partition_table.h>
#include <obj/logical/partition_table/mbr_partition_table.h>
#include <sys/debug/assert.h>
#include <sys/objectmgr/objectmgr.h>

#include <sys/objecttype/objecttype_part_table.h>

void fsutil_attach_partition_tables(struct object* block_obj) {
    ASSERT_NOT_NULL(block_obj);
    ASSERT_NOT_NULL(block_obj->api);
    ASSERT(1 == blockutil_is_block_object(block_obj));

    // try to attach gpt
    struct object* gpt = guid_pt_attach(block_obj);
    if (0 == gpt) {
        // maybe its mbr
        mbr_pt_attach(block_obj);
    } else {
        //   guid_pt_dump(gpt);
    }
}

void fsutil_detach_partition_tables(struct object* block_obj) {
    ASSERT_NOT_NULL(block_obj);
    ASSERT_NOT_NULL(block_obj->api);
    ASSERT(1 == blockutil_is_block_object(block_obj));
    PANIC("not implemented");
}

void fsutil_attach_partitions(struct object* partition_table_obj) {
    ASSERT_NOT_NULL(partition_table_obj);
    ASSERT(partition_table_obj->devicetype == PARTITION_TABLE);

    /*
     * mount partition devices
     */
    struct objecttype_part_table* api = (struct objecttype_part_table*)partition_table_obj->api;
    uint32_t num_partitions = (*api->partitions)(partition_table_obj);
    for (uint32_t i = 0; i < num_partitions; i++) {
        uint32_t sector_count = (*api->sectors)(partition_table_obj, i);
        if (sector_count > 0) {
            partition_attach(partition_table_obj, i);
        }
    }
}

void fsutil_detach_partitions(struct object* partition_table_obj) {
    ASSERT_NOT_NULL(partition_table_obj);
    ASSERT(partition_table_obj->devicetype == PARTITION_TABLE);
}

void fsutil_attach_fs(struct object* partition_obj) {
    ASSERT_NOT_NULL(partition_obj);
    ASSERT(partition_obj->devicetype == PARTITION);

    if (0 == fat_attach(partition_obj)) {
        tfs_attach(partition_obj);
    }
}

void fsutil_detach_fs(struct object* partition_obj) {
    ASSERT_NOT_NULL(partition_obj);
    ASSERT(partition_obj->devicetype == PARTITION);
}
