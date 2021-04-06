//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/elf/elf.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/string.h>

uint8_t ELF_MAGIC[] = {0x7F, 'E', 'L', 'F'};

uint8_t elf_is_elf_binary(uint8_t* binary, uint32_t len) {
    ASSERT_NOT_NULL(binary);
    ASSERT(0 != len);
    ASSERT(len >= sizeof(struct elf_header));
    /*
    * check header
    */
    struct elf_header* header = (struct elf_header*)binary;
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

void elf_dump(uint8_t* binary, uint32_t len) {
    ASSERT_NOT_NULL(binary);
    ASSERT(0 != len);
    ASSERT(len >= sizeof(struct elf_header));
    struct elf_header* elf = elf_get_elf_header(binary, len);
    ASSERT_NOT_NULL(elf);

    kprintf("Program headers %llu\n", elf_count_program_headers(binary, len));
    kprintf("Section headers %llu\n", elf_count_section_headers(binary, len));
    kprintf("Entry point %#llX\n", elf->entry);
    for (uint32_t i = 0; i < elf_count_section_headers(binary, len); i++) {
        uint64_t size = elf_get_section_size(binary, len, i);
        uint8_t* name = elf_get_section_name(binary, len, i);
        kprintf("section %llu %s size %llu\n", i, name, size);
    }
}

uint64_t elf_get_entry(uint8_t* binary, uint32_t len) {
    ASSERT_NOT_NULL(binary);
    ASSERT_NOT_NULL(len);
    struct elf_header* elf = elf_get_elf_header(binary, len);
    ASSERT_NOT_NULL(elf);
    return elf->entry;
}

/*
* not that complicated...
*/
struct elf_header* elf_get_elf_header(uint8_t* binary, uint32_t len) {
    ASSERT_NOT_NULL(binary);
    ASSERT_NOT_NULL(len);
    ASSERT(len >= sizeof(struct elf_header));
    ASSERT(elf_is_elf_binary(binary, len));
    return (struct elf_header*)binary;
}

uint16_t elf_count_program_headers(uint8_t* binary, uint32_t len) {
    ASSERT_NOT_NULL(binary);
    ASSERT_NOT_NULL(len);
    struct elf_header* elf = elf_get_elf_header(binary, len);
    ASSERT_NOT_NULL(elf);
    return elf->phnum;
}

uint16_t elf_count_section_headers(uint8_t* binary, uint32_t len) {
    ASSERT_NOT_NULL(binary);
    ASSERT_NOT_NULL(len);
    struct elf_header* elf = elf_get_elf_header(binary, len);
    ASSERT_NOT_NULL(elf);
    return elf->shnum;
}

struct elf_program_header* elf_get_program_header(uint8_t* binary, uint32_t len, uint16_t idx) {
    ASSERT_NOT_NULL(binary);
    ASSERT_NOT_NULL(len);
    ASSERT(idx < elf_count_program_headers(binary, len));
    struct elf_header* elf = elf_get_elf_header(binary, len);
    ASSERT_NOT_NULL(elf);
    uint16_t program_header_entry_size = elf->phentsize;
    ASSERT_NOT_NULL(program_header_entry_size);
    uint64_t program_header_offset = elf->phoff;
    ASSERT_NOT_NULL(program_header_offset);
    uint64_t addr = (uint64_t)binary;
    addr += program_header_offset + (idx * program_header_entry_size);
    return (struct elf_program_header*)(uint8_t*)addr;
}

struct elf_section_header* elf_get_section_header(uint8_t* binary, uint32_t len, uint16_t idx) {
    ASSERT_NOT_NULL(binary);
    ASSERT_NOT_NULL(len);
    ASSERT(idx < elf_count_section_headers(binary, len));
    struct elf_header* elf = elf_get_elf_header(binary, len);
    ASSERT_NOT_NULL(elf);
    uint16_t section_header_entry_size = elf->shentsize;
    ASSERT_NOT_NULL(section_header_entry_size);
    uint64_t section_header_offset = elf->shoff;
    ASSERT_NOT_NULL(section_header_offset);
    uint64_t addr = (uint64_t)binary;
    addr += section_header_offset + (idx * section_header_entry_size);
    return (struct elf_section_header*)(uint8_t*)addr;
}

uint64_t elf_get_section_size(uint8_t* binary, uint32_t len, uint16_t idx) {
    ASSERT_NOT_NULL(binary);
    ASSERT_NOT_NULL(len);
    ASSERT(idx < elf_count_section_headers(binary, len));
    struct elf_section_header* section_header = elf_get_section_header(binary, len, idx);
    ASSERT_NOT_NULL(section_header);
    return section_header->size;
}

uint8_t* elf_get_section(uint8_t* binary, uint32_t len, uint16_t idx) {
    ASSERT_NOT_NULL(binary);
    ASSERT_NOT_NULL(len);
    ASSERT(idx < elf_count_section_headers(binary, len));
    struct elf_section_header* section_header = elf_get_section_header(binary, len, idx);
    ASSERT_NOT_NULL(section_header);
    uint64_t addr = (uint64_t)binary;
    addr += section_header->offset;
    return (uint8_t*)addr;
}

uint8_t* elf_get_string(uint8_t* binary, uint32_t len, uint16_t string_idx) {
    ASSERT_NOT_NULL(binary);
    ASSERT_NOT_NULL(len);
    struct elf_header* elf = elf_get_elf_header(binary, len);
    ASSERT_NOT_NULL(elf);
    uint16_t string_section_idx = elf->shstrndx;
    uint8_t* string_section = elf_get_section(binary, len, string_section_idx);
    ASSERT_NOT_NULL(string_section);
    return &(string_section[string_idx]);
}

uint8_t* elf_get_section_name(uint8_t* binary, uint32_t len, uint16_t idx) {
    ASSERT_NOT_NULL(binary);
    ASSERT_NOT_NULL(len);
    ASSERT(idx < elf_count_section_headers(binary, len));
    struct elf_section_header* section_header = elf_get_section_header(binary, len, idx);
    ASSERT_NOT_NULL(section_header);
    return elf_get_string(binary, len, section_header->name);
}

uint16_t elf_get_section_by_name(uint8_t* binary, uint32_t len, uint8_t* section_name) {
    ASSERT_NOT_NULL(binary);
    ASSERT_NOT_NULL(len);
    ASSERT_NOT_NULL(section_name);
    for (uint32_t i = 0; i < elf_count_section_headers(binary, len); i++) {
        uint8_t* name = elf_get_section_name(binary, len, i);
        if (0 == strcmp(name, section_name)) {
            return i;
        }
    }
    return 0;
}
