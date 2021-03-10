//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/x86-64/smbios/smbios.h>
#include <sys/debug/assert.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <tests/obj/test_smbios.h>
#include <types.h>

void test_smbios() {
    kprintf("Testing SMBIOS\n");
    uint8_t devicename[] = {"smbios0"};

    /*
     * find the SMBIOS disk
     */
    struct object* smbios = objectmgr_find_object_by_name(devicename);
    if (0 != smbios) {
        ASSERT_NOT_NULL(smbios->object_data);
        struct smbios_entry_point* smbios_entry = smbios_get_smbios_entry_point(smbios);

        ASSERT_NOT_NULL(smbios_entry);
        ASSERT(smbios_entry->entry_point_string[0] == '_');
        ASSERT(smbios_entry->entry_point_string[1] == 'S');
        ASSERT(smbios_entry->entry_point_string[2] == 'M');
        ASSERT(smbios_entry->entry_point_string[3] == '_');
        //     kprintf("structures %llu, table len %llu, table address %#hX\n", smbios_entry->number_of_structures,
        //            smbios_entry->table_length, smbios_entry->table_address);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
