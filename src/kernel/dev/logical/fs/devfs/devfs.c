//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/fs/devfs/devfs.h>
#include <dev/logical/fs/node_cache.h>
#include <dev/logical/fs/node_util.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_filesystem.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/devicemgr/devicetypes.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>

struct devfs_devicedata {
    struct filesystem_node* root_node;
    struct node_cache* nc;
};

/*
 * perform device instance specific init here
 */
uint8_t devfs_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Init %s (%s)\n", dev->description, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t devfs_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Uninit %s  (%s)\n", dev->description, dev->name);
    struct devfs_devicedata* device_data = (struct devfs_devicedata*)dev->device_data;
    kfree(dev->api);
    kfree(device_data->root_node);
    node_cache_delete(device_data->nc);
    kfree(device_data);
    return 1;
}

struct filesystem_node* devfs_get_root_node(struct device* filesystem_device) {
    ASSERT_NOT_NULL(filesystem_device);
    ASSERT_NOT_NULL(filesystem_device->device_data);
    struct devfs_devicedata* device_data = (struct devfs_devicedata*)filesystem_device->device_data;
    return device_data->root_node;
}

uint32_t devfs_read(struct filesystem_node* fs_node, const uint8_t* data, uint32_t data_size) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);
    // read from node. we cant read from the root node, but we can find underlying file and folder nodes
    PANIC("not implemented");

    return 0;
}

uint32_t devfs_write(struct filesystem_node* fs_node, const uint8_t* data, uint32_t data_size) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);

    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);
    // write to node. we cant write to the root node, but we can find underlying file and folder nodes
    PANIC("not implemented");

    return 0;
}

void devfs_open(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);

    PANIC("not implemented");
}

void devfs_close(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);

    PANIC("not implemented");
}

struct filesystem_node* devfs_find_node_by_id(struct filesystem_node* fs_node, uint32_t id) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);

    // find subnode.  we can do this for the root node, but not contained nodes b/c devices are leaf nodes
    PANIC("not implemented");

    return 0;
}

void devfs_list_directory(struct filesystem_node* fs_node, struct filesystem_directory* dir) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);
    ASSERT_NOT_NULL(dir);
    struct devfs_devicedata* device_data = (struct devfs_devicedata*)fs_node->filesystem_device->device_data;
    if (fs_node == device_data->root_node) {
        /*
        * root node
        */

        //dir->count = devicetypes_count();

    } else {
        /*
        * folders are device types
        */
        if (fs_node->type == folder) {

        } else {
            // its a leaf node which is a device. it has no children
        }
    }
}

struct device* devfs_attach() {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &devfs_init;
    deviceinstance->uninit = &devfs_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = DEVFS;
    deviceinstance->device_data = 0;
    devicemgr_set_device_description(deviceinstance, "Device File System");
    /*
     * the device api
     */
    struct deviceapi_filesystem* api = (struct deviceapi_filesystem*)kmalloc(sizeof(struct deviceapi_filesystem));
    memzero((uint8_t*)api, sizeof(struct deviceapi_filesystem));
    api->close = &devfs_close;
    api->find_id = &devfs_find_node_by_id;
    api->open = &devfs_open;
    api->root = &devfs_get_root_node;
    api->write = &devfs_write;
    api->read = &devfs_read;
    api->list = &devfs_list_directory;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct devfs_devicedata* device_data = (struct devfs_devicedata*)kmalloc(sizeof(struct devfs_devicedata));
    device_data->root_node = filesystem_node_new(folder, deviceinstance, "devfs", 0, 0);
    device_data->nc = node_cache_new();
    deviceinstance->device_data = device_data;
    /*
     * register
     */
    if (0 != devicemgr_attach_device(deviceinstance)) {
        /*
        * return device
        */
        return deviceinstance;
    } else {
        node_cache_delete(device_data->nc);
        kfree(device_data->root_node);
        kfree(device_data);
        kfree(api);
        kfree(deviceinstance);
        return 0;
    }
}

void devfs_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    /*
    * detach
    */
    devicemgr_detach_device(dev);
}
