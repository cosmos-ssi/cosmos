//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/System_Management_BIOS

#include <obj/x86-64/smbios/smbios.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/obj/objectmgr/objectmgr.h>

#include <sys/kprintf/kprintf.h>
#include <sys/obj/objectinterface/objectinterface_bda.h>

struct smbios_objectdata {
    uint64_t base;
};

uint64_t smbios_find() {
    uint8_t* mem = (uint8_t*)0xF0000;
    uint64_t length, i;
    uint8_t checksum;
    while ((uint64_t)mem < 0x100000) {
        if (mem[0] == '_' && mem[1] == 'S' && mem[2] == 'M' && mem[3] == '_') {
            length = mem[5];
            checksum = 0;
            for (i = 0; i < length; i++) {
                checksum += mem[i];
            }
            if (checksum == 0)
                break;
        }
        mem += 16;
    }
    if ((uint64_t)mem == 0x100000) {
        return 0;
    } else {
        return (uint64_t)mem;
    }
}

struct smbios_entry_point* smbios_get_smbios_entry_point(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct smbios_objectdata* object_data = (struct smbios_objectdata*)obj->object_data;
    return (struct smbios_entry_point*)object_data->base;
}

/*
 * perform device instance specific init here
 */
uint8_t smbios_obj_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct smbios_objectdata* object_data = (struct smbios_objectdata*)obj->object_data;
    object_data->base = smbios_find();
    if (0 != object_data->base) {
        kprintf("Init %s (%s) at Base %#hX\n", obj->description, obj->name, object_data->base);
        return 1;
    } else {
        // cant find the mem region, we cant init it
        return 0;
    }
}

void smbios_objectmgr_register_objects() {
    /*
     * register device
     */
    struct object* objectinstance = objectmgr_new_object();
    objectmgr_set_object_description(objectinstance, "SMBIOS");
    objectinstance->objectype = SMBIOS;
    objectinstance->init = &smbios_obj_init;
    /*
     * device data
     */
    struct smbios_objectdata* object_data = (struct smbios_objectdata*)kmalloc(sizeof(struct smbios_objectdata));
    object_data->base = 0;
    objectinstance->object_data = object_data;
    /*
     * register
     */
    objectmgr_register_object(objectinstance);
}