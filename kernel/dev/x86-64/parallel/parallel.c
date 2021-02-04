//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/x86-64/bda/bda.h>
#include <dev/x86-64/parallel/parallel.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_parallel.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kprintf/kprintf.h>
#include <sys/sleep/sleep.h>

#define PARALLEL_DEVICE_LTP1_IRQ 7
#define PARALLEL_DEVICE_LTP2_IRQ 6
#define PARALLEL_DEVICE_LTP3_IRQ 5

#define PARALLEL_DEVICE_REGISTER_DATA 0x00
#define PARALLEL_DEVICE_REGISTER_STATUS 0x01
#define PARALLEL_DEVICE_REGISTER_CONTROL 0x02

// https://wiki.osdev.org/Parallel_port

struct parallel_devicedata {
    uint16_t address;
    uint16_t irq;
} __attribute__((packed));

void parallel_irq_handler(stack_frame* frame) {
    ASSERT_NOT_NULL(frame);
}

/*
 * wait until ready
 */
void parallel_device_ready(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct parallel_devicedata* device_data = (struct parallel_devicedata*)(dev->device_data);
    while (!(asm_in_b(device_data->address + PARALLEL_DEVICE_REGISTER_STATUS) & 0x80)) {
        sleep_wait(10);
    }
}

/*
 * perform device instance specific init here
 */
uint8_t parallel_device_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct parallel_devicedata* device_data = (struct parallel_devicedata*)(dev->device_data);
    kprintf("Init %s at IRQ %llu Base %#hX (%s)\n", dev->description, device_data->irq, device_data->address,
            dev->name);
    interrupt_router_register_interrupt_handler(device_data->irq, &parallel_irq_handler);
    /*
     * reset
     */
    asm_out_w(device_data->address + PARALLEL_DEVICE_REGISTER_CONTROL, 0x00);
    return 1;
}

void parallel_write(struct device* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    ASSERT_NOT_NULL(data);
    struct parallel_devicedata* device_data = (struct parallel_devicedata*)(dev->device_data);
    for (uint16_t i = 0; i < size; i++) {
        /*
         * wait for ready
         */
        parallel_device_ready(dev);
        /*
         * write byte
         */
        asm_out_w(device_data->address + PARALLEL_DEVICE_REGISTER_DATA, data[i]);
        /*
         * strobe
         */
        uint8_t lControl = asm_in_b(device_data->address + PARALLEL_DEVICE_REGISTER_CONTROL);
        asm_out_b(device_data->address + PARALLEL_DEVICE_REGISTER_CONTROL, lControl | 1);
        sleep_wait(10);
        asm_out_b(device_data->address + PARALLEL_DEVICE_REGISTER_CONTROL, lControl);
    }
}

void parallel_devicemgr_register_device(uint64_t base, uint8_t irq) {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    devicemgr_set_device_description(deviceinstance, "Parallel Port");
    deviceinstance->devicetype = PARALLEL;
    deviceinstance->init = &parallel_device_init;
    /*
     * device api
     */
    struct deviceapi_parallel* api = (struct deviceapi_parallel*)kmalloc(sizeof(struct deviceapi_parallel));
    api->write = &parallel_write;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct parallel_devicedata* device_data = (struct parallel_devicedata*)kmalloc(sizeof(struct parallel_devicedata));
    device_data->address = base;
    device_data->irq = irq;
    deviceinstance->device_data = device_data;
    /*
     * register
     */
    devicemgr_register_device(deviceinstance);
}

void parallel_devicemgr_register_devices() {
    // lpt0
    uint16_t lpt1_base = bda_parallel0_base();
    if (0 != lpt1_base) {
        parallel_devicemgr_register_device(lpt1_base, PARALLEL_DEVICE_LTP1_IRQ);
    }
    // lpt1
    uint16_t lpt2_base = bda_parallel1_base();
    if (0 != lpt2_base) {
        parallel_devicemgr_register_device(lpt2_base, PARALLEL_DEVICE_LTP2_IRQ);
    }
    // lpt2
    uint16_t lpt3_base = bda_parallel2_base();
    if (0 != lpt3_base) {
        parallel_devicemgr_register_device(lpt3_base, PARALLEL_DEVICE_LTP3_IRQ);
    }
}