//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/Virtio

#include <dev/logical/virtio/vblock/vblock.h>
#include <dev/logical/virtio/virtio.h>
#include <dev/logical/virtio/virtqueue.h>
#include <dev/x86-64/pci/pci.h>
#include <sys/asm/asm.h>
#include <sys/asm/io.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/sleep/sleep.h>
#include <sys/string/mem.h>
#include <types.h>

// registers
#define VIRTIO_BLOCK_TOTAL_SECTORS 0x14
#define VIRTIO_BLOCK_MAX_SEGMENT_SIZE 0x1C
#define VIRTIO_BLOCK_MAX_SEGMENT_COUNT 0x20
#define VIRTIO_BLOCK_CYLINDER_COUNT 0x24
#define VIRTIO_BLOCK_HEAD_COUNT 0x26
#define VIRTIO_BLOCK_SECTOR_COUNT 0x27
#define VIRTIO_BLOCK_LENGTH 0x28

// feature flags
#define VIRTIO_BLK_F_SIZE_MAX 1
#define VIRTIO_BLK_F_SEG_MAX 2
#define VIRTIO_BLK_F_GEOMETRY 4
#define VIRTIO_BLK_F_RO 5
#define VIRTIO_BLK_F_BLK_SIZE 6
#define VIRTIO_BLK_F_FLUSH 9
#define VIRTIO_BLK_F_TOPOLOGY 10
#define VIRTIO_BLK_F_CONFIG_WCE 11
#define VIRTIO_BLK_F_DISCARD 13
#define VIRTIO_BLK_F_WRITE_ZEROES 14

// request types
#define VIRTIO_BLK_T_IN 0
#define VIRTIO_BLK_T_OUT 1
#define VIRTIO_BLK_T_FLUSH 4
#define VIRTIO_BLK_T_DISCARD 11
#define VIRTIO_BLK_T_WRITE_ZEROES 13

/*
 * vblock instance specific data
 */
struct vblock_devicedata {
    uint64_t base;
    uint32_t sectorLength;
    uint32_t totalSectors;
    struct virtq* request_queue;
} __attribute__((packed));

struct vblock_block_request {
    uint32_t type;  // 0: Read; 1: Write; 4: Flush; 11: Discard; 13: Write zeroes
    uint32_t reserved;
    uint64_t sector;
    uint8_t* data;   // Data's size must be a multiple of 512
    uint8_t status;  // 0: OK; 1: Error; 2: Unsupported
};

struct vblock_block_request* vblock_block_request_new() {
    struct vblock_block_request* ret = kmalloc(sizeof(struct vblock_block_request));
    ret->data = 0;
    ret->type = 0;
    ret->reserved = 0;
    ret->sector = 0;
    ret->status = 0;
    return ret;
}

void vblock_irq_handler(stack_frame* frame) {
    ASSERT_NOT_NULL(frame);
    kprintf("#");
}

/*
 * perform device instance specific init here
 */
uint8_t vblock_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);

    struct vblock_devicedata* device_data = (struct vblock_devicedata*)dev->device_data;
    interrupt_router_register_interrupt_handler(dev->pci->irq, &vblock_irq_handler);
    device_data->base = pci_calcbar(dev->pci);

    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n", dev->description, dev->pci->irq,
            dev->pci->vendor_id, dev->pci->device_id, device_data->base, dev->name);

    // acknowledge device and set the driver loaded bit
    asm_out_b(device_data->base + VIRTIO_DEVICE_STATUS, VIRTIO_STATUS_DEVICE_ACKNOWLEGED);

    // get the feature bits
    //   uint32_t features = asm_in_b(device_data->base + VIRTIO_DEVICE_FEATURES);

    // we kinda care about geometry and block size
    asm_out_b(device_data->base + VIRTIO_GUEST_FEATURES, VIRTIO_BLK_F_GEOMETRY | VIRTIO_BLK_F_BLK_SIZE);

    // write features ok
    asm_out_b(device_data->base + VIRTIO_DEVICE_STATUS, VIRTIO_STATUS_FEATURES_OK);

    // read features ok.  we good?
    uint32_t status = asm_in_b(device_data->base + VIRTIO_DEVICE_STATUS);
    if (VIRTIO_STATUS_FEATURES_OK != status) {
        panic("virtio feature negotiation failed");
    }

    // cool
    asm_out_b(device_data->base + VIRTIO_DEVICE_STATUS, VIRTIO_STATUS_DRIVER_READY);

    /*
     * length*totalSectors should equal the byte size of the mounted file (currently hda.img)
     */
    device_data->totalSectors = asm_in_d(device_data->base + VIRTIO_BLOCK_TOTAL_SECTORS);
    device_data->sectorLength = asm_in_d(device_data->base + VIRTIO_BLOCK_LENGTH);
    uint64_t totalBytes = device_data->totalSectors * (device_data->sectorLength);
    kprintf("   Total byte size of mounted media: %llu\n", totalBytes);

    // select queue 0
    asm_out_w(device_data->base + VIRTIO_QUEUE_SELECT, 0);

    // get the needed size
    uint16_t queue_size_needed = asm_in_w(device_data->base + VIRTIO_QUEUE_SIZE);
    kprintf("   Queue size needed: %llu\n", queue_size_needed);

    // make the queue
    struct virtq* q = virtq_new(queue_size_needed);
    bool all = virtio_isAligned(((uint64_t)q), 4096);
    ASSERT(all);
    device_data->request_queue = q;

    // divide by 4096
    uint32_t q_shifted = (uint64_t)q >> 12;

    // set the queue.  The API takes a 32 bit pointer, but we have a 64 bit pointer, so ... some conversions
    kprintf("   Queue Address: %#hX %#hX\n", q, q_shifted);
    asm_out_d(device_data->base + VIRTIO_QUEUE_ADDRESS, q_shifted);

    return 1;
}

void vblockutil_read_sector(struct device* dev, uint32_t sector, uint8_t* data, uint32_t sector_count) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);

    ASSERT_NOT_NULL(dev->device_data);
    struct vblock_devicedata* device_data = (struct vblock_devicedata*)dev->device_data;

    /*
     * drop a message
     */
    kprintf("read sector %llu, size %llu\n", sector, sector_count);

    /*
     * block request
     */
    struct vblock_block_request* req = vblock_block_request_new();
    req->type = VIRTIO_BLK_T_IN;
    req->sector = sector;
    req->data = data;

    /*
     * descriptor
     */
    struct virtq_descriptor* desc = virtq_descriptor_new((uint8_t*)req, sizeof(struct vblock_block_request), true);
    kprintf("desc addr %#hX\n", desc->addr);
    kprintf("desc length %llu\n", desc->len);
    kprintf("desc flags %llu\n", desc->flags);
    kprintf("desc next %llu\n", desc->next);

    // enqueue
    virtq_enqueue_descriptor(device_data->request_queue, desc);

    // there is an available buffer
    //    uint16_t avail_idx = virtq_get_available_idx(device_data->vblock_queue);
    //    kprintf("avail_idx %llu\n",avail_idx);
    asm_out_w(device_data->base + VIRTIO_QUEUE_NOTIFY, 0);
}

void vblockutil_write_sector(struct device* dev, uint32_t sector, uint8_t* data, uint32_t sector_count) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);
    panic("vblock write not implemented yet");
}

uint16_t vblock_sector_size(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct vblock_devicedata* device_data = (struct vblock_devicedata*)dev->device_data;
    return device_data->sectorLength;
}

uint32_t vblock_total_size(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct vblock_devicedata* device_data = (struct vblock_devicedata*)dev->device_data;
    return device_data->totalSectors * device_data->sectorLength;
}

void vblock_search_cb(struct pci_device* dev) {
    ASSERT_NOT_NULL(dev);
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &vblock_init;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = VBLOCK;
    devicemgr_set_device_description(deviceinstance, "Virtio ATA");
    /*
     * device data
     */
    struct vblock_devicedata* device_data = (struct vblock_devicedata*)kmalloc(sizeof(struct vblock_devicedata));
    deviceinstance->device_data = device_data;
    /*
     * the device api
     */
    struct deviceapi_block* api = (struct deviceapi_block*)kmalloc(sizeof(struct deviceapi_block));
    memzero((uint8_t*)api, sizeof(struct deviceapi_block));
    api->write = &vblockutil_write_sector;
    api->read = &vblockutil_read_sector;
    api->sector_size = &vblock_sector_size;
    api->total_size = &vblock_total_size;
    deviceinstance->api = api;
    /*
     * register
     */
    devicemgr_register_device(deviceinstance);
}

/**
 * find all virtio block devices and register them
 */
void vblock_devicemgr_register_devices() {
    pci_devicemgr_search_device(PCI_CLASS_MASS_STORAGE, PCI_MASS_STORAGE_SUBCLASS_SCSI, VIRTIO_PCI_MANUFACTURER,
                                VIRTIO_PCI_DEVICED_BLOCK, &vblock_search_cb);
}
