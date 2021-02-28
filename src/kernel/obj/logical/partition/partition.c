//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/block_util.h>
#include <obj/logical/fs/fs_util.h>
#include <obj/logical/partition/partition.h>
#include <obj/logical/partition_table/partition_table_util.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/obj/objectinterface/objectinterface_part_table.h>
#include <sys/string/mem.h>

struct partition_objectdata {
    struct object* partition_table_objice;
    uint8_t type[64];  // type string
    uint8_t partition_index;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t partition_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct partition_objectdata* object_data = (struct partition_objectdata*)obj->object_data;

    struct objectinterface_part_table* pt_api =
        (struct objectinterface_part_table*)object_data->partition_table_objice->api;
    (*pt_api->type)(object_data->partition_table_objice, object_data->partition_index, (object_data->type), 64);

    kprintf("Init %s on %s index %llu of type %s (%s)\n", obj->description, object_data->partition_table_objice->name,
            object_data->partition_index, object_data->type, obj->name);

    // attach fs
    fsutil_attach_fs(obj);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t partition_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct partition_objectdata* object_data = (struct partition_objectdata*)obj->object_data;
    kprintf("Uninit %s on %s (%s)\n", obj->description, object_data->partition_table_objice->name, obj->name);
    // detach fs
    fsutil_detach_fs(obj);

    kfree(obj->api);
    kfree(obj->object_data);
    return 1;
}

uint16_t partition_sector_size(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct partition_objectdata* object_data = (struct partition_objectdata*)obj->object_data;
    return partition_table_util_sector_size(obj, object_data->partition_index);
}

uint32_t partition_total_size(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct partition_objectdata* object_data = (struct partition_objectdata*)obj->object_data;
    return partition_table_util_total_size(obj, object_data->partition_index);
}

uint32_t partition_read_sectors(struct object* obj, uint8_t* data, uint32_t data_size, uint32_t start_lba) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);

    ASSERT_NOT_NULL(obj->object_data);
    struct partition_objectdata* object_data = (struct partition_objectdata*)obj->object_data;
    return partition_table_util_read_sectors(obj, object_data->partition_index, data, data_size, start_lba);
}

uint32_t partition_write_sectors(struct object* obj, uint8_t* data, uint32_t data_size, uint32_t start_lba) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);

    ASSERT_NOT_NULL(obj->object_data);
    struct partition_objectdata* object_data = (struct partition_objectdata*)obj->object_data;
    return partition_table_util_write_sectors(obj, object_data->partition_index, data, data_size, start_lba);
}

struct object* partition_attach(struct object* partition_table_objice, uint8_t partition_index) {
    ASSERT_NOT_NULL(partition_table_objice);
    ASSERT(partition_table_objice->objectype == PARTITION_TABLE);
    /*
     * register device
     */
    struct object* objectinstance = object_new_object();
    objectinstance->init = &partition_init;
    objectinstance->uninit = &partition_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = PARTITION;
    objectmgr_set_object_description(objectinstance, "Partition");
    /*
     * the device api
     */
    struct objectinterface_block* api = (struct objectinterface_block*)kmalloc(sizeof(struct objectinterface_block));
    memzero((uint8_t*)api, sizeof(struct objectinterface_block));
    api->sector_size = &partition_sector_size;
    api->total_size = &partition_total_size;
    api->read = &partition_read_sectors;
    api->write = &partition_write_sectors;
    objectinstance->api = api;
    /*
     * device data
     */
    struct partition_objectdata* object_data =
        (struct partition_objectdata*)kmalloc(sizeof(struct partition_objectdata));
    memzero((uint8_t*)object_data, sizeof(struct partition_objectdata));
    object_data->partition_table_objice = partition_table_objice;
    object_data->partition_index = partition_index;

    objectinstance->object_data = object_data;

    /*
     * register
     */
    if (0 != objectmgr_attach_object(objectinstance)) {
        /*
        * increase ref count of underlying device
        */
        objectmgr_increment_object_refcount(partition_table_objice);
        /*
        * return device
        */
        return objectinstance;
    } else {
        kfree(object_data);
        kfree(api);
        kfree(objectinstance);
        return 0;
    }
}

void partition_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct partition_objectdata* object_data = (struct partition_objectdata*)obj->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->partition_table_objice);
    /*
    * detach
    */
    objectmgr_detach_object(obj);
}
