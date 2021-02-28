//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/virtio/virtqueue.h>
#include <sys/debug/assert.h>
#include <sys/iobuffers/iobuffers.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/x86-64/mm/pagetables.h>

/*
 * create virtq
 */
struct virtq* virtq_new(uint16_t size) {
    // virtqueue must be aligned on a 4096-byte boundary
    struct virtq* ret = (struct virtq*)iobuffers_request_buffer(sizeof(struct virtq));  // 32-bit identity mapped
    /*
     * size
     */
    // Queue Size value is always a power of 2.
    if (0 == (size && !(size & (size - 1)))) {
        PANIC("Queue size must be a power of 2.");
    }
    ret->size = size;
    /*
     *  allocate descriptor pointer array
     */
    ret->descriptors = kmalloc(sizeof(struct virtq_descriptor*) * size);
    for (uint16_t i = 0; i < size; i++) {
        (ret->descriptors)[i] = 0;
    }
    /*
     * avail queue
     */
    ret->avail.ring = kmalloc(sizeof(uint16_t) * size);
    for (uint16_t i = 0; i < size; i++) {
        ret->avail.ring[i] = 0;
    }
    ret->avail.flags = 0;
    ret->avail.idx = 0;
    /*
     * used queue
     */
    ret->used.ring = kmalloc(sizeof(struct virtq_used_elem) * size);
    for (uint16_t i = 0; i < size; i++) {
        ret->used.ring[i].id = 0;
        ret->used.ring[i].len = 0;
    }
    ret->used.avail_event = 0;
    ret->used.flags = 0;
    ret->used.idx = 0;
    return ret;

    /*
    * last_seen_used
    */
    ret->last_seen_used = -1;
}

/*
 * print virtq
 */
void virtq_print(uint8_t qname[], struct virtq* queue) {
    kprintf(qname);
    kprintf("availidx: %u, usedidx: %u, lastidx: %u | ", queue->avail.idx, queue->used.idx, queue->last_seen_used);
    kprintf("nextempty: %u, ", find_first_empty_slot(queue));
    kprintf("status: ");
    if (0 != queue->descriptors) {
        for (uint16_t i = 0; i < queue->size; i++) {
            if (0 == (queue->descriptors)[i]) {
                kprintf("N");
            } else {
                kprintf("Y");
            }
        }
        kprintf("\n");
    }
}

/*
 * delete virtq
 */
void virtq_delete(struct virtq* queue) {
    ASSERT_NOT_NULL(queue);
    /*
     *  descriptor array
     */
    if (0 != queue->descriptors) {
        for (uint16_t i = 0; i < queue->size; i++) {
            if (0 != (queue->descriptors)[i]) {
                kfree((queue->descriptors)[i]);
            }
        }
    } else {
        PANIC("descriptor array should not be null in virtq_delete");
    }
    kfree(queue);
}

/**
 * find empty slot in descriptor array
 */
uint16_t find_first_empty_slot(struct virtq* queue) {
    ASSERT_NOT_NULL(queue);
    for (uint16_t i = 0; i < queue->size; i++) {
        if (queue->descriptors[i] == 0) {
            return i;
        }
    }
    return -1;
}

/*
 * enqueue descriptor
 * TODO: Support chaining mulitple descriptors via next
 * TODO: Support indirect descriptors
 */
void virtq_enqueue_descriptor(struct virtq* queue, struct virtq_descriptor* descriptor) {
    ASSERT_NOT_NULL(queue);
    ASSERT_NOT_NULL(descriptor);

    // put descriptor(s) in next available slot
    uint16_t slot = find_first_empty_slot(queue);
    if (-1 == slot) {
        PANIC("Ran out of virtual queue slots\n");
    }
    queue->descriptors[slot] = descriptor;

    // point to the descriptor chain head in the avail ring (modulo queue size)
    queue->avail.ring[queue->avail.idx % queue->size] = slot;

    // increment the available idx by the number of descriptor chain heads added
    queue->avail.idx = queue->avail.idx + 1;
}

/*
 * dequeue descriptor
 * TODO: Support mulitple chained descriptors
 * TODO: Support indirect descriptors
 */
struct virtq_descriptor* virtq_dequeue_descriptor(struct virtq* queue) {
    // check if any unseen buffers exist
    // if (queue->used.idx == queue->last_seen_used) {
    //     return;
    // }

    // get the next used elem
    uint16_t used_idx = queue->last_seen_used % queue->size;
    struct virtq_used_elem* e = &queue->used.ring[used_idx];

    queue->last_seen_used++;

    // get the descriptor
    struct virtq_descriptor* desc = queue->descriptors[e->id];

    // make the slot available for re-use
    queue->descriptors[e->id] = 0;

    // return the descriptor
    return desc;
}

/*
 * new descriptor
 */
struct virtq_descriptor* virtq_descriptor_new(uint8_t* buffer, uint32_t len, bool writable) {
    ASSERT_NOT_NULL(buffer);
    struct virtq_descriptor* ret = kmalloc(sizeof(struct virtq_descriptor));

    // buffer address must be guest-physical
    ret->addr = (uint64_t)CONV_DMAP_ADDR(buffer);

    if (writable) {
        ret->flags = VIRTQ_DESC_F_DEVICE_WRITE_ONLY;
    } else {
        ret->flags = 0;
    }
    ret->len = len;
    ret->next = 0;  // there is no next
    return ret;
}

/*
 * delete descriptor
 */
void virtq_descriptor_delete(struct virtq_descriptor* descriptor) {
    ASSERT_NOT_NULL(descriptor);
    if (0 != descriptor->addr) {
        kfree((uint8_t*)descriptor->addr);
    } else {
        PANIC("virtq_descriptor address should not be zero");
    }
    kfree(descriptor);
}

/*
 * available idx
 */
uint16_t virtq_get_available_idx(struct virtq* queue) {
    ASSERT_NOT_NULL(queue);
    return queue->avail.idx;
}

/*
 * used idx
 */
uint16_t virtq_get_used_idx(struct virtq* queue) {
    ASSERT_NOT_NULL(queue);
    return queue->used.idx;
}
