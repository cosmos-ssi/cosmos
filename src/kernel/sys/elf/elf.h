//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _ELF_H
#define _ELF_H

// https://wiki.osdev.org/ELF_Tutorial

#include <types.h>

#define ELF_SECTION_TEXT ".text"
#define ELF_SECTION_BSS ".bss"
#define ELF_SECTION_DATA ".data"

struct elf_binary {
    uint8_t* binary;
    uint32_t len;
    uint64_t entry_point;
    uint16_t text_section;
    uint64_t text_size;
    uint8_t* text;
};

struct elf_section_header {
    uint32_t name;
    uint32_t type;
    uint64_t flags;
    uint64_t addr;
    uint64_t offset;
    uint64_t size;
    uint32_t link;
    uint32_t info;
    uint64_t addralign;
    uint64_t entsize;
};

struct elf_program_header {
    uint32_t type;
    uint32_t flags;
    uint64_t offset;
    uint64_t vaddr;
    uint64_t paddr;
    uint64_t filesz;
    uint64_t memsz;
    uint64_t align;
};

/*
* elf binary header
*/
struct elf_header {
    uint8_t ident[4];
    uint8_t class;
    uint8_t endian;
    uint8_t elf_version;
    uint8_t abi;
    uint8_t abi_version;
    uint8_t pad[7];
    uint16_t type;
    uint16_t machine;
    uint32_t version;
    uint64_t entry;
    uint64_t phoff;
    uint64_t shoff;
    uint32_t flags;
    uint16_t ehsize;
    uint16_t phentsize;
    uint16_t phnum;
    uint16_t shentsize;
    uint16_t shnum;
    uint16_t shstrndx;
};

struct elf_header* elf_get_elf_header(struct elf_binary* elf_binary);
uint8_t elf_is_elf_binary(struct elf_binary* elf_binary);
void elf_dump(struct elf_binary* elf_binary);
uint16_t elf_count_program_headers(struct elf_binary* elf_binary);
uint16_t elf_count_section_headers(struct elf_binary* elf_binary);
struct elf_section_header* elf_get_section_header(struct elf_binary* elf_binary, uint16_t idx);
struct elf_program_header* elf_get_program_header(struct elf_binary* elf_binary, uint16_t idx);
uint8_t* elf_get_section(struct elf_binary* elf_binary, uint16_t idx);
uint64_t elf_get_section_size(struct elf_binary* elf_binary, uint16_t idx);
uint8_t* elf_get_section_name(struct elf_binary* elf_binary, uint16_t idx);
uint16_t elf_get_section_by_name(struct elf_binary* elf_binary, uint8_t* section_name);
uint64_t elf_get_entry(struct elf_binary* elf_binary);
void elf_delete(struct elf_binary* elf_binary);
struct elf_binary* elf_new();
struct elf_binary* elf_load(uint8_t* fs_name, uint8_t* binary_name);

#endif
