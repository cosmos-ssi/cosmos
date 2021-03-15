//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/initrd/initrd.h>
#include <sys/debug/assert.h>
#include <sys/fs/file_util.h>
#include <sys/fs/fs_facade.h>
#include <sys/init/init.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/loader/elf/elf.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/panic/panic.h>
#include <sys/string/mem.h>

uint8_t init_load(uint8_t* initrd_disk_name, uint8_t* initrd_binary_name) {
    uint8_t ret = 0;
    ASSERT_NOT_NULL(initrd_disk_name);
    ASSERT_NOT_NULL(initrd_binary_name);
    uint32_t file_len = 0;
    uint8_t* file_data = file_util_read_file(initrd_disk_name, initrd_binary_name, &file_len);
    ASSERT_NOT_NULL(file_len);
    ASSERT_NOT_NULL(file_data);

    uint8_t is_elf = elf_is_elf_binary(file_data, file_len);
    if (1 != is_elf) {
        kprintf("'%s' is not an ELF binary\n", initrd_binary_name);
        PANIC("oops!");
    }

    // exec

    // done with data
    kfree(file_data);
    ret = 1;

    return ret;
}
