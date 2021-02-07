//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/fs/initrd/initrd.h>
#include <sys/debug/assert.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/init/init.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/loader/elf/elf.h>

uint8_t init_load(uint8_t* initrd_disk_name, uint8_t* initrd_binary_name) {
    uint8_t ret = 0;
    ASSERT_NOT_NULL(initrd_disk_name);
    ASSERT_NOT_NULL(initrd_binary_name);

    struct device* dsk = devicemgr_find_device(initrd_disk_name);
    if (0 != dsk) {
        struct device* initrd = initrd_attach(dsk, initrd_lba());
        ASSERT_NOT_NULL(initrd);

        uint8_t idx = 0;
        if (1 == initrd_find_file(initrd, initrd_binary_name, &idx)) {
            uint32_t len = initrd_get_file_length(initrd, idx);
            uint8_t* buffer = kmalloc(len);
            initrd_get_file_data(initrd, idx, buffer, len);
            uint8_t is_elf = elf_is_elf_binary(buffer, len);
            if (1 != is_elf) {
                kprintf("'%s' is not an ELF binary\n");
                panic("oops!");
            }
            kfree(buffer);
            ret = 1;
        } else {
            panic("could not find init binary in initrd file system");
        }

        // detach
        initrd_detach(initrd);
    } else {
        kprintf("Unable to find %s\n", initrd_disk_name);
        panic("could not find disk for initrd file system");
    }
    return ret;
}
