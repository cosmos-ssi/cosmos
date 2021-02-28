//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _DEV_H
#define _DEV_H

#include <obj/logical/fs/devfs/devfs.h>
#include <obj/logical/fs/swap/swap.h>
#include <obj/logical/ramdisk/ramdisk.h>
#include <obj/logical/virtio/virtio.h>

#ifdef TARGET_PLATFORM_i386

#include <obj/x86-64/acpi/acpi.h>
#include <obj/x86-64/ata/ata_controller.h>
#include <obj/x86-64/bda/bda.h>
#include <obj/x86-64/bga/bga.h>
#include <obj/x86-64/bridge/bridge.h>
#include <obj/x86-64/cmos/cmos.h>
#include <obj/x86-64/cpu/cpu.h>
#include <obj/x86-64/floppy/floppy.h>
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
#include <obj/x86-64/vga/vga.h>

#else

#include <obj/arm/pl101/pl101.h>

#endif

#endif