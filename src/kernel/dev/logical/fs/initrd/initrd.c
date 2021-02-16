//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/fs/block_util.h>
#include <dev/logical/fs/initrd/initrd.h>
#include <dev/logical/fs/node_cache.h>
#include <dev/logical/fs/node_util.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_filesystem.h>
#include <sys/devicemgr/device.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/panic/panic.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>

struct initrd_file_header {
    uint8_t magic;
    uint8_t name[INITRD_NAME_SIZE];
    uint32_t offset;  // byte count from start of initrd
    uint32_t length;  // byte length
};

struct initrd_header {
    uint32_t number_files;
    struct initrd_file_header headers[INITRD_MAX_FILES];
};

struct initrd_devicedata {
    struct device* partition_device;
    uint32_t lba;
    struct initrd_header header;
    struct filesystem_node* root_node;
    struct node_cache* nc;
};

/*
 * perform device instance specific init here
 */
uint8_t initrd_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct initrd_devicedata* device_data = (struct initrd_devicedata*)dev->device_data;
    /*
    * read the header
    */
    blockutil_read(device_data->partition_device, (uint8_t*)&(device_data->header), sizeof(struct initrd_header),
                   device_data->lba);

    kprintf("Init %s on %s (%s)\n", dev->description, device_data->partition_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t initrd_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct initrd_devicedata* device_data = (struct initrd_devicedata*)dev->device_data;
    kprintf("Uninit %s on %s (%s)\n", dev->description, device_data->partition_device->name, dev->name);
    kfree(dev->api);
    node_cache_delete(device_data->nc);
    kfree(device_data->root_node);
    kfree(device_data);
    return 1;
}

struct filesystem_node* initrd_get_root_node(struct device* filesystem_device) {
    ASSERT_NOT_NULL(filesystem_device);
    ASSERT_NOT_NULL(filesystem_device->device_data);
    struct initrd_devicedata* device_data = (struct initrd_devicedata*)filesystem_device->device_data;
    return device_data->root_node;
}

uint32_t initrd_read(struct filesystem_node* fs_node, uint8_t* data, uint32_t data_size) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);

    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);
    struct initrd_devicedata* device_data = (struct initrd_devicedata*)fs_node->filesystem_device->device_data;
    if (fs_node == device_data->root_node) {
        /*
        * cant read or write root node
        */
        return 0;
    } else {
        /*
        * get underlying sector size
        */
        uint32_t sector_size = blockutil_get_sector_size(device_data->partition_device);

        uint32_t offset = device_data->header.headers[fs_node->id].offset;
        ASSERT(offset > 0);
        uint32_t length = device_data->header.headers[fs_node->id].length;
        ASSERT(data_size >= length);
        //  kprintf("offset %llu length %llu\n", offset, length);

        uint32_t lba_offset = offset / sector_size;
        uint32_t byte_offset = offset % sector_size;
        uint32_t total_sectors = length / sector_size;
        if ((length % 512) != 0) {
            total_sectors += 1;
        }
        // if it spans sectors
        if (byte_offset + length > 512) {
            total_sectors += 1;
        }
        uint32_t buffer_size = total_sectors * sector_size;
        uint8_t* buffer = kmalloc(buffer_size);
        memzero(buffer, buffer_size);
        uint32_t target_lba = device_data->lba + lba_offset;
        //  kprintf("lba_offset %llu byte_offset %llu total_sectors %llu target_lba %llu buffer_size %llu\n", lba_offset,
        //       byte_offset, total_sectors, target_lba, buffer_size);

        /*
        * read the blocks
        */
        blockutil_read(device_data->partition_device, buffer, buffer_size, target_lba);

        /*
        * copy the data
        */
        memcpy(data, (uint8_t*)&(buffer[byte_offset]), data_size);

        /*
        * release the buffer
        */
        kfree(buffer);
        return 1;
    }
}

uint32_t initrd_write(struct filesystem_node* fs_node, const uint8_t* data, uint32_t data_size) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);

    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);

    /*
    * cant write to initrd fs
    */
    return 0;
}

void initrd_open(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);

    PANIC("not implemented");
}

void initrd_close(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);

    PANIC("not implemented");
}

struct filesystem_node* initrd_find_node_by_id(struct filesystem_node* fs_node, uint32_t id) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);
    struct initrd_devicedata* device_data = (struct initrd_devicedata*)fs_node->filesystem_device->device_data;
    if (fs_node == device_data->root_node) {
        /*
        * root node
        */
        struct filesystem_node* ret = node_cache_find(device_data->nc, id);
        if (0 == ret) {
            ASSERT(id < device_data->header.number_files);
            // the node id is the index into the headers
            char* name = device_data->header.headers[id].name;
            ret = filesystem_node_new(file, fs_node->filesystem_device, name, id, 0);
            node_cache_add(device_data->nc, ret);
        }
        return ret;
    } else {
        // devices are leaf nodes they have no children
        return 0;
    }
}

void initrd_list_directory(struct filesystem_node* fs_node, struct filesystem_directory* dir) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);
    ASSERT_NOT_NULL(dir);
    struct initrd_devicedata* device_data = (struct initrd_devicedata*)fs_node->filesystem_device->device_data;
    if (fs_node == device_data->root_node) {
        /*
        * root node
        */
        dir->count = device_data->header.number_files;
        for (uint32_t i = 0; i < device_data->header.number_files; i++) {
            // node id is the index into the header table
            dir->ids[i] = i;
        }
    } else {
        dir->count = 0;
        // devices are leaf nodes they have no children
    }
}

uint64_t initrd_size(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);
    struct initrd_devicedata* device_data = (struct initrd_devicedata*)fs_node->filesystem_device->device_data;
    if (fs_node == device_data->root_node) {
        /*
        * cant read or write root node
        */
        return 0;
    } else {
        return device_data->header.headers[fs_node->id].length;
    }
}

struct device* initrd_attach(struct device* partition_device, uint32_t lba) {
    ASSERT_NOT_NULL(partition_device);
    ASSERT(1 == blockutil_is_block_device(partition_device));

    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &initrd_init;
    deviceinstance->uninit = &initrd_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = FILESYSTEM;
    devicemgr_set_device_description(deviceinstance, "initrd File System");
    /*
     * the device api
     */
    struct deviceapi_filesystem* api = (struct deviceapi_filesystem*)kmalloc(sizeof(struct deviceapi_filesystem));
    memzero((uint8_t*)api, sizeof(struct deviceapi_filesystem));
    api->close = &initrd_close;
    api->find_id = &initrd_find_node_by_id;
    api->open = &initrd_open;
    api->root = &initrd_get_root_node;
    api->write = &initrd_write;
    api->read = &initrd_read;
    api->list = &initrd_list_directory;
    api->size = &initrd_size;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct initrd_devicedata* device_data = (struct initrd_devicedata*)kmalloc(sizeof(struct initrd_devicedata));
    device_data->root_node = filesystem_node_new(folder, deviceinstance, "initrd", 0, 0);
    device_data->lba = lba;
    device_data->partition_device = partition_device;
    device_data->nc = node_cache_new();
    deviceinstance->device_data = device_data;
    /*
     * register
     */
    if (0 != devicemgr_attach_device(deviceinstance)) {
        /*
        * increase ref count of underlying device
        */
        devicemgr_increment_device_refcount(partition_device);
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

void initrd_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct initrd_devicedata* device_data = (struct initrd_devicedata*)dev->device_data;
    /*
    * decrease ref count of underlying device
    */
    devicemgr_decrement_device_refcount(device_data->partition_device);
    /*
    * detach
    */
    devicemgr_detach_device(dev);
}

/*
* kernel is at 4th lba.  so figure out the kernel size and add
*/
uint64_t initrd_lba() {
    // currently the makefile just puts the initrd fs at offset 10MB in the boot disk
    return 20480;

    //linkage_show_kernel_section_data();
    //  uint64_t kernel_sector_count = linkage_get_kernel_sector_count();

    //  kprintf("kernel_sector_count %#llX\n", kernel_sector_count);
    // return 4 + kernel_sector_count;
}
