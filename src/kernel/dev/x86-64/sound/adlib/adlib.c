/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020-2021 Tom Everett                           *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/x86-64/sound/adlib/adlib.h>
#include <sys/asm/asm.h>
#include <sys/collection/arraylist/arraylist.h>
#include <sys/debug/assert.h>
#include <sys/objectmgr/objectmgr.h>

#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objecttype/objecttype_dsp.h>

// http://shipbrook.net/jeff/sb.html

#define ADLIB_ADDRESS_STATUS 0x388
#define ADLIB_DATA 0x389

#define ADLIB_WAVEFORM 0x01
#define ADLIB_TIMER1 0x02
#define ADLIB_TIMER2 0x03
#define ADLIB_CONTROL 0x04

/*
01	Test LSI / Enable waveform control
02	Timer 1 data
03	Timer 2 data
04	Timer control flags
08	Speech synthesis mode / Keyboard split note select
20..35	Amp Mod / Vibrato / EG type / Key Scaling / Multiple
40..55	Key scaling level / Operator output level
60..75	Attack Rate / Decay Rate
80..95	Sustain Level / Release Rate
A0..A8	Frequency (low 8 bits)
B0..B8	Key On / Octave / Frequency (high 2 bits)
BD	AM depth / Vibrato depth / Rhythm control
C0..C8	Feedback strength / Connection type
E0..F5	Wave Select
*/

void adlib_handle_irq(stack_frame* frame) {
    ASSERT_NOT_NULL(frame);
}

uint8_t adlib_read_status() {
    return asm_in_b(0x388);
}

/*
 * perform device instance specific init here
 */
uint8_t adlib_device_init(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Init %s\n", dev->description);
    //   interrupt_router_register_interrupt_handler(SB16_IRQ, &adlib_handle_irq);
    return 1;
}

void adlib_objectmgr_register_objects() {
    /*
     * register device
     */
    struct object* deviceinstance = objectmgr_new_object();
    objectmgr_set_object_description(deviceinstance, "Yamaha YM3812 (OPL2)");
    deviceinstance->devicetype = DSP;
    deviceinstance->init = &adlib_device_init;
    /*
     * device api
     */
    struct objecttype_dsp* api = (struct objecttype_dsp*)kmalloc(sizeof(struct objecttype_dsp));
    deviceinstance->api = api;
    /*
     * register
     */
    objectmgr_register_object(deviceinstance);
}
