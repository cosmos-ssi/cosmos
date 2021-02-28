/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <obj/x86-64/speaker/speaker.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>
#include <sys/obj/objectmgr/objectmgr.h>

#include <sys/kprintf/kprintf.h>
#include <sys/obj/objectinterface/objectinterface_speaker.h>
#include <sys/sleep/sleep.h>

// https://wiki.osdev.org/PC_Speaker

#define SPEAKER_PORT 0x61

/*
 * perform device instance specific init here
 */
uint8_t speaker_obj_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    kprintf("Init %s (%s)\n", obj->description, obj->name);
    return 1;
}

// make it shutup
static void nosound() {
    uint8_t tmp = asm_in_b(SPEAKER_PORT) & 0xFC;
    asm_out_b(SPEAKER_PORT, tmp);
}

void play_sound(uint32_t frequency) {
    uint32_t div;
    uint8_t tmp;

    // set the PIT to the desired frequency
    div = 1193180 / frequency;
    asm_out_b(0x43, 0xb6);
    asm_out_b(0x42, (uint8_t)(div));
    asm_out_b(0x42, (uint8_t)(div >> 8));

    // And play the sound using the PC speaker
    tmp = asm_in_b(SPEAKER_PORT);
    if (tmp != (tmp | 3)) {
        asm_out_b(SPEAKER_PORT, tmp | 3);
    }
}

// Make a beep
void speaker_beep(struct object* obj, uint32_t frequency, uint32_t milliseconds) {
    ASSERT_NOT_NULL(obj);
    play_sound(frequency);
    sleep_wait(milliseconds);
    nosound();
    // set_PIT_2(old_frequency);
}

void speaker_objectmgr_register_objects() {
    /*
     * register device
     */
    struct object* objectinstance = object_new_object();
    objectmgr_set_object_description(objectinstance, "Speaker");
    objectinstance->objectype = SPEAKER;
    objectinstance->init = &speaker_obj_init;
    /*
     * device api
     */
    struct objectinterface_speaker* api =
        (struct objectinterface_speaker*)kmalloc(sizeof(struct objectinterface_speaker));
    api->beep = &speaker_beep;
    objectinstance->api = api;
    /**
     * register
     */
    objectmgr_register_object(objectinstance);
}
