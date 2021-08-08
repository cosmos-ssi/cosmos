//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/virtio/virtio.h>
#include <obj/x86-64/ata/ata_controller.h>
#include <obj/x86-64/bda/bda.h>
#include <obj/x86-64/bga/bga.h>
#include <obj/x86-64/bridge/bridge.h>
#include <obj/x86-64/cmos/cmos.h>
#include <obj/x86-64/cpu/cpu.h>
#include <obj/x86-64/isadma/isadma.h>
#include <obj/x86-64/kernelmap/kernelmap.h>
#include <obj/x86-64/keyboard/keyboard.h>
#include <obj/x86-64/mouse/mouse.h>
#include <obj/x86-64/network/network.h>
#include <obj/x86-64/parallel/parallel.h>
#include <obj/x86-64/pci/pci.h>
#include <obj/x86-64/pci_ehci/pci_ehci.h>
#include <obj/x86-64/pit/pit.h>
#include <obj/x86-64/rtc/rtc.h>
#include <obj/x86-64/sdhci/sdhci.h>
#include <obj/x86-64/serial/serial.h>
#include <obj/x86-64/smbios/smbios.h>
#include <obj/x86-64/sound/sound.h>
#include <obj/x86-64/speaker/speaker.h>
#include <obj/x86-64/usb_ehci/usb_ehci.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/obj/objectregistry/objectregistry.h>
#include <sys/obj/objecttype/objectype.h>
#include <sys/obj/objecttypes/objecttypes.h>
#include <sys/panic/panic.h>
#include <sys/string/string.h>
#include <sys/x86-64/interrupts/pic.h>

#define MAX_DEVICE_NAME_LENGTH 128

void objectmgr_init() {
    objecttypes_init();
    objectregistry_init();
}

int8_t* createDeviceName(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->objectype);

    int8_t nn[32];
    int8_t* ret = kmalloc(MAX_DEVICE_NAME_LENGTH);
    struct object_type* ot = objecttypes_find(obj->objectype);
    ASSERT_NOT_NULL(ot);
    ASSERT_NOT_NULL(ot->id);
    ASSERT_NOT_NULL(ot->name);
    strncpy(ret, ot->name, MAX_DEVICE_NAME_LENGTH);
    uitoa3(obj->type_index, nn, 32, 10);
    ret = strncat(ret, nn, MAX_DEVICE_NAME_LENGTH);
    return ret;
}

void objectmgr_register_object(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->description);
    ASSERT_NOT_NULL(obj->objectype);
    ASSERT_NOT_NULL(obj->init);
    /*
     * set index
     */
    obj->type_index = objectregistry_objectcount_type(obj->objectype);
    /*
     * create name
     */
    obj->name = createDeviceName(obj);
    /*
     * register
     */
    objectregistry_registerobject(obj);
}

void objectmgr_unregister_object(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    /*
     * unregister
     */
    objectregistry_unregisterobject(obj);
}

uint16_t objectmgr_object_count() {
    return objectregistry_objectcount();
}

void obj_initIterator(struct object* obj) {
    if (0 != obj) {
        if (0 == obj->init(obj)) {
            kprintf("Failed to Initialize %s\n", obj->name);
        }
    } else {
        PANIC("um. why is there a null device?");
    }
}

/*
 * init order matters
 */
void objectmgr_init_objects() {
    //   kprintf("Initializing Devices\n");
    /*
     * CPU first before first?
     */
    objectregistry_iterate_type(OBJECT_TYPE_CPU, obj_initIterator);
    /*
     * BDA, EBDA
     */
    objectregistry_iterate_type(OBJECT_TYPE_BDA, obj_initIterator);
    /*
     * Serial next
     */
    objectregistry_iterate_type(OBJECT_TYPE_SERIAL, obj_initIterator);
    /*
     * the PIT
     */
    objectregistry_iterate_type(OBJECT_TYPE_PIT, obj_initIterator);
    /*
     * CMOS
     */
    objectregistry_iterate_type(OBJECT_TYPE_CMOS, obj_initIterator);
    /*
     * DMA
     */
    objectregistry_iterate_type(OBJECT_TYPE_ISADMA, obj_initIterator);
    /*
     * virtual devices
     */
    objectregistry_iterate_type(OBJECT_TYPE_VNIC, obj_initIterator);
    objectregistry_iterate_type(OBJECT_TYPE_VBLOCK, obj_initIterator);
    /*
     * everything else
     */
    objectregistry_iterate_type(OBJECT_TYPE_RTC, obj_initIterator);
    objectregistry_iterate_type(OBJECT_TYPE_KEYBOARD, obj_initIterator);
    objectregistry_iterate_type(OBJECT_TYPE_VGA, obj_initIterator);
    objectregistry_iterate_type(OBJECT_TYPE_BGA, obj_initIterator);
    objectregistry_iterate_type(OBJECT_TYPE_USB, obj_initIterator);
    objectregistry_iterate_type(OBJECT_TYPE_NIC, obj_initIterator);
    objectregistry_iterate_type(OBJECT_TYPE_BRIDGE, obj_initIterator);
    objectregistry_iterate_type(OBJECT_TYPE_ATA, obj_initIterator);
    objectregistry_iterate_type(OBJECT_TYPE_MOUSE, obj_initIterator);
    objectregistry_iterate_type(OBJECT_TYPE_FLOPPY, obj_initIterator);
    objectregistry_iterate_type(OBJECT_TYPE_SPEAKER, obj_initIterator);
    objectregistry_iterate_type(OBJECT_TYPE_DSP, obj_initIterator);
    objectregistry_iterate_type(OBJECT_TYPE_DISK, obj_initIterator);
    objectregistry_iterate_type(OBJECT_TYPE_SDHCI, obj_initIterator);
    objectregistry_iterate_type(OBJECT_TYPE_SMBIOS, obj_initIterator);

    //  objectregistry_iterate_type(RAMDISK, obj_initIterator);
    objectregistry_iterate_type(OBJECT_TYPE_PARALLEL, obj_initIterator);
    //  objectregistry_iterate_type(SWAP, obj_initIterator);
    objectregistry_iterate_type(OBJECT_TYPE_KERNELMAP, obj_initIterator);
}

void objectmgr_set_object_description(struct object* obj, const uint8_t* description) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(description);
    ASSERT(strlen(description) < OBJECT_MAX_DESCRIPTION);
    ASSERT(strlen(description) > 0);

    strncpy(obj->description, description, OBJECT_MAX_DESCRIPTION);

    //    kprintf("%s\n", obj->description);
}

struct object* objectmgr_find_object_by_name(const uint8_t* name) {
    ASSERT_NOT_NULL(name);
    return objectregistry_find_object_by_name(name);
}

struct object* objectmgr_find_object_by_handle(uint64_t handle) {
    ASSERT_NOT_NULL(handle);
    return objectregistry_find_object_by_handle(handle);
}

void objectmgr_find_objects_by_description(uint16_t dt, const uint8_t* description, objectSearchCallback cb) {
    ASSERT_NOT_NULL(description);
    ASSERT_NOT_NULL(cb);
    ASSERT_NOT_NULL(dt);
    objectregistry_find_objects_by_description(dt, description, cb);
}

void objectmgr_find_objects_by_object_type(uint16_t dt, objectSearchCallback cb) {
    ASSERT_NOT_NULL(cb);
    ASSERT_NOT_NULL(dt);
    objectregistry_find_objects_by_objectype(dt, cb);
}

void objectmgr_register_objects() {
    /*
     * scan the PCI bus first
     */
    pci_init();
    /*
     * and then RS232
     */
    serial_objectmgr_register_objects();
    /*
     * and the then the PIT
     */
    pit_objectmgr_register_objects();
    /*
     * we need the CMOS
     */
    cmos_objectmgr_register_objects();
    /*
     * ISA DMA Controller
     */
    isadma_objectmgr_register_objects();
    /*
     * rest of this stuff can really happen in any order
     */
    //   swap_objectmgr_register_objects();
    rtc_objectmgr_register_objects();
    keyboard_objectmgr_register_objects();
    //  vga_objectmgr_register_objects();
    bga_objectmgr_register_objects();
    usb_ehci_objectmgr_register_objects();
    network_objectmgr_register_objects();
    bridge_objectmgr_register_objects();
    sdhci_objectmgr_register_objects();
    ata_objectmgr_register_objects();
    mouse_objectmgr_register_objects();
    // floppy_objectmgr_register_objects();
    speaker_objectmgr_register_objects();
    sound_objectmgr_register_objects();
    cpu_objectmgr_register_objects();
    virtio_objectmgr_register_objects();
    //  ramdisk_objectmgr_register_objects();
    pci_ehci_objectmgr_register_objects();
    parallel_objectmgr_register_objects();
    bda_objectmgr_register_objects();
    smbios_objectmgr_register_objects();
    kernelmap_objectmgr_register_objects();
}

//void objectmgr_register_objects() {
//    pl101_objectmgr_register_objects();
//}

// attach a device (non-fixed devices... like RAM disks and SWAP)
uint8_t objectmgr_attach_object(struct object* obj) {
    ASSERT_NOT_NULL(obj);

    /*
     * register
     */
    objectmgr_register_object(obj);
    /*
     * init
     */
    uint8_t ret = obj->init(obj);
    /*
    * unregister if we need to
    */
    if (0 == ret) {
        objectmgr_unregister_object(obj);
    }
    /*
    * done
    */
    return ret;
}

// detach a device (non-fixed devices... like RAM disks and SWAP)
uint8_t objectmgr_detach_object(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    /*
     * unregister
     */
    objectmgr_unregister_object(obj);
    /*
     * uninit
     */
    if (0 != obj->uninit) {
        if (0 != obj->uninit(obj)) {
            // free the device struct
            kfree(obj);

            // good!
            return 1;
        } else {
            return 0;
        }
    } else {
        return 1;
    }
}

/*
* increment device reference count
*/
uint8_t objectmgr_increment_object_refcount(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    //    kprintf("Increasing ref count on %s\n", obj->name);
    obj->reference_count += 1;
    return obj->reference_count;
}
/*
* decrease device reference count
*/
uint8_t objectmgr_decrement_object_refcount(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    //   kprintf("Decreasing ref count on %s\n", obj->name);
    ASSERT(obj->reference_count > 0);
    obj->reference_count -= 1;
    return obj->reference_count;
}

void objectmgr_dump_objects_iterator(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    kprintf("   Name: %s Handle: %llu Type: %#llX Refcount: %llu Description: %s\n", obj->name, obj->handle,
            obj->objectype, obj->reference_count, obj->description);
}

void objectmgr_dump_objects() {
    kprintf("***** All Objects *****\n");
    objectregistry_iterate(&objectmgr_dump_objects_iterator);
}
