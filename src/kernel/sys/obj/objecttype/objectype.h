//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _OBJECTTYPE_H
#define _OBJECTTYPE_H

#include <types.h>

#define OBJECT_TYPE_MAX_NAME 64

#define OBJECT_TYPE_NONE 0x00
#define OBJECT_TYPE_SERIAL 0x01           // serial0 objecttype_serial
#define OBJECT_TYPE_VGA 0x02              // vga0 objecttype_vga
#define OBJECT_TYPE_RTC 0x03              // rtc0 objecttype_rtc
#define OBJECT_TYPE_KEYBOARD 0x04         // keyboard0 objecttype_keyboard
#define OBJECT_TYPE_NIC 0x05              // nic0 objecttype_nic
#define OBJECT_TYPE_BRIDGE 0x06           // bridge0 objecttype_bridge
#define OBJECT_TYPE_USB 0x07              // usb0 objecttype_usb
#define OBJECT_TYPE_ATA 0x08              // ata0
#define OBJECT_TYPE_PIC 0x09              // pic0 objecttype_pic
#define OBJECT_TYPE_MOUSE 0x0A            // mouse0 objecttype_mouse
#define OBJECT_TYPE_FLOPPY 0x0B           // floppy0 objecttype_floppy
#define OBJECT_TYPE_SPEAKER 0x0C          // speaker0 objecttype_speaker
#define OBJECT_TYPE_PIT 0x0D              // pit0 objecttype_pit
#define OBJECT_TYPE_DSP 0x0E              // dsp0 objecttype_dsp
#define OBJECT_TYPE_CMOS 0x0F             // cmos0 objecttype_cmos
#define OBJECT_TYPE_ISADMA 0x10           // isadma0
#define OBJECT_TYPE_CPU 0x11              // cpu0 objecttype_cpu
#define OBJECT_TYPE_RAMDISK 0x12          // rd0 objecttype_block
#define OBJECT_TYPE_VNIC 0x13             // vnic0 objecttype_nic
#define OBJECT_TYPE_VBLOCK 0x14           // vblock0 objecttype_block
#define OBJECT_TYPE_DISK 0x15             // disk0 objecttype_block
#define OBJECT_TYPE_PARALLEL 0x16         // parallel0 objecttype_parallel
#define OBJECT_TYPE_BDA 0x17              // bda0
#define OBJECT_TYPE_SWAP 0x19             // swap0 objecttype_swap
#define OBJECT_TYPE_FILESYSTEM 0x1A       // fs0 objecttype_filesytem
#define OBJECT_TYPE_PARTITION_TABLE 0x1B  // pt0 objecttype_part_table
#define OBJECT_TYPE_CONSOLE 0x1C          // console0 objecttype_console
#define OBJECT_TYPE_PARTITION 0x1D        // partition0 objecttype_block
#define OBJECT_TYPE_NULL0 0x1E            // null0 objecttype_null
#define OBJECT_TYPE_TICK 0x1F             // tick0 objecttype_tick
#define OBJECT_TYPE_RAND 0x20             // rand0 objecttype_rand
#define OBJECT_TYPE_IP 0x21               // ip0 objecttype_ip
#define OBJECT_TYPE_TCP 0x22              // tcp0 objecttype_tcp
#define OBJECT_TYPE_ARP 0x23              // arp0 objecttype_arp
#define OBJECT_TYPE_UDP 0x24              // udp0 objecttype_arp
#define OBJECT_TYPE_ICMP 0x25             // icmp0 objecttype_icmp
#define OBJECT_TYPE_ETHERNET 0x26         // eth0 objecttype_ethernet
#define OBJECT_TYPE_SDHCI 0x27            // sdhci0
#define OBJECT_TYPE_SMBIOS 0x28           // smbios0
#define OBJECT_TYPE_OBJFS 0x29            // objfs0 objecttype_filesytem
#define OBJECT_TYPE_VOH 0x2A              // voh0 objecttype_filesytem
#define OBJECT_TYPE_BGA 0x2B              // bga0 objecttype_bga
#define OBJECT_TYPE_KERNELMAP 0x2C        // kernelmap0 objecttype_kernelmap
#define OBJECT_TYPE_TELNET 0x2D           // telnet0 objecttype_telnet
#define OBJECT_TYPE_USER 0x2E             // user0 objecttype_user
#define OBJECT_TYPE_GROUP 0x2F            // group0 objecttype_group
#define OBJECT_TYPE_SERIALIZER 0x30       // serializer0 objecttype_serializer
#define OBJECT_TYPE_HOSTID 0x31           // hostid0 objecttype_hostid
#define OBJECT_TYPE_TIME 0x32             // time0 objecttype_time

struct object_type {
    uint8_t name[OBJECT_TYPE_MAX_NAME];
    uint16_t id;
};

struct object_type* objecttype_new(uint8_t* name, uint16_t id);

#endif