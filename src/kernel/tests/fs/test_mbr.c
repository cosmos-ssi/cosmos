//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/partition_table/mbr_partition_table.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_part_table.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <tests/fs/test_mbr.h>
#include <types.h>

void test_mbr() {
    uint8_t devicename[] = {"disk1"};

    struct object* dsk = objectmgr_find_object_by_name(devicename);
    if (0 != dsk) {
        struct object* obj = mbr_pt_attach(dsk);

        struct objectinterface_part_table* api = (struct objectinterface_part_table*)obj->api;

        uint8_t number_partitions = (*api->partitions)(obj);
        kprintf("number_partitions %llu\n", number_partitions);

        //    uint64_t lba = (*api->lba)(obj, 0);
        //      uint64_t type = (*api->type)(obj, 0);
        //    kprintf("partition 0 type %llu lba %llu\n", type, lba);

        mbr_pt_detach(obj);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
