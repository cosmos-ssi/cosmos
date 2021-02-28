//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/array/array.h>
#include <sys/collection/arraylist/arraylist.h>
#include <sys/debug/assert.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kprintf/kprintf.h>
#include <sys/panic/panic.h>
#include <types.h>

#define NUMBER_INTERRUPTS 16

/**
 * array of lists
 */
struct array* interrupt_handlers;

/**
 * create the array of interrupts and then add a list for each entry
 */
void interrupt_router_init() {
    interrupt_handlers = array_new(NUMBER_INTERRUPTS);
    for (int i = 0; i < NUMBER_INTERRUPTS; i++) {
        array_set(interrupt_handlers, i, arraylist_new());
    }
}

/**
 * register an interrupt handler callback
 */
void interrupt_router_register_interrupt_handler(int interruptNumber, interrupt_handler func) {
    ASSERT_NOT_NULL(func);
    ASSERT_NOT_NULL(interrupt_handlers);

    if ((interruptNumber >= 0) && (interruptNumber < NUMBER_INTERRUPTS)) {
        struct arraylist* lst = array_get(interrupt_handlers, interruptNumber);
        if (0 == lst) {
            PANIC("List should not be null");
        }
        arraylist_add(lst, func);
    } else {
        PANIC("Invalid interrupt number");
    }
}

/**
 * route an interrupt
 */
void interrupt_router_route_interrupt(int interruptNumber, stack_frame* frame) {
    ASSERT_NOT_NULL(frame);
    ASSERT_NOT_NULL(interrupt_handlers);

    if ((8 != interruptNumber) && (0 != interruptNumber)) {
        kprintf("Routing IRQ %llu\n", interruptNumber);
    }
    if ((interruptNumber >= 0) && (interruptNumber < NUMBER_INTERRUPTS)) {
        struct arraylist* lst = array_get(interrupt_handlers, interruptNumber);
        if (0 == lst) {
            PANIC("List should not be null");
        }
        for (uint16_t i = 0; i < arraylist_count(lst); i++) {
            interrupt_handler handler = (interrupt_handler)arraylist_get(lst, i);
            if (0 != handler) {
                (*handler)(frame);
            } else {
                PANIC("Interrupt handler should not be null");
            }
        }
    } else {
        PANIC("Invalid interrupt number");
    }
}
