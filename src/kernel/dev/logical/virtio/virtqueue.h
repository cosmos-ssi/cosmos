//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _VIRTQUEUE_H
#define _VIRTQUEUE_H

#include <types.h>

// https://www.redhat.com/en/blog/virtqueues-and-virtio-ring-how-data-travels
// https://docs.oasis-open.org/virtio/virtio/v1.1/virtio-v1.1.pdf

// #define VIRTQUEUE_NUM_BUFFERS 0x08                                     // 8 buffers (1 per device * 8 devices)
// #define VIRTQUEUE_BUFFER_SIZE sizeof(struct virtq)                     // 8k blocks
// #define VIRTQUEUE_ALIGNMENT 0x10000                                    // channels must be 64k aligned
// #define VIRTQUEUE_BUFSIZ VIRTQUEUE_NUM_BUFFERS* VIRTQUEUE_BUFFER_SIZE  // this is used by mm to reserve space

// #define VIRTQ_DESC_F_AVAIL (1 << 7)
// #define VIRTQ_DESC_F_USED (1 << 15)

// flags for virtqueue descriptors
#define VIRTQ_DESC_F_NEXT 0x01               // marks a buffer as continuing via the next field.
#define VIRTQ_DESC_F_DEVICE_WRITE_ONLY 0x02  // marks a buffer as device write-only (otherwise device read-only).
#define VIRTQ_DESC_F_DEVICE_READ_ONLY 0x0
#define VIRTQ_DESC_F_INDIRECT 0x04  // buffer contains a list of buffer descriptors.

struct virtq_descriptor {
    uint8_t* addr; /* Address (guest-physical). */
    uint32_t len;
    uint16_t flags;
    uint16_t next;
};

// flags for virtq_avail
#define VIRTQ_AVAIL_F_NO_INTERRUPT 1
// driver area. written to by the driver, read by the device.

struct virtq_avail {
    uint16_t flags;
    uint16_t idx;
    uint16_t* ring;  // Queue Size
};

// flags for virtq_used
#define VIRTQ_USED_F_NO_NOTIFY 1

struct virtq_used_elem {
    uint32_t id;   // Index of start of used descriptor chain.
    uint32_t len;  // total of bytes written into the buffer.
};

struct virtq_used {
    uint16_t flags;
    uint16_t idx;  // device specifies next descriptor entry in the ring (modulo the queue size)
    struct virtq_used_elem* ring;
    uint16_t avail_event;  // Only if VIRTIO_F_EVENT_IDX
};

struct virtq {
    uint16_t size;                          // in elements
    struct virtq_descriptor** descriptors;  // used for describing buffers
    struct virtq_avail avail;               // driver area. data supplied by driver to the device
    struct virtq_used used;                 // device area. data supplied by device to driver.
    uint16_t last_seen_used;                // the last value of used->idx we've seen; devive may have added since.
};

// virtq
struct virtq* virtq_new(uint16_t size);
void virtq_delete(struct virtq* queue);
void virtq_enqueue_descriptor(struct virtq* queue, struct virtq_descriptor* descriptor);
struct virtq_descriptor* virtq_dequeue_descriptor(struct virtq* queue);

// descriptors
struct virtq_descriptor* virtq_descriptor_new(uint8_t* buffer, uint32_t len, bool writable);
void virtq_descriptor_delete(struct virtq_descriptor* descriptor);

// available
uint16_t virtq_get_available_idx(struct virtq* queue);

// used
uint16_t virtq_get_used_idx(struct virtq* queue);

uint16_t find_first_empty_slot(struct virtq* queue);

void virtq_print(uint8_t qname[], struct virtq* queue);
#endif