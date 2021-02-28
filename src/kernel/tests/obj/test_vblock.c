//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/kprintf/kprintf.h>
#include <sys/objectinterface/objectinterface_block.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>
#include <tests/obj/test_blockdevice.h>
#include <tests/obj/test_vblock.h>
#include <types.h>

void test_vblock() {
    // get virtual block device
    uint8_t devicename[] = {"vblock0"};

    struct object* vblock = objectmgr_find_object(devicename);
    if (0 != vblock) {
        test_block_device(vblock);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
