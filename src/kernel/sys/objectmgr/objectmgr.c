//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/virtio/virtio.h>
#include <obj/x86-64/acpi/acpi.h>
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
#include <obj/x86-64/pic/pic.h>
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
#include <sys/objectmgr/objectmgr.h>
#include <sys/objectmgr/objectregistry.h>
#include <sys/panic/panic.h>
#include <sys/string/string.h>
#include <types.h>

#define MAX_DEVICE_NAME_LENGTH 128

void objectmgr_init() {
    objectregistry_init();
}

int8_t* createDeviceName(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->devicetype);
    int8_t nn[32];
    int8_t* ret = kmalloc(MAX_DEVICE_NAME_LENGTH);
    ASSERT_NOT_NULL(object_type_names[dev->devicetype]);
    strncpy(ret, object_type_names[dev->devicetype], MAX_DEVICE_NAME_LENGTH);
    uitoa3(dev->type_index, nn, 32, 10);
    ret = strncat(ret, nn, MAX_DEVICE_NAME_LENGTH);
    return ret;
}

void objectmgr_register_object(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->description);
    ASSERT_NOT_NULL(dev->devicetype);
    ASSERT_NOT_NULL(dev->init);
    /*
     * set index
     */
    dev->type_index = objectregistry_devicecount_type(dev->devicetype);
    /*
     * create name
     */
    dev->name = createDeviceName(dev);
    /*
     * register
     */
    objectregistry_registerdevice(dev);
}

void objectmgr_unregister_object(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    /*
     * unregister
     */
    objectregistry_unregisterdevice(dev);
}

uint16_t objectmgr_object_count() {
    return objectregistry_devicecount();
}

void obj_initIterator(struct object* dev) {
    if (0 != dev) {
        if (0 == dev->init(dev)) {
            kprintf("Failed to Initialize %s\n", dev->name);
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
    objectregistry_iterate_type(CPU, obj_initIterator);
    /*
     * BDA, EBDA
     */
    objectregistry_iterate_type(BDA, obj_initIterator);
    objectregistry_iterate_type(ACPI, obj_initIterator);
    /*
     * PIC first
     */
    objectregistry_iterate_type(PIC, obj_initIterator);
    /*
     * Serial next
     */
    objectregistry_iterate_type(SERIAL, obj_initIterator);
    /*
     * the PIT
     */
    objectregistry_iterate_type(PIT, obj_initIterator);
    /*
     * CMOS
     */
    objectregistry_iterate_type(CMOS, obj_initIterator);
    /*
     * DMA
     */
    objectregistry_iterate_type(ISADMA, obj_initIterator);
    /*
     * virtual devices
     */
    objectregistry_iterate_type(VNIC, obj_initIterator);
    objectregistry_iterate_type(VBLOCK, obj_initIterator);
    /*
     * everything else
     */
    objectregistry_iterate_type(RTC, obj_initIterator);
    objectregistry_iterate_type(KEYBOARD, obj_initIterator);
    objectregistry_iterate_type(VGA, obj_initIterator);
    objectregistry_iterate_type(BGA, obj_initIterator);
    objectregistry_iterate_type(USB, obj_initIterator);
    objectregistry_iterate_type(NIC, obj_initIterator);
    objectregistry_iterate_type(BRIDGE, obj_initIterator);
    objectregistry_iterate_type(ATA, obj_initIterator);
    objectregistry_iterate_type(MOUSE, obj_initIterator);
    objectregistry_iterate_type(FLOPPY, obj_initIterator);
    objectregistry_iterate_type(SPEAKER, obj_initIterator);
    objectregistry_iterate_type(DSP, obj_initIterator);
    objectregistry_iterate_type(DISK, obj_initIterator);
    objectregistry_iterate_type(SDHCI, obj_initIterator);
    objectregistry_iterate_type(SMBIOS, obj_initIterator);

    //  objectregistry_iterate_type(RAMDISK, obj_initIterator);
    objectregistry_iterate_type(PARALLEL, obj_initIterator);
    //  objectregistry_iterate_type(SWAP, obj_initIterator);
    objectregistry_iterate_type(KERNELMAP, obj_initIterator);
}

struct object* objectmgr_new_object() {
    struct object* ret = (struct object*)kmalloc(sizeof(struct object));
    for (uint16_t i = 0; i < OBJECT_MAX_DESCRIPTION; i++) {
        ret->description[i] = 0;
    }
    ret->init = 0;
    ret->object_data = 0;
    ret->name = 0;
    ret->type_index = 0;
    ret->devicetype = 0;
    ret->api = 0;
    ret->pci = 0;
    ret->reference_count = 0;
    return ret;
}

void objectmgr_set_object_description(struct object* dev, const uint8_t* description) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(description);
    ASSERT(strlen(description) < OBJECT_MAX_DESCRIPTION);
    ASSERT(strlen(description) > 0);

    strncpy(dev->description, description, OBJECT_MAX_DESCRIPTION);

    //    kprintf("%s\n", dev->description);
}

struct object* objectmgr_find_object(const uint8_t* name) {
    ASSERT_NOT_NULL(name);
    return objectregistry_find_device(name);
}

void objectmgr_find_objects_by_description(device_type dt, const uint8_t* description, deviceSearchCallback cb) {
    ASSERT_NOT_NULL(description);
    ASSERT_NOT_NULL(cb);
    ASSERT_NOT_NULL(dt);
    objectregistry_find_devices_by_description(dt, description, cb);
}

void objectmgr_find_objects_by_device_type(device_type dt, deviceSearchCallback cb) {
    ASSERT_NOT_NULL(cb);
    ASSERT_NOT_NULL(dt);
    objectregistry_find_devices_by_devicetype(dt, cb);
}

#ifdef TARGET_PLATFORM_i386
void objectmgr_register_objects() {
    /*
     * scan the PCI bus first
     */
    pci_init();
    /*
     * register up the pic next
     */
    pic_objectmgr_register_objects();
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
    acpi_objectmgr_register_objects();
    smbios_objectmgr_register_objects();
    kernelmap_objectmgr_register_objects();
}

#else

void objectmgr_register_objects() {
    pl101_objectmgr_register_objects();
}

#endif

// attach a device (non-fixed devices... like RAM disks and SWAP)
uint8_t objectmgr_attach_object(struct object* dev) {
    ASSERT_NOT_NULL(dev);

    /*
     * register
     */
    objectmgr_register_object(dev);
    /*
     * init
     */
    uint8_t ret = dev->init(dev);
    /*
    * unregister if we need to
    */
    if (0 == ret) {
        objectmgr_unregister_object(dev);
    }
    /*
    * done
    */
    return ret;
}

// detach a device (non-fixed devices... like RAM disks and SWAP)
uint8_t objectmgr_detach_object(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    /*
     * unregister
     */
    objectmgr_unregister_object(dev);
    /*
     * uninit
     */
    if (0 != dev->uninit) {
        if (0 != dev->uninit(dev)) {
            // free the device struct
            kfree(dev);

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
uint8_t objectmgr_increment_object_refcount(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    //    kprintf("Increasing ref count on %s\n", dev->name);
    dev->reference_count += 1;
    return dev->reference_count;
}
/*
* decrease device reference count
*/
uint8_t objectmgr_decrement_object_refcount(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    //   kprintf("Decreasing ref count on %s\n", dev->name);
    ASSERT(dev->reference_count > 0);
    dev->reference_count -= 1;
    return dev->reference_count;
}

void objectmgr_dump_objects_iterator(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("%s Refcount %llu\n", dev->name, dev->reference_count);
}

void objectmgr_dump_objects() {
    objectregistry_iterate(&objectmgr_dump_objects_iterator);
}
