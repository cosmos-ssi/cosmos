/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2022 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/timing/timing.h>
#include <types.h>

#ifndef _HPET_REQUEST_QUEUE_H
#define _HPET_REQUEST_QUEUE_H

typedef struct hpet_request_queue_t {
    timing_request_t* request;
    uint64_t deadline;
    struct hpet_request_queue_t* next;
} hpet_request_queue_t;

void hpet_request_queue_add(timing_request_t* request, uint64_t deadline);
void hpet_request_queue_dump();

#endif