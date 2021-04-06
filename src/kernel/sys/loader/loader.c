//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/fs/file_util.h>
#include <sys/fs/fs_facade.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/loader/elf.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/panic/panic.h>
#include <sys/string/mem.h>

void loader_load(uint8_t* fs_name, uint8_t* binary_name, struct elf_binary* elf_binary) {
    ASSERT_NOT_NULL(fs_name);
    ASSERT_NOT_NULL(binary_name);
    ASSERT_NOT_NULL(elf_binary);

    elf_binary->len = 0;
    elf_binary->binary = file_util_read_file(fs_name, binary_name, &(elf_binary->len));
    ASSERT_NOT_NULL(elf_binary->len);
    ASSERT_NOT_NULL(elf_binary->binary);

    uint8_t is_elf = elf_is_elf_binary(elf_binary);
    if (1 != is_elf) {
        kprintf("'%s' is not an ELF binary\n", binary_name);
        PANIC("oops!");
    }

    uint16_t text_section = elf_get_section_by_name(elf_binary, ELF_SECTION_TEXT);
    ASSERT_NOT_NULL(text_section);
    struct elf_section_header* text_section_header = elf_get_section_header(elf_binary, text_section);
    ASSERT_NOT_NULL(text_section_header);
    kprintf(".text is section %llu at address %#llX\n", text_section, text_section_header->addr);

    uint64_t text_size = elf_get_section_size(elf_binary, text_section);
    ASSERT_NOT_NULL(text_size);
    kprintf(".text section length %llu\n", text_size);

    uint64_t entry_point = elf_get_entry(elf_binary);
    ASSERT_NOT_NULL(entry_point);
    kprintf("Entry point %#llX\n", entry_point);

    uint8_t* text_data_aka_the_program = elf_get_section(elf_binary, text_section);
    ASSERT_NOT_NULL(text_data_aka_the_program);
    debug_show_memblock(text_data_aka_the_program, text_size);

    //    elf_dump(file_data, file_len);

    // exec
}
