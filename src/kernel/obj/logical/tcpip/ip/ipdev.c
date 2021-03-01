//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_ip.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/obj/objecttype/objectype.h>
#include <sys/string/mem.h>
#include <types.h>

struct ip_objectdata {
    struct object* ethernet_device;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t ip_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct ip_objectdata* object_data = (struct ip_objectdata*)obj->object_data;
    kprintf("Init %s on %s (%s)\n", obj->description, object_data->ethernet_device->name, obj->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t ip_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    kprintf("Uninit %s (%s)\n", obj->description, obj->name);
    kfree(obj->api);
    kfree(obj->object_data);

    return 1;
}

void ip_read(struct object* obj, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    //  struct ip_objectdata* object_data = (struct ip_objectdata*)obj->object_data;
}
void ip_write(struct object* obj, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    //   struct ip_objectdata* object_data = (struct ip_objectdata*)obj->object_data;
}

struct object* ip_attach(struct object* ethernet_device) {
    ASSERT_NOT_NULL(ethernet_device);
    ASSERT(ethernet_device->objectype == OBJECT_TYPE_ETHERNET);

    /*
     * register device
     */
    struct object* objectinstance = object_new_object();
    objectinstance->init = &ip_init;
    objectinstance->uninit = &ip_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = OBJECT_TYPE_IP;
    objectmgr_set_object_description(objectinstance, "Internet Protocol");
    /*
     * the device api
     */
    struct objectinterface_ip* api = (struct objectinterface_ip*)kmalloc(sizeof(struct objectinterface_ip));
    memzero((uint8_t*)api, sizeof(struct objectinterface_ip));
    api->read = &ip_read;
    api->write = &ip_write;

    objectinstance->api = api;
    /*
     * device data
     */
    struct ip_objectdata* object_data = (struct ip_objectdata*)kmalloc(sizeof(struct ip_objectdata));
    object_data->ethernet_device = ethernet_device;
    objectinstance->object_data = object_data;
    /*
     * register
     */
    if (0 != objectmgr_attach_object(objectinstance)) {
        /*
        * increase ref count of underlying device
        */
        objectmgr_increment_object_refcount(ethernet_device);
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

void ip_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct ip_objectdata* object_data = (struct ip_objectdata*)obj->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->ethernet_device);
    /*
    * detach
    */
    objectmgr_detach_object(obj);
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
