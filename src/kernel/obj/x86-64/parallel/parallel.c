//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/x86-64/bda/bda.h>
#include <obj/x86-64/parallel/parallel.h>
#include <sys/asm/io.h>
#include <sys/debug/assert.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_parallel.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/obj/objecttype/objectype.h>
#include <sys/sleep/sleep.h>
#include <sys/x86-64/idt/irq.h>
#include <types.h>

#define PARALLEL_DEVICE_LTP1_IRQ 7
#define PARALLEL_DEVICE_LTP2_IRQ 6
#define PARALLEL_DEVICE_LTP3_IRQ 5

#define PARALLEL_DEVICE_REGISTER_DATA 0x00
#define PARALLEL_DEVICE_REGISTER_STATUS 0x01
#define PARALLEL_DEVICE_REGISTER_CONTROL 0x02

// https://wiki.osdev.org/Parallel_port

struct parallel_objectdata {
    uint16_t address;
    uint16_t irq;
} __attribute__((packed));

void parallel_irq_handler(stack_frame* frame) {
    ASSERT_NOT_NULL(frame);
}

/*
 * wait until ready
 */
void parallel_device_ready(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct parallel_objectdata* object_data = (struct parallel_objectdata*)(obj->object_data);
    while (!(asm_in_b(object_data->address + PARALLEL_DEVICE_REGISTER_STATUS) & 0x80)) {
        sleep_wait(10);
    }
}

/*
 * perform device instance specific init here
 */
uint8_t parallel_obj_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct parallel_objectdata* object_data = (struct parallel_objectdata*)(obj->object_data);
    kprintf("Init %s at IRQ %llu Base %#hX (%s)\n", obj->description, object_data->irq, object_data->address,
            obj->name);
    interrupt_router_register_interrupt_handler(object_data->irq, &parallel_irq_handler);
    /*
     * reset
     */
    asm_out_w(object_data->address + PARALLEL_DEVICE_REGISTER_CONTROL, 0x00);
    return 1;
}

void parallel_write(struct object* obj, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    ASSERT_NOT_NULL(data);
    struct parallel_objectdata* object_data = (struct parallel_objectdata*)(obj->object_data);
    for (uint16_t i = 0; i < size; i++) {
        /*
         * wait for ready
         */
        parallel_device_ready(obj);
        /*
         * write byte
         */
        asm_out_w(object_data->address + PARALLEL_DEVICE_REGISTER_DATA, data[i]);
        /*
         * strobe
         */
        uint8_t lControl = asm_in_b(object_data->address + PARALLEL_DEVICE_REGISTER_CONTROL);
        asm_out_b(object_data->address + PARALLEL_DEVICE_REGISTER_CONTROL, lControl | 1);
        sleep_wait(10);
        asm_out_b(object_data->address + PARALLEL_DEVICE_REGISTER_CONTROL, lControl);
    }
}

void parallel_objectmgr_register_object(uint64_t base, uint8_t irq) {
    /*
     * register device
     */
    struct object* objectinstance = object_new_object();
    objectmgr_set_object_description(objectinstance, "Parallel Port");
    objectinstance->objectype = PARALLEL;
    objectinstance->init = &parallel_obj_init;
    /*
     * device api
     */
    struct objectinterface_parallel* api =
        (struct objectinterface_parallel*)kmalloc(sizeof(struct objectinterface_parallel));
    api->write = &parallel_write;
    objectinstance->api = api;
    /*
     * device data
     */
    struct parallel_objectdata* object_data = (struct parallel_objectdata*)kmalloc(sizeof(struct parallel_objectdata));
    object_data->address = base;
    object_data->irq = irq;
    objectinstance->object_data = object_data;
    /*
     * register
     */
    objectmgr_register_object(objectinstance);
}

void parallel_objectmgr_register_objects() {
    // lpt0
    uint16_t lpt1_base = bda_parallel0_base();
    if (0 != lpt1_base) {
        parallel_objectmgr_register_object(lpt1_base, PARALLEL_DEVICE_LTP1_IRQ);
    }
    // lpt1
    uint16_t lpt2_base = bda_parallel1_base();
    if (0 != lpt2_base) {
        parallel_objectmgr_register_object(lpt2_base, PARALLEL_DEVICE_LTP2_IRQ);
    }
    // lpt2
    uint16_t lpt3_base = bda_parallel2_base();
    if (0 != lpt3_base) {
        parallel_objectmgr_register_object(lpt3_base, PARALLEL_DEVICE_LTP3_IRQ);
    }
}