//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Jeff Schnurr                           *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/virtio/virtio.h>
#include <dev/logical/virtio/virtqueue.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <tests/dev/test_virtio_virtqueue.h>
#include <types.h>

void test_virtio_virtqueue() {
    // make the queue
    struct virtq* q = virtq_new(16);
    ASSERT(virtio_isAligned(((uint64_t)q), 4096));

    // make some descriptors
    char s1[] = "Test";
    struct virtq_descriptor* desc1 = virtq_descriptor_new(s1, sizeof(s1), true);
    char s2[] = "Test test";
    struct virtq_descriptor* desc2 = virtq_descriptor_new(s2, sizeof(s2), true);

    // put descriptors on the queue
    virtq_enqueue_descriptor(q, desc1);
    virtq_enqueue_descriptor(q, desc2);

    // simulate what device hardware would do by populating the used queue
    struct virtq_used_elem* e1 = kmalloc(sizeof(struct virtq_used_elem));
    e1->id = 0;
    e1->len = sizeof(s1);
    q->used.ring[0] = *e1;

    struct virtq_used_elem* e2 = kmalloc(sizeof(struct virtq_used_elem));
    e2->id = 1;
    e2->len = sizeof(s2);
    q->used.ring[1] = *e2;

    // dequeue and validate we got our descriptor back with same contents
    struct virtq_descriptor* r1 = virtq_dequeue_descriptor(q);
    ASSERT(r1 == desc1);

    struct virtq_descriptor* r2 = virtq_dequeue_descriptor(q);
    ASSERT(r2 == desc2);
}
