//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/obj/objectinterface/objectinterface_ethernet.h>
#include <sys/obj/objectinterface/objectinterface_nic.h>
#include <sys/string/mem.h>
#include <types.h>

struct ethernet_objectdata {
    struct object* nic_device;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t ethernet_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct ethernet_objectdata* object_data = (struct ethernet_objectdata*)obj->object_data;
    kprintf("Init %s on %s (%s)\n", obj->description, object_data->nic_device->name, obj->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t ethernet_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    kprintf("Uninit %s (%s)\n", obj->description, obj->name);
    kfree(obj->api);
    kfree(obj->object_data);

    return 1;
}

void ethernet_read(struct object* obj, struct eth_hdr* eth, uint16_t size) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    ASSERT_NOT_NULL(eth);
    struct ethernet_objectdata* object_data = (struct ethernet_objectdata*)obj->object_data;
    struct objectinterface_nic* nic_api = (struct objectinterface_nic*)object_data->nic_device->api;
    nic_api->read(object_data->nic_device, (uint8_t*)eth, size);
}

void ethernet_write(struct object* obj, struct eth_hdr* eth, uint16_t size) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    ASSERT_NOT_NULL(eth);
    struct ethernet_objectdata* object_data = (struct ethernet_objectdata*)obj->object_data;
    struct objectinterface_nic* nic_api = (struct objectinterface_nic*)object_data->nic_device->api;
    nic_api->write(object_data->nic_device, (uint8_t*)eth, size);
}

struct object* ethernet_attach(struct object* nic_device) {
    ASSERT_NOT_NULL(nic_device);
    ASSERT((nic_device->objectype == NIC) || (nic_device->objectype == VNIC));
    /*
     * register device
     */
    struct object* objectinstance = objectmgr_new_object();
    objectinstance->init = &ethernet_init;
    objectinstance->uninit = &ethernet_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = ETHERNET;
    objectmgr_set_object_description(objectinstance, "Ethernet");
    /*
     * the device api
     */
    struct objectinterface_ethernet* api =
        (struct objectinterface_ethernet*)kmalloc(sizeof(struct objectinterface_ethernet));
    memzero((uint8_t*)api, sizeof(struct objectinterface_ethernet));
    api->read = &ethernet_read;
    api->write = &ethernet_write;
    objectinstance->api = api;
    /*
     * device data
     */
    struct ethernet_objectdata* object_data = (struct ethernet_objectdata*)kmalloc(sizeof(struct ethernet_objectdata));
    object_data->nic_device = nic_device;
    objectinstance->object_data = object_data;
    /*
     * register
     */
    if (0 != objectmgr_attach_object(objectinstance)) {
        /*
        * increase ref count of underlying device
        */
        objectmgr_increment_object_refcount(nic_device);
        /*
        * return device
        */
        return objectinstance;
    } else {
        kfree(api);
        kfree(object_data);
        kfree(objectinstance);
        return 0;
    }
}

void ethernet_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct ethernet_objectdata* object_data = (struct ethernet_objectdata*)obj->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->nic_device);
    /*
    * detach
    */
    objectmgr_detach_object(obj);
}

void ethernet_init_eth(struct eth_hdr* eth, uint8_t* hw_source, uint8_t* hw_dest, uint8_t* data,
                       uint16_t payload_size) {
    ASSERT_NOT_NULL(eth);
    ASSERT_NOT_NULL(hw_source);
    ASSERT_NOT_NULL(hw_dest);
    ASSERT_NOT_NULL(data);
    ASSERT(payload_size <= 1500);
    memzero((uint8_t*)eth, sizeof(struct eth_hdr));
    memcpy((uint8_t*)&(eth->dest_hw), hw_dest, 6);
    memcpy((uint8_t*)&(eth->source_hw), hw_source, 6);
    eth->data = data;
    eth->type = payload_size;
}
