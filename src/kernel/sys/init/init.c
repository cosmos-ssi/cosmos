//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/fs/initrd/initrd.h>
#include <sys/debug/assert.h>
#include <sys/objectmgr/objectmgr.h>

#include <sys/fs/fs_facade.h>
#include <sys/init/init.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/loader/elf/elf.h>
#include <sys/string/mem.h>

uint8_t init_load(uint8_t* initrd_disk_name, uint8_t* initrd_binary_name) {
    uint8_t ret = 0;
    ASSERT_NOT_NULL(initrd_disk_name);
    ASSERT_NOT_NULL(initrd_binary_name);

    struct object* dsk = objectmgr_find_object(initrd_disk_name);
    if (0 != dsk) {
        struct object* initrd = initrd_attach(dsk, initrd_lba());
        ASSERT_NOT_NULL(initrd);

        // get root node
        struct filesystem_node* filesystem_root_node = fsfacade_get_fs_rootnode(initrd);

        // get the initrd_binary
        struct filesystem_node* initrd_node = fsfacade_find_node_by_name(filesystem_root_node, initrd_binary_name);
        if (0 != initrd_node) {
            uint32_t len = fsfacade_size(initrd_node);
            uint8_t* buffer = kmalloc(len);
            fsfacade_read(initrd_node, buffer, len);

            uint8_t is_elf = elf_is_elf_binary(buffer, len);
            if (1 != is_elf) {
                kprintf("'%s' is not an ELF binary\n", initrd_binary_name);
                PANIC("oops!");
            }
            /*
            * parse
            */
            struct elf_file elf;
            memzero((uint8_t*)&elf, sizeof(struct elf_file));
            elf_parse(buffer, len, &elf);

            /*
            * done
            */
            kfree(buffer);
            ret = 1;
        } else {
            PANIC("Could not find initrd binary on filesystem");
        }

        // detach
        initrd_detach(initrd);
    } else {
        kprintf("Unable to find %s\n", initrd_disk_name);
        PANIC("could not find disk for initrd file system");
    }
    return ret;
}
