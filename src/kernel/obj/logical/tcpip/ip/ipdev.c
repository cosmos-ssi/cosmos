//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/objecttype/objecttype_ip.h>
#include <sys/string/mem.h>

struct ip_devicedata {
    struct object* ethernet_device;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t ip_init(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct ip_devicedata* object_data = (struct ip_devicedata*)dev->object_data;
    kprintf("Init %s on %s (%s)\n", dev->description, object_data->ethernet_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t ip_uninit(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    kfree(dev->api);
    kfree(dev->object_data);

    return 1;
}

void ip_read(struct object* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    //  struct ip_devicedata* object_data = (struct ip_devicedata*)dev->object_data;
}
void ip_write(struct object* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    //   struct ip_devicedata* object_data = (struct ip_devicedata*)dev->object_data;
}

struct object* ip_attach(struct object* ethernet_device) {
    ASSERT_NOT_NULL(ethernet_device);
    ASSERT(ethernet_device->devicetype == ETHERNET);

    /*
     * register device
     */
    struct object* deviceinstance = objectmgr_new_object();
    deviceinstance->init = &ip_init;
    deviceinstance->uninit = &ip_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = IP;
    objectmgr_set_object_description(deviceinstance, "Internet Protocol");
    /*
     * the device api
     */
    struct objecttype_ip* api = (struct objecttype_ip*)kmalloc(sizeof(struct objecttype_ip));
    memzero((uint8_t*)api, sizeof(struct objecttype_ip));
    api->read = &ip_read;
    api->write = &ip_write;

    deviceinstance->api = api;
    /*
     * device data
     */
    struct ip_devicedata* object_data = (struct ip_devicedata*)kmalloc(sizeof(struct ip_devicedata));
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

void ip_detach(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct ip_devicedata* object_data = (struct ip_devicedata*)dev->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->ethernet_device);
    /*
    * detach
    */
    objectmgr_detach_object(dev);
}

// https://www.saminiir.com/lets-code-tcp-ip-stack-2-ipv4-icmpv4/
// https://tools.ietf.org/html/rfc1071

uint16_t ip_checksum(uint16_t* addr, int count) {
    uint32_t ret = 0;
    while (count > 1) {
        ret += *addr++;
        count -= 2;
    }
    if (count > 0) {
        ret += *(uint8_t*)addr;
    }
    while (ret >> 16) {
        ret = (ret & 0xffff) + (ret >> 16);
    }
    return ~ret;
}

void ip_header_init(struct ip_header* header, uint16_t total_length, uint16_t protocol, uint32_t source,
                    uint32_t dest) {
    ASSERT_NOT_NULL(header);
    ASSERT(sizeof(struct ip_header) == 20);  // ip_header should be 20 bytes
    memzero((uint8_t*)header, sizeof(struct ip_header));
    header->version = 4;
    header->ihl = 5;  // 20 bytes = 160 bits = 5x 32-bits
    header->len = total_length;
    header->ttl = 64;
    header->proto = protocol;
    header->saddr = source;
    header->daddr = dest;
    header->csum = ip_checksum((uint16_t*)header, sizeof(struct ip_header));
}
