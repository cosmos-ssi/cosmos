/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _SYS_TIMING_H
#define _SYS_TIMING_H

#include <subsystems.h>
#include <types.h>

extern const uint64_t one_billion;

typedef uint64_t timing_source_descriptor;

typedef enum timing_source_type_t {
    TIMING_SOURCE_HPET,
    TIMING_SOURCE_LAPIC,
    TIMING_SOURCE_PIT,
    TIMING_SOURCE_RTC,
    TIMING_SOURCE_TSC,
    TIMING_SOURCE_OTHER
} timing_source_type_t;

typedef enum timing_request_status_t {
    TIMING_REQUEST_STATUS_INITIATED,
    TIMING_REQUEST_STATUS_SUBMITTED,
    TIMING_REQUEST_STATUS_EXPIRED,
    TIMING_REQUEST_STATUS_DELIVERED
} timing_request_status_t;

typedef struct timing_request_t {
    uint64_t request_id;
    uint64_t delay_nsec;
    timing_request_status_t status;
} timing_request_t;

typedef struct timing_driver_api_t {
    bool (*calibrate)(uint64_t);
    uint64_t (*set_frequency)(uint64_t);
    bool (*set_deadline_absolute)(timing_request_t* deadline);
    bool (*set_deadline_relative)(timing_request_t* deadline);
    uint64_t (*get_value)(void);
} timing_driver_api_t;

typedef struct timing_driver_t {
    timing_source_type_t type;
    uint64_t driver_id[4];
    uint64_t num_sources;
    timing_driver_api_t api;

    driver_list_entry_t* list_entry;
} timing_driver_t;

typedef struct timing_source_t {
    uint64_t frequency;
    timing_source_type_t type;
    uint64_t driver_id[4];       // the registered 256-bit ID of the responsible driver
    uint64_t driver_source_idx;  // index by which the driver will select among
                                 // multiple sources of this type it is responsible for
    bool calibrated;
} timing_source_t;

timing_request_t* timing_create_request(uint64_t delay_nsec);
void timing_init(driver_list_entry_t** drivers);
void timer_set_alarm_relative(timing_request_t* req);

#endif