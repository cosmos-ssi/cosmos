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
    kfree(dev->device_data);
    return 1;
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
    deviceinstance->api = api;
    /*
     * device data
     */
    struct initrd_devicedata* device_data = (struct initrd_devicedata*)kmalloc(sizeof(struct initrd_devicedata));
    device_data->partition_device = partition_device;
    device_data->lba = lba;
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

uint8_t initrd_get_file_length(struct device* initrd_dev, uint8_t idx, uint16_t* size) {
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

    uint32_t offset = device_data->header.headers[idx].offset;
    ASSERT(offset > 0);
    uint32_t length = device_data->header.headers[idx].length;
    ASSERT(size >= length);

    blockutil_read(device_data->partition_device, data, length, device_data->lba + offset);

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
