/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2022 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <api/api_object.h>
#include <subsystems.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>

api_object_t** block_devices = NULL;

void block_init(driver_list_entry_t** drivers) {
    uint64_t i = 0;
    void* tmp;
    //api_object_t* new_api_object;

    kprintf("Initializing block subsystem...\n");

    while (drivers[i]) {
        block_devices = krealloc(block_devices, sizeof(api_object_t*) * (i + 1));

        block_devices[i] = kmalloc(sizeof(api_object_t));

        switch (drivers[i]->driver_interface_version) {
            case 1:
                tmp = drivers[i]->driver_info;
                break;
            default:
                PANIC("Invalid driver interface version!");
                break;
        }

        i++;
    }
}