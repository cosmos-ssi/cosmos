//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/elf/elf.h>
#include <sys/fs/file_util.h>
#include <sys/fs/fs_facade.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/objectinterface/objectinterface_filesystem.h>
#include <sys/string/string.h>

uint8_t ELF_MAGIC[] = {0x7F, 'E', 'L', 'F'};

uint8_t elf_is_elf_binary(struct elf_binary* elf_binary) {
    ASSERT_NOT_NULL(elf_binary);
    ASSERT(elf_binary->binary);
    ASSERT_NOT_NULL(elf_binary->len);
    ASSERT(elf_binary->len >= sizeof(struct elf_header));
    /*
    * check header
    */
    struct elf_header* header = (struct elf_header*)elf_binary->binary;
    for (uint8_t i = 0; i < 4; i++) {
        if (header->ident[i] != ELF_MAGIC[i]) {
            // it's not ELF
            return 0;
        }
    }

    /*
    * check version (we expect elf v1)
    */
    if (0x01 != header->elf_version) {
        return 0;
    }
    /*
    * check class (we expect 64 bit code)
    */
    if (0x02 != header->class) {
        return 0;
    }
    /*
    * check endianness (we expect little endian)
    */
    if (0x01 != header->endian) {
        return 0;
    }
    return 1;
}

void elf_dump(struct elf_binary* elf_binary) {
    ASSERT_NOT_NULL(elf_binary);
    ASSERT(elf_binary->binary);
    ASSERT_NOT_NULL(elf_binary->len);
    ASSERT(elf_binary->len >= sizeof(struct elf_header));
    struct elf_header* elf = elf_get_elf_header(elf_binary);
    ASSERT_NOT_NULL(elf);

    kprintf("Program headers %llu\n", elf_count_program_headers(elf_binary));
    kprintf("Section headers %llu\n", elf_count_section_headers(elf_binary));
    kprintf("Entry point %#llX\n", elf->entry);
    for (uint32_t i = 0; i < elf_count_section_headers(elf_binary); i++) {
        uint64_t size = elf_get_section_size(elf_binary, i);
        uint8_t* name = elf_get_section_name(elf_binary, i);
        kprintf("section %llu %s size %llu\n", i, name, size);
    }
}

uint64_t elf_get_entry(struct elf_binary* elf_binary) {
    ASSERT_NOT_NULL(elf_binary);
    ASSERT(elf_binary->binary);
    ASSERT_NOT_NULL(elf_binary->len);
    ASSERT(elf_binary->len >= sizeof(struct elf_header));
    struct elf_header* elf = elf_get_elf_header(elf_binary);
    ASSERT_NOT_NULL(elf);
    return elf->entry;
}

/*
* not that complicated...
*/
struct elf_header* elf_get_elf_header(struct elf_binary* elf_binary) {
    ASSERT_NOT_NULL(elf_binary);
    ASSERT(elf_binary->binary);
    ASSERT_NOT_NULL(elf_binary->len);
    ASSERT(elf_binary->len >= sizeof(struct elf_header));
    ASSERT(elf_is_elf_binary(elf_binary));
    return (struct elf_header*)elf_binary->binary;
}

uint16_t elf_count_program_headers(struct elf_binary* elf_binary) {
    ASSERT_NOT_NULL(elf_binary);
    ASSERT(elf_binary->binary);
    ASSERT_NOT_NULL(elf_binary->len);
    ASSERT(elf_binary->len >= sizeof(struct elf_header));
    struct elf_header* elf = elf_get_elf_header(elf_binary);
    ASSERT_NOT_NULL(elf);
    return elf->phnum;
}

uint16_t elf_count_section_headers(struct elf_binary* elf_binary) {
    ASSERT_NOT_NULL(elf_binary);
    ASSERT(elf_binary->binary);
    ASSERT_NOT_NULL(elf_binary->len);
    ASSERT(elf_binary->len >= sizeof(struct elf_header));
    struct elf_header* elf = elf_get_elf_header(elf_binary);
    ASSERT_NOT_NULL(elf);
    return elf->shnum;
}

struct elf_program_header* elf_get_program_header(struct elf_binary* elf_binary, uint16_t idx) {
    ASSERT_NOT_NULL(elf_binary);
    ASSERT(elf_binary->binary);
    ASSERT_NOT_NULL(elf_binary->len);
    ASSERT(elf_binary->len >= sizeof(struct elf_header));
    ASSERT(idx < elf_count_program_headers(elf_binary));
    struct elf_header* elf = elf_get_elf_header(elf_binary);
    ASSERT_NOT_NULL(elf);
    uint16_t program_header_entry_size = elf->phentsize;
    ASSERT_NOT_NULL(program_header_entry_size);
    uint64_t program_header_offset = elf->phoff;
    ASSERT_NOT_NULL(program_header_offset);
    uint64_t addr = (uint64_t)elf_binary->binary;
    addr += program_header_offset + (idx * program_header_entry_size);
    return (struct elf_program_header*)(uint8_t*)addr;
}

struct elf_section_header* elf_get_section_header(struct elf_binary* elf_binary, uint16_t idx) {
    ASSERT_NOT_NULL(elf_binary);
    ASSERT(elf_binary->binary);
    ASSERT_NOT_NULL(elf_binary->len);
    ASSERT(elf_binary->len >= sizeof(struct elf_header));
    ASSERT(idx < elf_count_section_headers(elf_binary));
    struct elf_header* elf = elf_get_elf_header(elf_binary);
    ASSERT_NOT_NULL(elf);
    uint16_t section_header_entry_size = elf->shentsize;
    ASSERT_NOT_NULL(section_header_entry_size);
    uint64_t section_header_offset = elf->shoff;
    ASSERT_NOT_NULL(section_header_offset);
    uint64_t addr = (uint64_t)elf_binary->binary;
    addr += section_header_offset + (idx * section_header_entry_size);
    return (struct elf_section_header*)(uint8_t*)addr;
}

uint64_t elf_get_section_size(struct elf_binary* elf_binary, uint16_t idx) {
    ASSERT_NOT_NULL(elf_binary);
    ASSERT(elf_binary->binary);
    ASSERT_NOT_NULL(elf_binary->len);
    ASSERT(elf_binary->len >= sizeof(struct elf_header));
    ASSERT(idx < elf_count_section_headers(elf_binary));
    struct elf_section_header* section_header = elf_get_section_header(elf_binary, idx);
    ASSERT_NOT_NULL(section_header);
    return section_header->size;
}

uint8_t* elf_get_section(struct elf_binary* elf_binary, uint16_t idx) {
    ASSERT_NOT_NULL(elf_binary);
    ASSERT(elf_binary->binary);
    ASSERT_NOT_NULL(elf_binary->len);
    ASSERT(elf_binary->len >= sizeof(struct elf_header));
    ASSERT(idx < elf_count_section_headers(elf_binary));
    struct elf_section_header* section_header = elf_get_section_header(elf_binary, idx);
    ASSERT_NOT_NULL(section_header);
    uint64_t addr = (uint64_t)elf_binary->binary;
    addr += section_header->offset;
    return (uint8_t*)addr;
}

uint8_t* elf_get_string(struct elf_binary* elf_binary, uint16_t string_idx) {
    ASSERT_NOT_NULL(elf_binary);
    ASSERT(elf_binary->binary);
    ASSERT_NOT_NULL(elf_binary->len);
    ASSERT(elf_binary->len >= sizeof(struct elf_header));
    struct elf_header* elf = elf_get_elf_header(elf_binary);
    ASSERT_NOT_NULL(elf);
    uint16_t string_section_idx = elf->shstrndx;
    uint8_t* string_section = elf_get_section(elf_binary, string_section_idx);
    ASSERT_NOT_NULL(string_section);
    return &(string_section[string_idx]);
}

uint8_t* elf_get_section_name(struct elf_binary* elf_binary, uint16_t idx) {
    ASSERT_NOT_NULL(elf_binary);
    ASSERT(elf_binary->binary);
    ASSERT_NOT_NULL(elf_binary->len);
    ASSERT(elf_binary->len >= sizeof(struct elf_header));
    ASSERT(idx < elf_count_section_headers(elf_binary));
    struct elf_section_header* section_header = elf_get_section_header(elf_binary, idx);
    ASSERT_NOT_NULL(section_header);
    return elf_get_string(elf_binary, section_header->name);
}

uint16_t elf_get_section_by_name(struct elf_binary* elf_binary, uint8_t* section_name) {
    ASSERT_NOT_NULL(elf_binary);
    ASSERT(elf_binary->binary);
    ASSERT_NOT_NULL(elf_binary->len);
    ASSERT(elf_binary->len >= sizeof(struct elf_header));
    ASSERT_NOT_NULL(section_name);
    for (uint32_t i = 0; i < elf_count_section_headers(elf_binary); i++) {
        uint8_t* name = elf_get_section_name(elf_binary, i);
        if (0 == strcmp(name, section_name)) {
            return i;
        }
    }
    return 0;
}

void elf_delete(struct elf_binary* elf_binary) {
    ASSERT_NOT_NULL(elf_binary);
    ASSERT(elf_binary->binary);
    ASSERT_NOT_NULL(elf_binary->len);
    ASSERT(elf_binary->len >= sizeof(struct elf_header));
    kfree(elf_binary->binary);
    kfree(elf_binary);
}

struct elf_binary* elf_new() {
    return (struct elf_binary*)kmalloc(sizeof(struct elf_binary));
}

void elf_parse(struct elf_binary* elf_binary) {

    elf_binary->text_section = elf_get_section_by_name(elf_binary, ELF_SECTION_TEXT);
    ASSERT_NOT_NULL(elf_binary->text_section);
    struct elf_section_header* text_section_header = elf_get_section_header(elf_binary, elf_binary->text_section);
    ASSERT_NOT_NULL(text_section_header);
    kprintf(".text is section %llu at address %#llX\n", elf_binary->text_section, text_section_header->addr);

    elf_binary->text_size = elf_get_section_size(elf_binary, elf_binary->text_section);
    ASSERT_NOT_NULL(elf_binary->text_size);
    kprintf(".text section length %llu\n", elf_binary->text_size);

    elf_binary->entry_point = elf_get_entry(elf_binary);
    ASSERT_NOT_NULL(elf_binary->entry_point);
    kprintf("Entry point %#llX\n", elf_binary->entry_point);

    uint8_t* text_data_aka_the_program = elf_get_section(elf_binary, elf_binary->text_section);
    ASSERT_NOT_NULL(text_data_aka_the_program);
    debug_show_memblock(text_data_aka_the_program, elf_binary->text_size);
}

struct elf_binary* elf_load_file(uint8_t* fs_name, uint8_t* binary_name) {
    ASSERT_NOT_NULL(fs_name);
    ASSERT_NOT_NULL(binary_name);
    struct elf_binary* elf_binary = elf_new();

    elf_binary->len = 0;
    elf_binary->binary = file_util_read_file(fs_name, binary_name, &(elf_binary->len));
    ASSERT_NOT_NULL(elf_binary->len);
    ASSERT_NOT_NULL(elf_binary->binary);

    uint8_t is_elf = elf_is_elf_binary(elf_binary);
    if (1 != is_elf) {
        kprintf("'%s' is not an ELF binary\n", binary_name);
        PANIC("oops!");
    }

    elf_parse(elf_binary);

    return elf_binary;
}

struct elf_binary* elf_load_node(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    struct elf_binary* elf_binary = elf_new();

    elf_binary->len = fsfacade_size(fs_node);
    elf_binary->binary = kmalloc(elf_binary->len);
    fsfacade_read(fs_node, elf_binary->binary, elf_binary->len);

    ASSERT_NOT_NULL(elf_binary->len);
    ASSERT_NOT_NULL(elf_binary->binary);
    uint8_t is_elf = elf_is_elf_binary(elf_binary);
    if (1 != is_elf) {
        kprintf("'%s' is not an ELF binary\n", fs_node->name);
        PANIC("oops!");
    }

    elf_parse(elf_binary);

    return elf_binary;
}
