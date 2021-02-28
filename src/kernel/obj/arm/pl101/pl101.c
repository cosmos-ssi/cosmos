//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/arm/pl101/pl101.h>
#include <sys/asm/asm.h>
#include <sys/collection/ringbuffer/ringbuffer.h>
#include <sys/debug/assert.h>
#include <sys/objectmgr/objectmgr.h>

#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objecttype/objecttype_serial.h>

// void serial_irq_handler(stack_frame *frame){
//	ASSERT_NOT_NULL(frame);
//    struct rs232_16550* comport = (struct rs232_16550*) COM1_ADDRESS;
//    uint8_t data = asm_in_b((uint64_t)&(comport->data));

// echo the data
//    serial_write_char(data);
//}

/*
 * perform device instance specific init here
 */
void serial_device_init(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    //    struct serial_devicedata* object_data = (struct serial_devicedata*) dev->object_data;
    //    kprintf("Init %s at IRQ %llu Base %#hX (%s)\n",dev->description, object_data->irq, object_data->address, dev->name);
    //    interrupt_router_register_interrupt_handler(object_data->irq, &serial_irq_handler);
    //    serial_init_port(object_data->address);
}

void serial_register_device(uint8_t irq, uint64_t base) {
    /*
     * ISA serial port specific data
     */
    //   struct serial_devicedata* object_data = kmalloc(sizeof(struct serial_devicedata));
    //   object_data->irq=irq;
    //   object_data->address=base;
    //   object_data->buffer = ringbuffer_new();
    /*
     * the device instance
     */
    struct object* deviceinstance = objectmgr_new_object();
    deviceinstance->init = &serial_device_init;
    //    deviceinstance->object_data = object_data;
    deviceinstance->devicetype = SERIAL;
    objectmgr_set_object_description(deviceinstance, "PL101");
    /*
     * the device api
     */
    //    struct objecttype_serial* api = (struct objecttype_serial*) kmalloc(sizeof(struct objecttype_serial));
    //  api->write = &serial_write;
    //  deviceinstance->api = api;
    /*
     * register
     */
    objectmgr_register_object(deviceinstance);
}

/**
 * find all RS232 devices and register them
 */
void serial_objectmgr_register_objects() {
    //   serial_register_device(SERIAL_IRQ2,COM1_ADDRESS);

    // TODO add code to check if these even exist
    //    registerRS232Device(SERIAL_IRQ1,COM2_ADDRESS);
    //    registerRS232Device(SERIAL_IRQ2,COM3_ADDRESS);
    //    registerRS232Device(SERIAL_IRQ1,COM4_ADDRESS);
}
