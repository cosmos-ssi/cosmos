//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/fs/initrd/initrd.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/loader/elf/elf.h>
#include <tests/sys/test_init_loader.h>

void test_init_loader() {
    kprintf("Testing Init Loader\n");

    kprintf("Testing initrd\n");

    // boot disk.....
    uint8_t devicename[] = {INITRD_DISK};
    uint8_t init_file_name[] = {"cosmos_init"};

    struct device* dsk = devicemgr_find_device(devicename);
    if (0 != dsk) {

        struct device* initrd = initrd_attach(dsk, initrd_lba());
        ASSERT_NOT_NULL(initrd);

        uint8_t idx = 0;
        if (1 == initrd_find_file(initrd, init_file_name, &idx)) {
            uint32_t len = initrd_get_file_length(initrd, idx);
            uint8_t* buffer = kmalloc(len);
            initrd_get_file_data(initrd, idx, buffer, len);
            kprintf("Loaded '%s'\n", init_file_name);
            uint8_t is_elf = elf_is_elf_binary(buffer, len);
            if (1 == is_elf) {
                kprintf("is ELF\n");
            } else {
                kprintf("not ELF\n");
            }
            kfree(buffer);
        }

        // detach
        initrd_detach(initrd);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
