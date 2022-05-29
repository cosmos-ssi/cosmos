/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2022 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <subsystems.h>
#include <sys/debug/assert.h>
#include <sys/kprintf/kprintf.h>

void block_init(driver_list_entry_t** drivers) {
    uint64_t i = 0;

    kprintf("Initializing block subsystem...\n");

    while (drivers[i]) {
        switch (drivers[i]->driver_interface_version) {
            case 1:
                break;
            default:
                PANIC("Invalid driver interface version!");
                break;
        }

        i++;
    }
}