/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2022 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/timing/hpet/hpet_request_queue.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/timing/timing.h>

hpet_request_queue_t* request_queue = 0;

void hpet_request_queue_add(timing_request_t* request, uint64_t deadline) {
    hpet_request_queue_t* new;
    hpet_request_queue_t* cur;

    new = kmalloc(sizeof(hpet_request_queue_t));
    ASSERT_NOT_NULL(new);
    new->next = 0;
    new->deadline = deadline;
    new->request = request;

    if (!request_queue) {
        request_queue = new;
        return;
    }

    ASSERT_NOT_NULL(request_queue);

    // We insert the new item in its location in the queue in ascending order of
    // deadline. If we the first item is > deadline, then we know right way that
    // the new entry is first.  If all items are < deadline, we know it goes to
    // the end.

    cur = request_queue;

    // Test the deadline of the first entry in the queue.  If it's >
    // new->deadline, we know that new becomes the new first item, so we set up
    // the linked list accordingly (including changing request_queue to point to
    // new) and return.
    if (cur->deadline > new->deadline) {
        new->next = cur;
        request_queue = new;
        return;
    }

    // Loop through the list.  At each step, check the deadline of the NEXT item
    // in the list (we already know that cur->deadline < new->deadline, either
    // from the above test or from the prior iteration through the loop); if it
    // is > new->deadline, then new goes between cur and cur->next and we're
    // finished
    while (cur->next) {
        if (cur->next->deadline > new->deadline) {
            new->next = cur->next;
            cur->next = new;
            return;
        }

        cur = cur->next;
    }

    // If we get here, it means that all values in the list were <=
    // new->deadline, so we add new to the end.
    cur->next = new;

    return;
}

void hpet_request_queue_dump() {
    hpet_request_queue_t* cur;

    cur = request_queue;

    while (cur) {
        kprintf("%llu: %llu\n", cur->request->request_id, cur->deadline);
        cur = cur->next;
    }

    return;
}