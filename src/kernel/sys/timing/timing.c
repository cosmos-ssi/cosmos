/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/timing/hpet.h>
#include <subsystems.h>
#include <sys/kprintf/kprintf.h>
#include <sys/timing/timing.h>

void timing_init(driver_list_entry_t** drivers) {
    uint64_t i = 0;

    kprintf("Initializing timing subsystem...\n");

    while (drivers[i]) {
        i++;
    }

    kprintf("\tNumber of timing drivers: %llu\n", i);

    return;
}