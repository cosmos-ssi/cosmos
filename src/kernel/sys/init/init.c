//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/initrd/initrd.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/elf/elf.h>
#include <sys/fs/file_util.h>
#include <sys/fs/fs_facade.h>
#include <sys/init/init.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/panic/panic.h>
#include <sys/string/mem.h>

uint8_t init_load(uint8_t* fs_name, uint8_t* binary_name) {
    uint8_t ret = 0;
    ASSERT_NOT_NULL(fs_name);
    ASSERT_NOT_NULL(binary_name);
    uint32_t file_len = 0;
    uint8_t* file_data = file_util_read_file(fs_name, binary_name, &file_len);
    ASSERT_NOT_NULL(file_len);
    ASSERT_NOT_NULL(file_data);

    uint8_t is_elf = elf_is_elf_binary(file_data, file_len);
    if (1 != is_elf) {
        kprintf("'%s' is not an ELF binary\n", binary_name);
        PANIC("oops!");
    }

    uint16_t text_section = elf_get_section_by_name(file_data, file_len, ELF_SECTION_TEXT);
    ASSERT_NOT_NULL(text_section);
    kprintf(".text is section %llu\n", text_section);

    uint64_t text_size = elf_get_section_size(file_data, file_len, text_section);
    ASSERT_NOT_NULL(text_size);
    kprintf(".text section length %llu\n", text_size);

    uint8_t* text_data_aka_the_program = elf_get_section(file_data, file_len, text_section);
    ASSERT_NOT_NULL(text_data_aka_the_program);
    debug_show_memblock(text_data_aka_the_program, text_size);

    //    elf_dump(file_data, file_len);

    // exec

    // done with data
    kfree(file_data);
    ret = 1;

    return ret;
}
