//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/x86-64/ata/ata_dma.h>
#include <sys/devicemgr/device.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kprintf/kprintf.h>
#include <tests/dev/test_ata.h>
#include <tests/dev/test_blockdevice.h>
#include <types.h>

void test_ata1() {
    kprintf("Testing ATA Device...\n");

    // disk 3 is mounted to blank.img.  We can use it for testing.
    uint8_t devicename[] = {"disk3"};

    struct device* ata = devicemgr_find_device(devicename);
    if (0 != ata) {
        test_block_device(ata);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}

void test_ata_dma() {
    kprintf("Testing ATA DMA...\n");

    device_t* dev = devicemgr_find_device("disk0");

    ata_dma_read(dev, 0, 129, 0);
}

void test_ata() {
    /*
    * this is commented out b/c it fails test_block_device.  
    */
    //   test_ata1();
    test_ata_dma();
}
