/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020-2021 Tom Everett                           *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <obj/x86-64/pit/pit.h>
#include <sys/collection/arraylist/arraylist.h>
#include <sys/debug/assert.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objectinterface/objectinterface_pit.h>

// https://wiki.osdev.org/Programmable_Interval_Timer
// http://www.osdever.net/bkerndev/Docs/pit.htm

#define PIT_IRQ 0x00

#define PIT_PORT_0 0x40
#define PIT_PORT_1 0x41
#define PIT_PORT_2 0x42
#define PIT_PORT_COMMAND 0x43

struct arraylist* pitEvents;

uint64_t tickcount = 0;

#define PIT_HZ 1  // 10 interrupts per second

// This is the perfect place to handle context switches.  Just saying.
void pit_handle_irq(stack_frame* frame) {
    ASSERT_NOT_NULL(pitEvents);
    ASSERT_NOT_NULL(frame);
    //  kprintf("@");
    tickcount = tickcount + 1;
}

/*
 * perform device instance specific init here
 */
/*
 * According to: https://wiki.osdev.org/Programmable_Interval_Timer
 *
 * "Typically during boot the BIOS sets channel 0 with a count of 65535
 *  or 0 (which translates to 65536), which gives an output frequency of
 *  18.2065 Hz (or an IRQ every 54.9254 ms)"
 */
uint8_t pit_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    kprintf("Init %s at IRQ %llu (%s)\n", obj->description, PIT_IRQ, obj->name);
    interrupt_router_register_interrupt_handler(PIT_IRQ, &pit_handle_irq);
    return 1;
}

uint64_t pit_tickcount(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    return tickcount;
}

void pit_subscribe(pit_event event) {
    ASSERT_NOT_NULL(pitEvents);
    ASSERT_NOT_NULL(event);
    arraylist_add(pitEvents, event);
}

void pit_objectmgr_register_objects() {
    pitEvents = arraylist_new();
    /*
     * register device
     */
    struct object* objectinstance = objectmgr_new_object();
    objectmgr_set_object_description(objectinstance, "8253/8254 PIT");
    objectinstance->objectype = PIT;
    objectinstance->init = &pit_init;
    /*
     * device api
     */
    struct objectinterface_pit* api = (struct objectinterface_pit*)kmalloc(sizeof(struct objectinterface_pit));
    api->tickcount = &pit_tickcount;
    api->subscribe = &pit_subscribe;
    objectinstance->api = api;
    /*
     * register
     */
    objectmgr_register_object(objectinstance);
}
