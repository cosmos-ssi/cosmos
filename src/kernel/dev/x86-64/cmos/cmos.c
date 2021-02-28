//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/x86-64/cmos/cmos.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>
#include <sys/objectmgr/objectmgr.h>

#include <sys/kprintf/kprintf.h>
#include <sys/objecttype/objecttype_cmos.h>

void cmos_write_register(uint8_t reg, uint8_t val) {
    uint8_t pv;

    asm_cli();
    asm_out_b(CMOS_REGISTER_SELECT_PORT, 0x80 | reg);  // disable NMI so we don't screw up the CMOS RTC irreparably
    pv = asm_in_b(CMOS_REGISTER_DATA_PORT);
    asm_out_b(CMOS_REGISTER_SELECT_PORT, 0x80 | reg);
    asm_out_b(CMOS_REGISTER_DATA_PORT, pv | val);

    asm_out_b(CMOS_REGISTER_SELECT_PORT, asm_in_b(CMOS_REGISTER_SELECT_PORT) & 0x7F);  // re-enable nmi
    asm_sti();

    return;
}

uint8_t cmos_read_register(uint8_t reg) {
    uint8_t b;

    asm_cli();
    asm_out_b(CMOS_REGISTER_SELECT_PORT, reg);
    b = asm_in_b(CMOS_REGISTER_DATA_PORT);
    asm_sti();

    return b;
}

/*
 * perform device instance specific init here
 */

uint8_t cmos_device_init(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Init %s (%s)\n", dev->description, dev->name);
    return 1;
}

void cmos_objectmgr_register_objects() {
    /*
     * register device
     */
    struct object* deviceinstance = objectmgr_new_object();
    objectmgr_set_object_description(deviceinstance, "i386 CMOS");
    deviceinstance->devicetype = CMOS;
    deviceinstance->init = &cmos_device_init;
    /*
     * api
     */
    //    struct objecttype_cmos* api = (struct objecttype_cmos*)kmalloc(sizeof(struct objecttype_cmos));
    //    deviceinstance->api = api;
    /*
     * register
     */
    objectmgr_register_object(deviceinstance);
}