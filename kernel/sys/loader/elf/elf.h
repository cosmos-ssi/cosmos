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

#define ELF_NIDENT 16

/*
* cosmos struct to represent an elf section
*/
struct elf_section {
    uint8_t name[64];
};
/*
* cosmos struct to represent elf file
*/
struct elf_file {
    uint8_t section_count;
    struct elf_section sections[64];
};

/*
* elf binary header
*/
struct elf_header {
    uint8_t e_ident[ELF_NIDENT];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;  // entry point
    uint64_t e_phoff;  // program header offset
    uint64_t e_shoff;  // section header table offset
    uint32_t e_flags;
    uint16_t e_ehsize;     // size of elf header, always 0x40
    uint16_t e_phentsize;  // program header entry size
    uint16_t e_phnum;      // number program headers
    uint16_t e_shentsize;  // section header entry size
    uint16_t e_shnum;      // number of section headers
    uint16_t e_shstrndx;
};

uint8_t elf_is_elf_binary(uint8_t* binary, uint32_t len);
void elf_dump(uint8_t* binary, uint32_t len);
uint8_t elf_parse(uint8_t* binary, uint32_t len, struct elf_file* elf);
#endif
