//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/fs/rootfs/rootfs.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_filesystem.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>

/*
 * perform device instance specific init here
 */
uint8_t rootfs_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Init %s (%s)\n", dev->description, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t rootfs_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Uninit %s  (%s)\n", dev->description, dev->name);
    kfree(dev->api);
    kfree(dev->device_data);
    return 1;
}

void rootfs_filesystem_open(struct device* filesystem_device) {
    ASSERT_NOT_NULL(filesystem_device);
}
void rootfs_filesystem_close(struct device* filesystem_device) {
    ASSERT_NOT_NULL(filesystem_device);
}

struct filesystem_node* rootfs_filesystem_find_node_by_id(struct device* filesystem_device, uint32_t idx) {
    ASSERT_NOT_NULL(filesystem_device);
    return 0;
}

struct filesystem_node* rootfs_filesystem_find_node_by_name(struct device* filesystem_device, uint8_t* name) {
    ASSERT_NOT_NULL(filesystem_device);
    return 0;
}

struct device* rootfs_attach() {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &rootfs_init;
    deviceinstance->uninit = &rootfs_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = ROOTFS;
    deviceinstance->device_data = 0;
    devicemgr_set_device_description(deviceinstance, "Root File System");
    /*
     * the device api
     */
    struct deviceapi_filesystem* api = (struct deviceapi_filesystem*)kmalloc(sizeof(struct deviceapi_filesystem));
    memzero((uint8_t*)api, sizeof(struct deviceapi_filesystem));
    api->close = &rootfs_filesystem_close;
    api->find_id = &rootfs_filesystem_find_node_by_id;
    api->find_name = &rootfs_filesystem_find_node_by_name;
    api->open = &rootfs_filesystem_open;
    api->read = 0;
    api->write = 0;
    deviceinstance->api = api;
    /*
     * register
     */
    if (0 != devicemgr_attach_device(deviceinstance)) {
        /*
        * return device
        */
        return deviceinstance;
    } else {
        kfree(api);
        kfree(deviceinstance);
        return 0;
    }
}

void rootfs_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    /*
    * detach
    */
    devicemgr_detach_device(dev);
}
