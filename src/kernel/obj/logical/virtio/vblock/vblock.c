//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/Virtio

#include <obj/logical/virtio/vblock/vblock.h>
#include <obj/logical/virtio/virtio.h>
#include <obj/logical/virtio/virtqueue.h>
#include <obj/x86-64/pci/pci.h>
#include <sys/asm/asm.h>
#include <sys/asm/io.h>
#include <sys/debug/assert.h>
#include <sys/obj/objectmgr/objectmgr.h>

#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/objectinterface/objectinterface_block.h>
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
struct vblock_objectdata {
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
uint8_t vblock_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);

    struct vblock_objectdata* object_data = (struct vblock_objectdata*)obj->object_data;
    interrupt_router_register_interrupt_handler(obj->pci->irq, &vblock_irq_handler);
    object_data->base = pci_calcbar(obj->pci);

    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n", obj->description, obj->pci->irq,
            obj->pci->vendor_id, obj->pci->device_id, object_data->base, obj->name);

    // acknowledge device and set the driver loaded bit
    asm_out_b(object_data->base + VIRTIO_DEVICE_STATUS, VIRTIO_STATUS_DEVICE_ACKNOWLEGED);

    // get the feature bits
    //   uint32_t features = asm_in_b(object_data->base + VIRTIO_DEVICE_FEATURES);

    // we kinda care about geometry and block size
    asm_out_b(object_data->base + VIRTIO_GUEST_FEATURES, VIRTIO_BLK_F_GEOMETRY | VIRTIO_BLK_F_BLK_SIZE);

    // write features ok
    asm_out_b(object_data->base + VIRTIO_DEVICE_STATUS, VIRTIO_STATUS_FEATURES_OK);

    // read features ok.  we good?
    uint32_t status = asm_in_b(object_data->base + VIRTIO_DEVICE_STATUS);
    if (VIRTIO_STATUS_FEATURES_OK != status) {
        PANIC("virtio feature negotiation failed");
    }

    // cool
    asm_out_b(object_data->base + VIRTIO_DEVICE_STATUS, VIRTIO_STATUS_DRIVER_READY);

    /*
     * length*totalSectors should equal the byte size of the mounted file (currently hda.img)
     */
    object_data->totalSectors = asm_in_d(object_data->base + VIRTIO_BLOCK_TOTAL_SECTORS);
    object_data->sectorLength = asm_in_d(object_data->base + VIRTIO_BLOCK_LENGTH);
    uint64_t totalBytes = object_data->totalSectors * (object_data->sectorLength);
    kprintf("   Total byte size of mounted media: %llu\n", totalBytes);

    // select queue 0
    asm_out_w(object_data->base + VIRTIO_QUEUE_SELECT, 0);

    // get the needed size
    uint16_t queue_size_needed = asm_in_w(object_data->base + VIRTIO_QUEUE_SIZE);
    kprintf("   Queue size needed: %llu\n", queue_size_needed);

    // make the queue
    struct virtq* q = virtq_new(queue_size_needed);
    bool all = virtio_isAligned(((uint64_t)q), 4096);
    ASSERT(all);
    object_data->request_queue = q;

    // divide by 4096
    uint32_t q_shifted = (uint64_t)q >> 12;

    // set the queue.  The API takes a 32 bit pointer, but we have a 64 bit pointer, so ... some conversions
    kprintf("   Queue Address: %#hX %#hX\n", q, q_shifted);
    asm_out_d(object_data->base + VIRTIO_QUEUE_ADDRESS, q_shifted);

    return 1;
}

uint32_t vblockutil_read(struct object* obj, uint8_t* data, uint32_t data_size, uint32_t start_lba) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);

    ASSERT_NOT_NULL(obj->object_data);
    struct vblock_objectdata* object_data = (struct vblock_objectdata*)obj->object_data;

    /*
     * drop a message
     */
    kprintf("read sector %llu, size %llu\n", start_lba);

    PANIC("this code needs to be rewritten");

    /*
     * block request
     */
    struct vblock_block_request* req = vblock_block_request_new();
    req->type = VIRTIO_BLK_T_IN;
    req->sector = start_lba;
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
    virtq_enqueue_descriptor(object_data->request_queue, desc);

    // there is an available buffer
    //    uint16_t avail_idx = virtq_get_available_idx(object_data->vblock_queue);
    //    kprintf("avail_idx %llu\n",avail_idx);
    asm_out_w(object_data->base + VIRTIO_QUEUE_NOTIFY, 0);
    return 0;
}

uint32_t vblockutil_write(struct object* obj, uint8_t* data, uint32_t data_size, uint32_t start_lba) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);

    PANIC("vblock write not implemented yet");
    return 0;
}

uint16_t vblock_sector_size(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct vblock_objectdata* object_data = (struct vblock_objectdata*)obj->object_data;
    return object_data->sectorLength;
}

uint32_t vblock_total_size(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct vblock_objectdata* object_data = (struct vblock_objectdata*)obj->object_data;
    return object_data->totalSectors * object_data->sectorLength;
}

void vblock_search_cb(struct pci_device* dev) {
    ASSERT_NOT_NULL(dev);
    /*
     * register device
     */
    struct object* objectinstance = object_new_object();
    objectinstance->init = &vblock_init;
    objectinstance->pci = dev;
    objectinstance->objectype = VBLOCK;
    objectmgr_set_object_description(objectinstance, "Virtio ATA");
    /*
     * device data
     */
    struct vblock_objectdata* object_data = (struct vblock_objectdata*)kmalloc(sizeof(struct vblock_objectdata));
    objectinstance->object_data = object_data;
    /*
     * the device api
     */
    struct objectinterface_block* api = (struct objectinterface_block*)kmalloc(sizeof(struct objectinterface_block));
    memzero((uint8_t*)api, sizeof(struct objectinterface_block));
    api->write = &vblockutil_write;
    api->read = &vblockutil_read;
    api->sector_size = &vblock_sector_size;
    api->total_size = &vblock_total_size;
    objectinstance->api = api;
    /*
     * register
     */
    objectmgr_register_object(objectinstance);
}

/**
 * find all virtio block devices and register them
 */
void vblock_objectmgr_register_objects() {
    pci_objectmgr_search_device(PCI_CLASS_MASS_STORAGE, PCI_MASS_STORAGE_SUBCLASS_SCSI, VIRTIO_PCI_MANUFACTURER,
                                VIRTIO_PCI_DEVICED_BLOCK, &vblock_search_cb);
}
