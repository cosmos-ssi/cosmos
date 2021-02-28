//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _OBJECT_H
#define _OBJECT_H
#include <types.h>

#define OBJECT_MAX_DESCRIPTION 64

// forward declare these
struct pci_device;
struct object;

// return 1 if successful, 0 if failed to unit
typedef uint8_t (*obj_init)(struct object* obj);
// return 1 is successful, 0 if failed to uninit, including if device refcount>0
typedef uint8_t (*obj_uninit)(struct object* obj);

typedef enum object_type {
    NONE = 0x00,
    SERIAL = 0x01,           // serial0, objecttype_serial
    VGA = 0x02,              // vga0, objecttype_vga
    RTC = 0x03,              // rtc0, objecttype_rtc
    KEYBOARD = 0x04,         // keyboard0, objecttype_keyboard
    NIC = 0x05,              // nic0, objecttype_nic
    BRIDGE = 0x06,           // bridge0, objecttype_bridge
    USB = 0x07,              // usb0, objecttype_usb
    ATA = 0x08,              // ata0
    PIC = 0x09,              // pic0, objecttype_pic
    MOUSE = 0x0A,            // mouse0, objecttype_mouse
    FLOPPY = 0x0B,           // floppy0, objecttype_floppy
    SPEAKER = 0x0C,          // speaker0, objecttype_speaker
    PIT = 0x0D,              // pit0, objecttype_pit
    DSP = 0x0E,              // dsp0, objecttype_dsp
    CMOS = 0x0F,             // cmos0, objecttype_cmos
    ISADMA = 0x10,           // isadma0
    CPU = 0x11,              // cpu0, objecttype_cpu
    RAMDISK = 0x12,          // rd0, objecttype_block
    VNIC = 0x13,             // vnic0, objecttype_nic
    VBLOCK = 0x14,           // vblock0, objecttype_block
    DISK = 0x15,             // disk0, objecttype_block
    PARALLEL = 0x16,         // parallel0, objecttype_parallel
    BDA = 0x17,              // bda0
    ACPI = 0x18,             // acpi0
    SWAP = 0x19,             // swap0, objecttype_swap
    FILESYSTEM = 0x1A,       // fs0, objecttype_filesytem
    PARTITION_TABLE = 0x1B,  // pt0, objecttype_part_table
    CONSOLE = 0x1C,          // console0, objecttype_console
    PARTITION = 0x1D,        // partition0, objecttype_block
    NULL0 = 0x1E,            // null0, objecttype_null
    TICK = 0x1F,             // tick0, objecttype_tick
    RAND = 0x20,             // rand0, objecttype_rand
    IP = 0x21,               // ip0, objecttype_ip
    TCP = 0x22,              // tcp0, objecttype_tcp
    ARP = 0x23,              // arp0, objecttype_arp
    UDP = 0x24,              // udp0, objecttype_udp
    ICMP = 0x25,             // icmp0, objecttype_icmp
    ETHERNET = 0x26,         // eth0, objecttype_ethernet
    SDHCI = 0x27,            // sdhci0
    SMBIOS = 0x28,           // smbios0
    DEVFS = 0x29,            // devfs0, objecttype_filesytem
    VFS = 0x2A,              // vfs0, objecttype_filesytem
    BGA = 0x2B,              // bga0, objecttype_bga
    KERNELMAP = 0x2C         // kernelmap0, objecttype_kernelmap
} device_type;

/*
 * array of names, indexed by device_type
 */
extern int8_t* object_type_names[];

struct object {
    /*
     * the combination of name (from object_type_names) and index
     * create the device name.  ie "serial0".
     */
    int8_t* name;
    uint8_t type_index;
    /*
     * the type (SERIAL, VGA etc)
     */
    enum object_type devicetype;
    /*
     * init function
     */
    obj_init init;
    /*
     * un-init function
     */
    obj_uninit uninit;
    /*
     * human readable description provided by the driver
     */
    uint8_t description[OBJECT_MAX_DESCRIPTION];
    /*
     * object-specific data
     */
    void* object_data;
    /*
     * For PCI devices, this is a struct pci_device*.
     */
    struct pci_device* pci;
    /*
     * pointer to the type-specific API struct
     */
    void* api;
    /*
    * reference count.  incremented when a device attaches to this device
    * and decremented when a device detaches from this device
    */
    uint8_t reference_count;
};

#endif