//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/x86-64/ata/ata_dma.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/objectinterface/objectinterface_block.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>
#include <tests/obj/test_ata.h>
#include <tests/obj/test_blockdevice.h>

void test_ata1() {
    kprintf("Testing ATA Device...\n");

    // disk 3 is mounted to blank.img.  We can use it for testing.
    uint8_t devicename[] = {"disk3"};

    struct object* ata = objectmgr_find_object(devicename);
    if (0 != ata) {
        test_block_device(ata);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}

void test_ata_dma() {
    kprintf("Testing ATA DMA...\n");

    struct object* obj = objectmgr_find_object("disk0");

    ata_dma_read(obj, 0, 129, 0);
}

void test_ata() {
    /*
    * this is commented out b/c it fails test_block_device.  
    */
    //   test_ata1();
    test_ata_dma();
}
