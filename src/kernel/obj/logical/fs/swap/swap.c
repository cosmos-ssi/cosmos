//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/block_util.h>
#include <obj/logical/fs/swap/swap.h>
#include <sys/debug/assert.h>
#include <sys/obj/objectmgr/objectmgr.h>

#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/objectinterface/objectinterface_swap.h>
#include <sys/string/mem.h>
#include <types.h>

struct swap_objectdata {
    struct object* block_device;
    uint16_t block_size;  // block size of the underlying physical device
};

/*
 * perform device instance specific init here
 */
uint8_t swap_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct swap_objectdata* object_data = (struct swap_objectdata*)obj->object_data;
    kprintf("Init %s on %s (%s)\n", obj->description, object_data->block_device->name, obj->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t swap_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct swap_objectdata* object_data = (struct swap_objectdata*)obj->object_data;
    kprintf("Uninit %s on %s (%s)\n", obj->description, object_data->block_device->name, obj->name);
    kfree(obj->api);
    kfree(obj->object_data);
    return 1;
}

uint16_t swap_block_size(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct swap_objectdata* object_data = (struct swap_objectdata*)obj->object_data;
    return blockutil_get_sector_size(object_data->block_device);
}

void swap_read(struct object* obj, uint8_t* data, uint32_t block) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct swap_objectdata* object_data = (struct swap_objectdata*)obj->object_data;
    blockutil_read(object_data->block_device, data, swap_block_size(obj), block, 0);
}

void swap_write(struct object* obj, uint8_t* data, uint32_t block) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct swap_objectdata* object_data = (struct swap_objectdata*)obj->object_data;
    blockutil_write(object_data->block_device, data, swap_block_size(obj), block, 0);
}

uint16_t swap_block_count(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct swap_objectdata* object_data = (struct swap_objectdata*)obj->object_data;
    return blockutil_get_sector_count(object_data->block_device);
}

struct object* swap_attach(struct object* block_device) {
    ASSERT_NOT_NULL(block_device);
    ASSERT(1 == blockutil_is_block_object(block_device));
    /*
     * register device
     */
    struct object* objectinstance = objectmgr_new_object();
    objectinstance->init = &swap_init;
    objectinstance->uninit = &swap_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = SWAP;
    objectmgr_set_object_description(objectinstance, "Swap");
    /*
     * the device api
     */
    struct objectinterface_swap* api = (struct objectinterface_swap*)kmalloc(sizeof(struct objectinterface_swap));
    memzero((uint8_t*)api, sizeof(struct objectinterface_swap));
    api->write = &swap_write;
    api->read = &swap_read;
    api->block_count = &swap_block_count;
    api->block_size = &swap_block_size;
    objectinstance->api = api;
    /*
     * device data
     */
    struct swap_objectdata* object_data = (struct swap_objectdata*)kmalloc(sizeof(struct swap_objectdata));
    object_data->block_device = block_device;
    object_data->block_size = blockutil_get_sector_size(block_device);
    objectinstance->object_data = object_data;
    /*
     * register
     */
    if (0 != objectmgr_attach_object(objectinstance)) {
        /*
        * increase ref count of underlying device
        */
        objectmgr_increment_object_refcount(block_device);

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

void swap_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct swap_objectdata* object_data = (struct swap_objectdata*)obj->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->block_device);
    /*
    * detach
    */
    objectmgr_detach_object(obj);
}
