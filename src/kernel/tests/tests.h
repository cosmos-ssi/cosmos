//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef __TESTS_H
#define __TESTS_H

#include <tests/fs/test_cfs.h>
#include <tests/fs/test_cpm.h>
#include <tests/fs/test_devfs.h>
#include <tests/fs/test_fat.h>
#include <tests/fs/test_gpt.h>
#include <tests/fs/test_initrd.h>
#include <tests/fs/test_mbr.h>
#include <tests/fs/test_sfs.h>
#include <tests/fs/test_swap.h>
#include <tests/fs/test_tfs.h>
#include <tests/fs/test_vfs.h>
#include <tests/obj/test_acpi.h>
#include <tests/obj/test_arp.h>
#include <tests/obj/test_ata.h>
#include <tests/obj/test_bda.h>
#include <tests/obj/test_cpu.h>
#include <tests/obj/test_display.h>
#include <tests/obj/test_floppy.h>
#include <tests/obj/test_kernelmap.h>
#include <tests/obj/test_null.h>
#include <tests/obj/test_parallel.h>
#include <tests/obj/test_pit.h>
#include <tests/obj/test_ramdisk.h>
#include <tests/obj/test_rand.h>
#include <tests/obj/test_rtc.h>
#include <tests/obj/test_rtl8139.h>
#include <tests/obj/test_sb16.h>
#include <tests/obj/test_serial.h>
#include <tests/obj/test_serial_console.h>
#include <tests/obj/test_smbios.h>
#include <tests/obj/test_speaker.h>
#include <tests/obj/test_vblock.h>
#include <tests/obj/test_vga_console.h>
#include <tests/obj/test_virtio_virtqueue.h>
#include <tests/obj/test_virtio_vnic.h>
#include <tests/sys/test_array.h>
#include <tests/sys/test_arraylist.h>
#include <tests/sys/test_bitmap.h>
#include <tests/sys/test_debug.h>
#include <tests/sys/test_init_loader.h>
#include <tests/sys/test_iobuffers.h>
#include <tests/sys/test_linkedlist.h>
#include <tests/sys/test_malloc.h>
#include <tests/sys/test_mm.h>
#include <tests/sys/test_ntree.h>
#include <tests/sys/test_objects.h>
#include <tests/sys/test_ringbuffer.h>
#include <tests/sys/test_string.h>
#include <tests/sys/test_trap.h>
#include <tests/sys/test_tree.h>

#include <types.h>

/*
 * some tests we always run on startup, just to be sure
 */
void tests_run();

#endif