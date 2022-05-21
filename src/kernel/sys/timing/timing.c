
/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/timing/hpet/hpet.h>
#include <subsystems.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/panic/panic.h>
#include <sys/sync/sync.h>
#include <sys/timing/timerapi.h>
#include <sys/timing/timing.h>

const uint64_t one_billion = 1000000000;
struct {
    timing_source_t* sources;
    uint64_t count;
} timing_sources;

struct {
    timing_driver_t* info;
    uint64_t count;
} timing_driver_info;

timing_driver_t* timing_find_driver(uint64_t id[4]);
uint64_t timing_get_request_id();
timing_source_descriptor timing_select_best_source(uint64_t interval_ns);

timing_request_t* timing_create_request(uint64_t delay_nsec) {
    timing_request_t* req;

    req = kmalloc(sizeof(timing_request_t));

    req->delay_nsec = delay_nsec;
    req->request_id = timing_get_request_id();
    req->status = TIMING_REQUEST_STATUS_INITIATED;

    return req;
}

uint64_t timing_get_request_id() {
    static function_spinlock lock = false;
    static uint64_t request_next = 0;
    uint64_t retval;

    FUNCTION_SPINLOCK_ACQUIRE(lock);
    retval = request_next;
    request_next++;
    FUNCTION_SPINLOCK_RELEASE(lock);

    return retval;
}

timing_driver_t* timing_find_driver(uint64_t id[4]) {
    uint64_t i;

    for (i = 0; i < timing_driver_info.count; i++) {
        if (driver_compare_id(id, timing_driver_info.info[i].driver_id)) {
            return &timing_driver_info.info[i];
        }
    }

    return NULL;
}

void timing_init(driver_list_entry_t** drivers) {
    uint64_t i = 0, j = 0;
    timing_source_t* tmp;

    kprintf("Initializing timing subsystem...\n");

    timing_sources.sources = NULL;
    timing_sources.count = 0;
    timing_driver_info.info = NULL;
    timing_driver_info.count = 0;

    while (drivers[i]) {
        timing_driver_info.info = krealloc(timing_driver_info.info, sizeof(timing_driver_t) * (i + 1));

        switch (drivers[i]->driver_interface_version) {
            case 1:
                tmp = ((driver_info_1_t*)drivers[i]->driver_info)
                          ->init_func(drivers[i], (void*)&timing_driver_info.info[i]);
                break;
            default:
                PANIC("Invalid driver interface version");
                break;
        }

        timing_sources.sources =
            krealloc(timing_sources.sources,
                     sizeof(timing_source_t) * (timing_sources.count + timing_driver_info.info[i].num_sources));

        for (j = 0; j < timing_driver_info.info[i].num_sources; j++) {
            timing_sources.sources[timing_sources.count + j] = tmp[j];
        }
        timing_sources.count += timing_driver_info.info[i].num_sources;

        i++;
    }

    timing_driver_info.count = i;

    system_sleep(10000000000);
    system_sleep(30000000000);
    asm_sti();

    return;
}

timing_source_descriptor timing_select_best_source(uint64_t interval_ns) {
    /* 
     * Timers are generally a trade-off between precision and accuracy, so we
     * choose the timer with the lowest precision (lowest frequency) that gives
     * us the resolution we need for the requested interval.  After that, if
     * there are multiple such sources, we choose the one with the fewest active
     * requests, to distribute the load.
     */
    uint64_t i;
    uint64_t source_interval_ns = 0, best_source_interval_ns = 0;
    uint64_t best_descriptor = 0;

    for (i = 0; i < timing_sources.count; i++) {
        source_interval_ns = one_billion / timing_sources.sources[i].frequency;

        /*
         * We want the largest interval (lowest frequency, which generally means
         * higher accuracy) that is smaller than the requested interval (so we
         * don't wait too long)
         */
        if ((source_interval_ns <= interval_ns) && (source_interval_ns > best_source_interval_ns)) {
            best_source_interval_ns = source_interval_ns;
            best_descriptor = i;
        }

        // TODO: Handle the case where no source interval is less than the requested interval
    }

    // TODO: Balance the load evenly among equally-good sources.

    /*kprintf("Best, frequency, interval, request: %llu, %llu, %llu, %llu\n", best_descriptor,
            timing_sources.sources[best_descriptor].frequency,
            one_billion / timing_sources.sources[best_descriptor].frequency, interval_ns);*/
    return best_descriptor;
}

void timer_set_alarm_relative(timing_request_t* req) {
    timing_source_descriptor tsd;
    timing_driver_t* driver;

    tsd = timing_select_best_source(req->delay_nsec);

    ASSERT(tsd <= timing_sources.count);

    driver = timing_find_driver(timing_sources.sources[tsd].driver_id);

    driver->api.set_deadline_relative(req);

    return;
}