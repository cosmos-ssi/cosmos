//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/fs/block_util.h>
#include <dev/logical/fs/fs_util.h>
#include <dev/logical/fs/initrd/initrd.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/deviceapi/deviceapi_filesystem.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/linkage/linkage.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>

#define INITRD_NAME_SIZE 64
#define INITRD_MAX_FILES 64

struct initrd_file_header {
    uint8_t magic;
    uint8_t name[INITRD_NAME_SIZE];
    uint32_t offset;
    uint32_t length;
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

uint32_t initrd_read(struct filesystem_node* fs_node, const uint8_t* data, uint32_t data_size) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);

    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);
    // read from node. we cant read from the root node, but we can find underlying file and folder nodes
    panic("not implemented");

    return 0;
}

uint32_t initrd_write(struct filesystem_node* fs_node, const uint8_t* data, uint32_t data_size) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);

    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);
    // write to node. we cant write to the root node, but we can find underlying file and folder nodes
    panic("not implemented");

    return 0;
}

void initrd_open(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);

    panic("not implemented");
}

void initrd_close(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);

    panic("not implemented");
}

struct filesystem_node* initrd_find_node_by_id(struct filesystem_node* fs_node, uint32_t id) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);

    // find subnode.  we can do this for the root node, but not contained nodes b/c initrd doesn't support folders
    panic("not implemented");

    return 0;
}

struct filesystem_node* initrd_find_node_by_name(struct filesystem_node* fs_node, uint8_t* name) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);

    ASSERT_NOT_NULL(name);
    // find subnode.  we can do this for the root node, but not contained nodes b/c initrd doesn't support folders
    panic("not implemented");

    return 0;
}

/*
* find a node by name
*/
struct filesystem_node* initrd_find_node_by_idx(struct filesystem_node* fs_node, uint32_t idx) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);

    // find subnode.  we can do this for the root node, and for subnodes that are folders
    panic("not implemented");

    return 0;
}
/*
* count
*/
uint32_t initrd_count(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->device_data);

    panic("not implemented");

    return 0;
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
    api->find_name = &initrd_find_node_by_name;
    api->open = &initrd_open;
    api->root = &initrd_get_root_node;
    api->write = &initrd_write;
    api->read = &initrd_read;
    api->find_idx = initrd_find_node_by_idx;
    api->count = initrd_count;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct initrd_devicedata* device_data = (struct initrd_devicedata*)kmalloc(sizeof(struct initrd_devicedata));
    struct filesystem_node* r = (struct filesystem_node*)kmalloc(sizeof(struct filesystem_node));
    memzero((uint8_t*)r, sizeof(struct filesystem_node));
    r->filesystem_device = deviceinstance;
    r->id = 0;
    strncpy(r->name, "initrd", FILESYSTEM_MAX_NAME);
    device_data->root_node = r;
    device_data->lba = lba;
    device_data->partition_device = partition_device;
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

uint8_t initrd_get_file_name(struct device* initrd_dev, uint8_t idx, uint8_t* name, uint16_t size) {
    ASSERT_NOT_NULL(initrd_dev);
    ASSERT_NOT_NULL(initrd_dev->device_data);
    struct initrd_devicedata* device_data = (struct initrd_devicedata*)initrd_dev->device_data;
    ASSERT(idx < device_data->header.number_files);
    ASSERT(size >= INITRD_NAME_SIZE);
    strncpy(name, device_data->header.headers[idx].name, size);
    return 1;
}

uint32_t initrd_get_file_length(struct device* initrd_dev, uint8_t idx) {
    ASSERT_NOT_NULL(initrd_dev);
    ASSERT_NOT_NULL(initrd_dev->device_data);
    struct initrd_devicedata* device_data = (struct initrd_devicedata*)initrd_dev->device_data;
    ASSERT(idx < device_data->header.number_files);
    return device_data->header.headers[idx].length;
}

uint8_t initrd_get_file_data(struct device* initrd_dev, uint8_t idx, uint8_t* data, uint32_t size) {
    ASSERT_NOT_NULL(initrd_dev);
    ASSERT_NOT_NULL(initrd_dev->device_data);
    struct initrd_devicedata* device_data = (struct initrd_devicedata*)initrd_dev->device_data;
    ASSERT(idx < device_data->header.number_files);

    uint32_t sector_size = blockutil_get_sector_size(device_data->partition_device);

    uint32_t offset = device_data->header.headers[idx].offset;
    ASSERT(offset > 0);
    uint32_t length = device_data->header.headers[idx].length;
    ASSERT(size >= length);

    uint32_t lba_offset = offset / sector_size;
    uint32_t byte_offset = offset % sector_size;
    uint32_t total_sectors = length / sector_size;
    if ((length % 512) != 0) {
        total_sectors += 1;
    }
    uint32_t buffer_size = total_sectors * sector_size;
    uint8_t* buffer = kmalloc(buffer_size);
    memzero(buffer, buffer_size);
    uint32_t target_lba = device_data->lba + lba_offset;
    //    kprintf("lba_offset %llu byte_offset %llu sectors %llu target_lba %llu buffer_size %llu\n", lba_offset, byte_offset,
    //            total_sectors, target_lba, buffer_size);

    /*
    * read the blocks
    */
    blockutil_read(device_data->partition_device, buffer, buffer_size, target_lba);

    /*
    * copy the data
    */
    memcpy(data, &(buffer[byte_offset]), size);

    /*
    * release the buffer
    */
    kfree(buffer);
    return 1;
}

uint8_t initrd_get_file_count(struct device* initrd_dev) {
    ASSERT_NOT_NULL(initrd_dev);
    ASSERT_NOT_NULL(initrd_dev->device_data);
    struct initrd_devicedata* device_data = (struct initrd_devicedata*)initrd_dev->device_data;
    return device_data->header.number_files;
}

void initrd_dump_dir(struct device* initrd_dev) {
    ASSERT_NOT_NULL(initrd_dev);
    ASSERT_NOT_NULL(initrd_dev->device_data);
    struct initrd_devicedata* device_data = (struct initrd_devicedata*)initrd_dev->device_data;
    for (uint32_t i = 0; i < device_data->header.number_files; i++) {
        struct initrd_file_header thisheader = device_data->header.headers[i];
        //     debug_show_memblock((uint8_t*)&thisheader, sizeof(struct initrd_file_header));

        kprintf("    %s at %#llX length %#llX magic %#X\n", thisheader.name, thisheader.offset, thisheader.length,
                thisheader.magic);
    }
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

uint8_t initrd_find_file(struct device* initrd_dev, uint8_t* name, uint8_t* idx) {
    ASSERT_NOT_NULL(initrd_dev);
    ASSERT_NOT_NULL(name);
    ASSERT_NOT_NULL(idx);
    ASSERT(strlen(name) < INITRD_NAME_SIZE);  // less than b/c of null termination
    uint8_t file_count = initrd_get_file_count(initrd_dev);
    for (uint8_t i = 0; i < file_count; i++) {
        uint8_t thisname[INITRD_NAME_SIZE];
        memzero(thisname, INITRD_NAME_SIZE);
        initrd_get_file_name(initrd_dev, i, thisname, INITRD_NAME_SIZE);
        if (0 == strcmp(thisname, name)) {
            *idx = i;
            return 1;
        }
    }
    return 0;
}
