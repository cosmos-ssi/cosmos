//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/dev.h>
#include <sys/collection/arraylist/arraylist.h>
#include <sys/debug/assert.h>
#include <sys/objectmgr/objectmgr.h>

#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objectmgr/objectregistry.h>
#include <sys/string/string.h>

#define MAX_DEVICE_NAME_LENGTH 128

void objectmgr_init() {
    deviceregistry_init();
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

void objectmgr_register_device(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->description);
    ASSERT_NOT_NULL(dev->devicetype);
    ASSERT_NOT_NULL(dev->init);
    /*
     * set index
     */
    dev->type_index = deviceregistry_devicecount_type(dev->devicetype);
    /*
     * create name
     */
    dev->name = createDeviceName(dev);
    /*
     * register
     */
    deviceregistry_registerdevice(dev);
}

void objectmgr_unregister_device(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    /*
     * unregister
     */
    deviceregistry_unregisterdevice(dev);
}

uint16_t objectmgr_device_count() {
    return deviceregistry_devicecount();
}

void device_initIterator(struct object* dev) {
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
void objectmgr_init_devices() {
    //   kprintf("Initializing Devices\n");
    /*
     * CPU first before first?
     */
    deviceregistry_iterate_type(CPU, device_initIterator);
    /*
     * BDA, EBDA
     */
    deviceregistry_iterate_type(BDA, device_initIterator);
    deviceregistry_iterate_type(ACPI, device_initIterator);
    /*
     * PIC first
     */
    deviceregistry_iterate_type(PIC, device_initIterator);
    /*
     * Serial next
     */
    deviceregistry_iterate_type(SERIAL, device_initIterator);
    /*
     * the PIT
     */
    deviceregistry_iterate_type(PIT, device_initIterator);
    /*
     * CMOS
     */
    deviceregistry_iterate_type(CMOS, device_initIterator);
    /*
     * DMA
     */
    deviceregistry_iterate_type(ISADMA, device_initIterator);
    /*
     * virtual devices
     */
    deviceregistry_iterate_type(VNIC, device_initIterator);
    deviceregistry_iterate_type(VBLOCK, device_initIterator);
    /*
     * everything else
     */
    deviceregistry_iterate_type(RTC, device_initIterator);
    deviceregistry_iterate_type(KEYBOARD, device_initIterator);
    deviceregistry_iterate_type(VGA, device_initIterator);
    deviceregistry_iterate_type(BGA, device_initIterator);
    deviceregistry_iterate_type(USB, device_initIterator);
    deviceregistry_iterate_type(NIC, device_initIterator);
    deviceregistry_iterate_type(BRIDGE, device_initIterator);
    deviceregistry_iterate_type(ATA, device_initIterator);
    deviceregistry_iterate_type(MOUSE, device_initIterator);
    deviceregistry_iterate_type(FLOPPY, device_initIterator);
    deviceregistry_iterate_type(SPEAKER, device_initIterator);
    deviceregistry_iterate_type(DSP, device_initIterator);
    deviceregistry_iterate_type(DISK, device_initIterator);
    deviceregistry_iterate_type(SDHCI, device_initIterator);
    deviceregistry_iterate_type(SMBIOS, device_initIterator);

    //  deviceregistry_iterate_type(RAMDISK, device_initIterator);
    deviceregistry_iterate_type(PARALLEL, device_initIterator);
    //  deviceregistry_iterate_type(SWAP, device_initIterator);
    deviceregistry_iterate_type(KERNELMAP, device_initIterator);
}

struct object* objectmgr_new_device() {
    struct object* ret = (struct object*)kmalloc(sizeof(struct object));
    for (uint16_t i = 0; i < OBJECT_MAX_DESCRIPTION; i++) {
        ret->description[i] = 0;
    }
    ret->init = 0;
    ret->device_data = 0;
    ret->name = 0;
    ret->type_index = 0;
    ret->devicetype = 0;
    ret->api = 0;
    ret->pci = 0;
    ret->reference_count = 0;
    return ret;
}

void objectmgr_set_device_description(struct object* dev, const uint8_t* description) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(description);
    ASSERT(strlen(description) < OBJECT_MAX_DESCRIPTION);
    ASSERT(strlen(description) > 0);

    strncpy(dev->description, description, OBJECT_MAX_DESCRIPTION);

    //    kprintf("%s\n", dev->description);
}

struct object* objectmgr_find_device(const uint8_t* name) {
    ASSERT_NOT_NULL(name);
    return deviceregistry_find_device(name);
}

void objectmgr_find_devices_by_description(device_type dt, const uint8_t* description, deviceSearchCallback cb) {
    ASSERT_NOT_NULL(description);
    ASSERT_NOT_NULL(cb);
    ASSERT_NOT_NULL(dt);
    deviceregistry_find_devices_by_description(dt, description, cb);
}

void objectmgr_find_devices_by_device_type(device_type dt, deviceSearchCallback cb) {
    ASSERT_NOT_NULL(cb);
    ASSERT_NOT_NULL(dt);
    deviceregistry_find_devices_by_devicetype(dt, cb);
}

#ifdef TARGET_PLATFORM_i386
void objectmgr_register_devices() {
    /*
     * scan the PCI bus first
     */
    pci_init();
    /*
     * register up the pic next
     */
    pic_objectmgr_register_devices();
    /*
     * and then RS232
     */
    serial_objectmgr_register_devices();
    /*
     * and the then the PIT
     */
    pit_objectmgr_register_devices();
    /*
     * we need the CMOS
     */
    cmos_objectmgr_register_devices();
    /*
     * ISA DMA Controller
     */
    isadma_objectmgr_register_devices();
    /*
     * rest of this stuff can really happen in any order
     */
    //   swap_objectmgr_register_devices();
    rtc_objectmgr_register_devices();
    keyboard_objectmgr_register_devices();
    //  vga_objectmgr_register_devices();
    bga_objectmgr_register_devices();
    usb_ehci_objectmgr_register_devices();
    network_objectmgr_register_devices();
    bridge_objectmgr_register_devices();
    sdhci_objectmgr_register_devices();
    ata_objectmgr_register_devices();
    mouse_objectmgr_register_devices();
    // floppy_objectmgr_register_devices();
    speaker_objectmgr_register_devices();
    sound_objectmgr_register_devices();
    cpu_objectmgr_register_devices();
    virtio_objectmgr_register_devices();
    //  ramdisk_objectmgr_register_devices();
    pci_ehci_objectmgr_register_devices();
    parallel_objectmgr_register_devices();
    bda_objectmgr_register_devices();
    acpi_objectmgr_register_devices();
    smbios_objectmgr_register_devices();
    kernelmap_objectmgr_register_devices();
}

#else

void objectmgr_register_devices() {
    pl101_objectmgr_register_devices();
}

#endif

// attach a device (non-fixed devices... like RAM disks and SWAP)
uint8_t objectmgr_attach_device(struct object* dev) {
    ASSERT_NOT_NULL(dev);

    /*
     * register
     */
    objectmgr_register_device(dev);
    /*
     * init
     */
    uint8_t ret = dev->init(dev);
    /*
    * unregister if we need to
    */
    if (0 == ret) {
        objectmgr_unregister_device(dev);
    }
    /*
    * done
    */
    return ret;
}

// detach a device (non-fixed devices... like RAM disks and SWAP)
uint8_t objectmgr_detach_device(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    /*
     * unregister
     */
    objectmgr_unregister_device(dev);
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
uint8_t objectmgr_increment_device_refcount(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    //    kprintf("Increasing ref count on %s\n", dev->name);
    dev->reference_count += 1;
    return dev->reference_count;
}
/*
* decrease device reference count
*/
uint8_t objectmgr_decrement_device_refcount(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    //   kprintf("Decreasing ref count on %s\n", dev->name);
    ASSERT(dev->reference_count > 0);
    dev->reference_count -= 1;
    return dev->reference_count;
}

void objectmgr_dump_devices_iterator(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("%s Refcount %llu\n", dev->name, dev->reference_count);
}

void objectmgr_dump_devices() {
    deviceregistry_iterate(&objectmgr_dump_devices_iterator);
}
