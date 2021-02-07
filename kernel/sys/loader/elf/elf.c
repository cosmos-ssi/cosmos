//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kprintf/kprintf.h>
#include <sys/loader/elf/elf.h>

uint8_t ELF_MAGIC[] = {0x7F, 'E', 'L', 'F'};

uint8_t elf_is_elf_binary(uint8_t* binary, uint32_t len) {
    ASSERT_NOT_NULL(binary);
    ASSERT(0 != len);

    /*
    * check header
    */
    struct elf_header* header = (struct elf_header*)binary;
    for (uint8_t i = 0; i < 4; i++) {
        if (header->e_ident[i] != ELF_MAGIC[i]) {
            // it's not ELF
            return 0;
        }
    }
    /*
    * check binary type
    */
    //  kprintf("type %llu\n", header->e_type);
    return 1;
}
