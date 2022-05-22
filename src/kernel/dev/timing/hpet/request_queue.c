/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2022 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/timing/hpet/request_queue.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/sync/sync.h>
#include <sys/timing/timing.h>

hpet_request_queue_t* request_queue = 0;
module_spinlock hpet_request_queue_lock;

void hpet_request_queue_add(timing_request_t* request, uint64_t deadline) {
    hpet_request_queue_t* new;
    hpet_request_queue_t* cur;

    new = kmalloc(sizeof(hpet_request_queue_t));
    ASSERT_NOT_NULL(new);
    new->next = 0;
    new->deadline = deadline;
    new->request = request;

    MODULE_SPINLOCK_ACQUIRE(hpet_request_queue_lock);

    if (!request_queue) {
        request_queue = new;
        MODULE_SPINLOCK_RELEASE(hpet_request_queue_lock);
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
        MODULE_SPINLOCK_RELEASE(hpet_request_queue_lock);
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
            MODULE_SPINLOCK_RELEASE(hpet_request_queue_lock);
            return;
        }

        cur = cur->next;
    }

    // If we get here, it means that all values in the list were <=
    // new->deadline, so we add new to the end.
    cur->next = new;

    MODULE_SPINLOCK_RELEASE(hpet_request_queue_lock);

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

timing_request_t* hpet_request_queue_extract_request(hpet_request_queue_t* entry) {
    ASSERT_NOT_NULL(entry);
    return entry->request;
}

uint64_t hpet_request_queue_first_deadline() {
    ASSERT_NOT_NULL(request_queue);

    return request_queue->deadline;
}

timing_request_t** hpet_request_queue_next_expired_request(hpet_request_queue_t* expired_queue) {
    // !!!CALLER IS RESPONSIBLE FOR FREEING RETURNED POINTER!!!

    timing_request_t** array = 0;
    hpet_request_queue_t *cur, *next;
    uint64_t i = 0;

    ASSERT_NOT_NULL(expired_queue);

    cur = expired_queue;

    while (cur) {
        array = krealloc(array, (i + 1) * sizeof(timing_request_t*));

        array[i] = hpet_request_queue_extract_request(cur);

        next = cur->next;
        kfree(cur);
        cur = next;
        i++;
    }

    array = krealloc(array, (i + 1) * sizeof(timing_request_t*));
    array[i] = 0;

    return array;
}

void hpet_request_queue_init() {
    MODULE_SPINLOCK_INIT(hpet_request_queue_lock);

    return;
}

hpet_request_queue_t* hpet_request_queue_slice_deadline(uint64_t deadline) {
    // Detach and return all queue entries before deadline from the request
    // queue

    hpet_request_queue_t *first, *cur;

    /*// If the deadline is less than the first entry in the queue, then either
    // the queue was not constructed properly or the deadline being passed was
    // never entered in the queue in the first place; either way, something has
    // gone wrong.
    ASSERT(deadline >= request_queue->deadline);
    */

    // In theory, this shouldn't happen if the queue is set up properly.  It
    // seems that, nevertheless, one HPET interrupt fires before I've even set a
    // timer.  It remains to be seen if the problem is with my test environment,
    // my code, or my understanding.  Nevertheless, for now we'll just ignore it
    // if this happens.

    // TODO: fix it
    if (deadline < request_queue->deadline) {
        return NULL;
    }

    MODULE_SPINLOCK_ACQUIRE(hpet_request_queue_lock);

    cur = first = request_queue;

    while (cur->next) {
        if (cur->next->deadline > deadline) {
            request_queue = cur->next;
            cur->next = 0;
            MODULE_SPINLOCK_RELEASE(hpet_request_queue_lock);
            return first;
        }

        cur = cur->next;
    }

    // If we get here, then the deadline passed as an argument > the deadline of
    // every entry in the queue, so we return the entire list and reset it
    MODULE_SPINLOCK_RELEASE(hpet_request_queue_lock);
    request_queue = 0;
    return first;
}

bool hpet_request_queue_valid() {
    // returns true if there is at least one valid entry in the request queue,
    // otherwise false

    if (request_queue) {
        return true;
    } else {
        return false;
    }
}