//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/fs/node_cache.h>
#include <dev/logical/fs/node_util.h>
#include <dev/logical/fs/vfs/vfs.h>
#include <sys/collection/arraylist/arraylist.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_filesystem.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>

struct vfs_devicedata {
    struct arraylist* children;
    struct filesystem_node* root_node;
};

/*
 * perform device instance specific init here
 */
uint8_t vfs_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Init %s (%s)\n", dev->description, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t vfs_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Uninit %s  (%s)\n", dev->description, dev->name);
    struct vfs_devicedata* device_data = (struct vfs_devicedata*)dev->device_data;

    kfree(dev->api);
    kfree(device_data->root_node);
    kfree(device_data->children);
    kfree(device_data);
    return 1;
}

struct filesystem_node* vfs_get_root_node(struct device* filesystem_device) {
    ASSERT_NOT_NULL(filesystem_device);
    ASSERT_NOT_NULL(filesystem_device->device_data);
    struct vfs_devicedata* device_data = (struct vfs_devicedata*)filesystem_device->device_data;
    return device_data->root_node;
}

uint32_t vfs_read(struct filesystem_node* fs_node, uint8_t* data, uint32_t data_size) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);

    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);
    // read from node. we cant read from the root node, but we can find underlying file and folder nodes
    PANIC("not implemented");
    return 0;
}

uint32_t vfs_write(struct filesystem_node* fs_node, const uint8_t* data, uint32_t data_size) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);

    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);
    // write to node. we cant write to the root node, but we can find underlying file and folder nodes

    PANIC("not implemented");
    return 0;
}

void vfs_open(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);

    PANIC("not implemented");
}

void vfs_close(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);

    PANIC("not implemented");
}

struct filesystem_node* vfs_find_node_by_id(struct filesystem_node* fs_node, uint32_t id) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);
    struct vfs_devicedata* device_data = (struct vfs_devicedata*)fs_node->filesystem_device->device_data;
    //  kprintf("vfs_find_node_by_id node id %llu of node %s\n", id, fs_node->name);
    if (fs_node == device_data->root_node) {
        /*
        * root node
        */
        if (0 == device_data->children) {
            //   kprintf("no chilren\n");
            return 0;
        } else {
            uint32_t child_count = arraylist_count(device_data->children);
            ASSERT(id < child_count);
            for (uint32_t i = 0; i < child_count; i++) {
                struct filesystem_node* n = (struct filesystem_node*)arraylist_get(device_data->children, i);
                //     kprintf("n %llu\n", n->id);
                if (n->id == id) {
                    return n;
                }
            }
            return 0;
        }
    } else {
        kprintf("vfs nodes dont have chilren\n");
        return 0;
    }
}

void vfs_list_directory(struct filesystem_node* fs_node, struct filesystem_directory* dir) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);
    ASSERT_NOT_NULL(dir);
    struct vfs_devicedata* device_data = (struct vfs_devicedata*)fs_node->filesystem_device->device_data;
    if (fs_node == device_data->root_node) {
        /*
        * root node
        */
        if (0 == device_data->children) {
            dir->count = 0;
        } else {
            dir->count = arraylist_count(device_data->children);
            for (uint32_t i = 0; i < dir->count; i++) {
                // node id is the index into the array list
                dir->ids[i] = i;
            }
        }
    } else {
        dir->count = 0;
        /*
        * return  here.  vfsdev has no concept of folders, therefore every node
        * is at the top level; a child of the root
        */
    }
}

uint64_t vfs_size(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);
    // vfs nodes have no size
    return 0;
}

struct device* vfs_attach() {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &vfs_init;
    deviceinstance->uninit = &vfs_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = VFS;
    deviceinstance->device_data = 0;
    devicemgr_set_device_description(deviceinstance, "VFS File System");
    /*
     * the device api
     */
    struct deviceapi_filesystem* api = (struct deviceapi_filesystem*)kmalloc(sizeof(struct deviceapi_filesystem));
    memzero((uint8_t*)api, sizeof(struct deviceapi_filesystem));
    api->close = &vfs_close;
    api->find_id = &vfs_find_node_by_id;
    api->open = &vfs_open;
    api->root = &vfs_get_root_node;
    api->write = &vfs_write;
    api->read = &vfs_read;
    api->list = &vfs_list_directory;
    api->size = &vfs_size;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct vfs_devicedata* device_data = (struct vfs_devicedata*)kmalloc(sizeof(struct vfs_devicedata));
    device_data->root_node = filesystem_node_new(folder, deviceinstance, "vfs", 0, 0);
    device_data->children = arraylist_new();
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
        kfree(device_data->children);
        kfree(device_data->root_node);
        kfree(device_data);
        kfree(api);
        kfree(deviceinstance);
        return 0;
    }
}

void vfs_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    /*
    * detach
    */
    devicemgr_detach_device(dev);
}

void vfs_add_child(struct device* vfs_device, struct filesystem_node* child_node) {
    ASSERT_NOT_NULL(vfs_device);
    ASSERT_NOT_NULL(vfs_device->device_data);
    ASSERT_NOT_NULL(child_node);
    struct vfs_devicedata* device_data = (struct vfs_devicedata*)vfs_device->device_data;
    ASSERT_NOT_NULL(device_data);
    ASSERT_NOT_NULL(device_data->children);
    // the node id will be it's position in teh array
    child_node->id = arraylist_count(device_data->children);
    arraylist_add(device_data->children, child_node);
}

void vfs_remove_child(struct device* vfs_device, uint64_t id) {
    ASSERT_NOT_NULL(vfs_device);
    ASSERT_NOT_NULL(vfs_device->device_data);
    PANIC("not implemented");
}