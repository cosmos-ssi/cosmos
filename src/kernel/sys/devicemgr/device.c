//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/devicemgr/device.h>

int8_t* device_type_names[] = {"None",   "serial", "vga",   "rtc",    "keyboard", "nic",  "bridge",   "usb",
                               "ata",    "pic",    "mouse", "floppy", "speaker",  "pit",  "dsp",      "cmos",
                               "isadma", "cpu",    "rd",    "vnic",   "vblock",   "disk", "parallel", "bda",
                               "acpi",   "swap",   "fs",    "pt",     "console",  "part", "null",     "tick",
                               "rand",   "ip",     "tcp",   "arp",    "udp",      "icmp", "eth",      "sdhci",
                               "smbios", "devfs",  "vfs",   "bga",    "kernelmap"};
