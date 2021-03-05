//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/arraylist/arraylist.h>
#include <sys/debug/assert.h>
#include <sys/obj/objecttype/objectype.h>
#include <sys/obj/objecttypes/objecttypes.h>
#include <sys/panic/panic.h>

struct arraylist* types;

void objecttypes_init() {
    types = arraylist_new();
    // objecttypes_add(objecttype_new("None", OBJECT_TYPE_NONE));
    objecttypes_add(objecttype_new("serial", OBJECT_TYPE_SERIAL));
    objecttypes_add(objecttype_new("vga", OBJECT_TYPE_VGA));
    objecttypes_add(objecttype_new("rtc", OBJECT_TYPE_RTC));
    objecttypes_add(objecttype_new("kb", OBJECT_TYPE_KEYBOARD));
    objecttypes_add(objecttype_new("nic", OBJECT_TYPE_NIC));
    objecttypes_add(objecttype_new("hb", OBJECT_TYPE_BRIDGE));
    objecttypes_add(objecttype_new("usb", OBJECT_TYPE_USB));
    objecttypes_add(objecttype_new("ata", OBJECT_TYPE_ATA));
    objecttypes_add(objecttype_new("pic", OBJECT_TYPE_PIC));
    objecttypes_add(objecttype_new("mouse", OBJECT_TYPE_MOUSE));
    objecttypes_add(objecttype_new("floppy", OBJECT_TYPE_FLOPPY));
    objecttypes_add(objecttype_new("speaker", OBJECT_TYPE_SPEAKER));
    objecttypes_add(objecttype_new("pit", OBJECT_TYPE_PIT));
    objecttypes_add(objecttype_new("dsp", OBJECT_TYPE_DSP));
    objecttypes_add(objecttype_new("cmos", OBJECT_TYPE_CMOS));
    objecttypes_add(objecttype_new("isadma", OBJECT_TYPE_ISADMA));
    objecttypes_add(objecttype_new("cpu", OBJECT_TYPE_CPU));
    objecttypes_add(objecttype_new("rd", OBJECT_TYPE_RAMDISK));
    objecttypes_add(objecttype_new("vnic", OBJECT_TYPE_VNIC));
    objecttypes_add(objecttype_new("vblock", OBJECT_TYPE_VBLOCK));
    objecttypes_add(objecttype_new("disk", OBJECT_TYPE_DISK));
    objecttypes_add(objecttype_new("parallel", OBJECT_TYPE_PARALLEL));
    objecttypes_add(objecttype_new("bda", OBJECT_TYPE_BDA));
    objecttypes_add(objecttype_new("acpi", OBJECT_TYPE_ACPI));
    objecttypes_add(objecttype_new("swap", OBJECT_TYPE_SWAP));
    objecttypes_add(objecttype_new("fs", OBJECT_TYPE_FILESYSTEM));
    objecttypes_add(objecttype_new("pt", OBJECT_TYPE_PARTITION_TABLE));
    objecttypes_add(objecttype_new("con", OBJECT_TYPE_CONSOLE));
    objecttypes_add(objecttype_new("par", OBJECT_TYPE_PARTITION));
    objecttypes_add(objecttype_new("null", OBJECT_TYPE_NULL0));
    objecttypes_add(objecttype_new("tick", OBJECT_TYPE_TICK));
    objecttypes_add(objecttype_new("rand", OBJECT_TYPE_RAND));
    objecttypes_add(objecttype_new("ip", OBJECT_TYPE_IP));
    objecttypes_add(objecttype_new("tcp", OBJECT_TYPE_TCP));
    objecttypes_add(objecttype_new("arp", OBJECT_TYPE_ARP));
    objecttypes_add(objecttype_new("icmp", OBJECT_TYPE_ICMP));
    objecttypes_add(objecttype_new("eth", OBJECT_TYPE_ETHERNET));
    objecttypes_add(objecttype_new("sdhci", OBJECT_TYPE_SDHCI));
    objecttypes_add(objecttype_new("smbios", OBJECT_TYPE_SMBIOS));
    objecttypes_add(objecttype_new("objfs", OBJECT_TYPE_OBJFS));
    objecttypes_add(objecttype_new("voh", OBJECT_TYPE_VOH));
    objecttypes_add(objecttype_new("bga", OBJECT_TYPE_BGA));
    objecttypes_add(objecttype_new("kernelmap", OBJECT_TYPE_KERNELMAP));
    objecttypes_add(objecttype_new("udp", OBJECT_TYPE_UDP));
    objecttypes_add(objecttype_new("telnet", OBJECT_TYPE_TELNET));
    objecttypes_add(objecttype_new("user", OBJECT_TYPE_USER));
    objecttypes_add(objecttype_new("group", OBJECT_TYPE_GROUP));
}

uint32_t objecttypes_count() {
    ASSERT_NOT_NULL(types);
    return arraylist_count(types);
}

struct object_type* objecttypes_find(uint16_t id) {
    ASSERT_NOT_NULL(types);
    ASSERT_NOT_NULL(id);
    for (uint32_t i = 0; i < arraylist_count(types); i++) {
        struct object_type* ot = (struct object_type*)arraylist_get(types, i);
        ASSERT_NOT_NULL(ot);
        if (ot->id == id) {
            return ot;
        }
    }
    return 0;
}

struct object_type* objecttypes_get(uint32_t i) {
    ASSERT_NOT_NULL(types);
    ASSERT(i < arraylist_count(types));
    return (struct object_type*)arraylist_get(types, i);
}

void objecttypes_add(struct object_type* ot) {
    ASSERT_NOT_NULL(types);
    ASSERT_NOT_NULL(ot);
    ASSERT_NOT_NULL(ot->id);
    ASSERT_NOT_NULL(ot->name);
    if (0 == objecttypes_find(ot->id)) {
        arraylist_add(types, ot);
    } else {
        PANIC("Duplicate object type");
    }
}

void objecttypes_dump() {
    ASSERT_NOT_NULL(types);
    kprintf("***** All Object Types *****\n");
    for (uint32_t i = 0; i < arraylist_count(types); i++) {
        struct object_type* ot = (struct object_type*)arraylist_get(types, i);
        ASSERT_NOT_NULL(ot);
        kprintf("   Object type %#llX: %s\n", ot->id, ot->name);
    }
}
