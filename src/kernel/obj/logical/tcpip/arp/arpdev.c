//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/objecttype/objecttype_arp.h>
#include <sys/objecttype/objecttype_ethernet.h>
#include <sys/panic/panic.h>
#include <sys/string/mem.h>
#include <types.h>

struct arp_objectdata {
    struct object* ethernet_device;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t arp_init(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct arp_objectdata* object_data = (struct arp_objectdata*)dev->object_data;
    kprintf("Init %s on %s (%s)\n", dev->description, object_data->ethernet_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t arp_uninit(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    kfree(dev->api);
    kfree(dev->object_data);

    return 1;
}

/*
* perform an ARP request
*/
void arp_request(struct object* dev, struct arp* request, struct arp* response) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    // get our device data for this device
    //   struct arp_objectdata* object_data = (struct arp_objectdata*)dev->object_data;
    // get the api for the underlying ethernet device
    //   struct objecttype_ethernet* ether_api = (struct objecttype_ethernet*)object_data->ethernet_device->api;
    PANIC("Um, what HW addresses do I put in here?");
    // send
    //  (*ether_api->write)(object_data->ethernet_device, (uint8_t*)request, sizeof(struct arp));
    // receive. I presume this blocks?
    // (*ether_api->read)(object_data->ethernet_device, (uint8_t*)response, sizeof(struct arp));
}

struct object* arp_attach(struct object* ethernet_device) {
    ASSERT_NOT_NULL(ethernet_device);
    ASSERT(ethernet_device->devicetype == ETHERNET);

    /*
     * register device
     */
    struct object* deviceinstance = objectmgr_new_object();
    deviceinstance->init = &arp_init;
    deviceinstance->uninit = &arp_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = ARP;
    objectmgr_set_object_description(deviceinstance, "Address Resolution Protocol");
    /*
     * the device api
     */
    struct objecttype_arp* api = (struct objecttype_arp*)kmalloc(sizeof(struct objecttype_arp));
    memzero((uint8_t*)api, sizeof(struct objecttype_arp));
    api->request = &arp_request;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct arp_objectdata* object_data = (struct arp_objectdata*)kmalloc(sizeof(struct arp_objectdata));
    object_data->ethernet_device = ethernet_device;
    deviceinstance->object_data = object_data;
    /*
     * register
     */
    if (0 != objectmgr_attach_object(deviceinstance)) {
        /*
        * increase ref count of underlying device
        */
        objectmgr_increment_object_refcount(ethernet_device);
        /*
        * return device
        */
        return deviceinstance;
    } else {
        kfree(api);
        kfree(object_data);
        kfree(deviceinstance);
        return 0;
    }
}

void arp_detach(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct arp_objectdata* object_data = (struct arp_objectdata*)dev->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->ethernet_device);
    /*
    * detach
    */
    objectmgr_detach_object(dev);
}

void arp_packet_init(struct arp* a, uint16_t opcode) {
    ASSERT_NOT_NULL(a);
    ASSERT(sizeof(struct arp) == 96);
    memset((uint8_t*)a, 0, sizeof(struct arp));
    a->htype = ARP_ETHERNET;
    a->ptype = ARP_IP;
    a->hlen = ARP_HLEN;
    a->plen = ARP_PLEN;
    a->opcode = opcode;
}
