/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/timing/hpet.h>
#include <subsystems.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/panic/panic.h>
#include <sys/timing/timing.h>

struct {
    timing_source_t* sources;
    uint64_t count;
} timing_sources;

struct {
    timing_driver_t* info;
    uint64_t count;
} timing_driver_info;

uint64_t max_timing_source_id;

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

    return;
}

void timer_set_alarm_relative(uint64_t ns) {
    return;
}