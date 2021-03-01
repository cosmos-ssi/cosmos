//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_pit.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/sleep/sleep.h>
#include <tests/obj/test_pit.h>
#include <types.h>

void test_pit() {
    // get the PIT
    struct object* pit = objectmgr_find_object_by_name("pit0");
    if (0 != pit) {
        struct objectinterface_pit* pit_api = (struct objectinterface_pit*)pit->api;
        pit_tickcount_function tickcount_func = pit_api->tickcount;

        // show the tick count, since we can
        uint64_t tc = (*tickcount_func)(pit);
        kprintf("Ticks: %llu\n", tc);
        sleep_wait(1000);
        tc = (*tickcount_func)(pit);
        kprintf("Ticks: %llu\n", tc);
    } else {
        kprintf("Unable to find pit0\n");
    }
}