//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/loader/elf/elf.h>

uint8_t elf_is_elf_binary(uint8_t* binary, uint32_t len) {
    ASSERT_NOT_NULL(binary);
    ASSERT(0 != len);
    return 0;
}
