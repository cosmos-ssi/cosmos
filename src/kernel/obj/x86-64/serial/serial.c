//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/x86-64/bda/bda.h>
#include <obj/x86-64/serial/ns16550.h>
#include <obj/x86-64/serial/serial.h>
#include <sys/asm/asm.h>
#include <sys/collection/ringbuffer/ringbuffer.h>
#include <sys/debug/assert.h>
#include <sys/obj/objectmgr/objectmgr.h>

#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/objectinterface/objectinterface_serial.h>

#define SERIAL_RINGBUFFER_SIZE 255

#define SERIAL_DESCRIPTION "RS232"

struct serial_objectdata {
    uint8_t irq;
    uint16_t address;
    struct ringbuffer* buffer;

} __attribute__((packed));

int serial_is_transmit_empty() {
    struct rs232_16550* comport = (struct rs232_16550*)COM1_ADDRESS;
    uint8_t data = asm_in_b((uint64_t) & (comport->linestatus));
    return data & 0x20;
}

void serial_write_char(const uint8_t c) {
    struct rs232_16550* comport = (struct rs232_16550*)COM1_ADDRESS;

    while (serial_is_transmit_empty() == 0)
        ;
    asm_out_b((uint64_t) & (comport->data), c);
}

void serial_irq_handler_for_device(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    //    struct serial_objectdata* object_data = (struct serial_objectdata*)obj->object_data;

    // TODO figure out if it was THIS dev that made the interrupt and respond accordingly (like by putting the data into the ringbuffer)
    struct rs232_16550* comport = (struct rs232_16550*)COM1_ADDRESS;
    uint8_t data = asm_in_b((uint64_t) & (comport->data));

    // echo the data
    serial_write_char(data);
}
void serial_irq_handler(stack_frame* frame) {
    ASSERT_NOT_NULL(frame);
    objectmgr_find_objects_by_description(SERIAL, SERIAL_DESCRIPTION, &serial_irq_handler_for_device);
}

void serial_write_string(const uint8_t* c) {
    uint16_t i = 0;
    while (c[i] != 0) {
        serial_write_char(c[i++]);
    }
}

// https://wiki.osdev.org/Serial_Ports
void serial_init_port(uint64_t portAddress) {
    struct rs232_16550* comport = (struct rs232_16550*)portAddress;

    // enable interrupt on received data
    asm_out_b((uint64_t) & (comport->interrupt), 0x01);

    // set 38400 baud
    asm_out_b((uint64_t) & (comport->linecontrol), 0x80);
    asm_out_b((uint64_t) & (comport->data), 0x03);
    asm_out_b((uint64_t) & (comport->interrupt), 0x00);
    asm_out_b((uint64_t) & (comport->linecontrol), 0x03);

    // FIFO on
    asm_out_b((uint64_t) & (comport->fifocontrol), 0xC7);

    // IRQ on, RTD/DSR set
    asm_out_b((uint64_t) & (comport->modemcontrol), 0x0B);
}

/*
 * perform device instance specific init here
 */
uint8_t serial_obj_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    struct serial_objectdata* object_data = (struct serial_objectdata*)obj->object_data;
    kprintf("Init %s at IRQ %llu Base %#hX (%s)\n", obj->description, object_data->irq, object_data->address,
            obj->name);
    interrupt_router_register_interrupt_handler(object_data->irq, &serial_irq_handler);
    serial_init_port(object_data->address);
    return 1;
}

void serial_write(struct object* obj, const int8_t* c) {
    ASSERT_NOT_NULL(obj);
    serial_write_string(c);
}

void serial_register_device(uint8_t irq, uint64_t base) {
    /*
     * ISA serial port specific data
     */
    struct serial_objectdata* object_data = kmalloc(sizeof(struct serial_objectdata));
    object_data->irq = irq;
    object_data->address = base;
    object_data->buffer = ringbuffer_new(SERIAL_RINGBUFFER_SIZE);
    /*
     * the device instance
     */
    struct object* objectinstance = object_new_object();
    objectinstance->init = &serial_obj_init;
    objectinstance->object_data = object_data;
    objectinstance->objectype = SERIAL;
    objectmgr_set_object_description(objectinstance, SERIAL_DESCRIPTION);
    /*
     * the device api
     */
    struct objectinterface_serial* api = (struct objectinterface_serial*)kmalloc(sizeof(struct objectinterface_serial));
    api->write = &serial_write;
    objectinstance->api = api;
    /*
     * register
     */
    objectmgr_register_object(objectinstance);
}

/**
 * find all RS232 devices and register them
 */
void serial_objectmgr_register_objects() {
    // serial0
    uint16_t serial0_base = bda_serial0_base();
    if (0 != serial0_base) {
        serial_register_device(SERIAL_IRQ2, serial0_base);
    }
    // serial1
    uint16_t serial1_base = bda_serial1_base();
    if (0 != serial1_base) {
        serial_register_device(SERIAL_IRQ1, serial1_base);
    }
    // serial2
    uint16_t serial2_base = bda_serial2_base();
    if (0 != serial2_base) {
        serial_register_device(SERIAL_IRQ2, serial2_base);
    }
    // serial3
    uint16_t serial3_base = bda_serial3_base();
    if (0 != serial3_base) {
        serial_register_device(SERIAL_IRQ1, serial3_base);
    }
}
