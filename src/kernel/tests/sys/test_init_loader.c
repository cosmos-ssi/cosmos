//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/elf/elf.h>
#include <sys/kprintf/kprintf.h>
#include <tests/sys/test_init_loader.h>
#include <types.h>

void test_init_loader() {
    kprintf("Testing Loader\n");

    // boot disk.....
    uint8_t devicename[] = {"disk0"};
    uint8_t init_file_name[] = {"test.elf"};
    struct elf_binary* bin = elf_load_file(devicename, init_file_name);
    kprintf("Loaded '%s' ELF binary\n", init_file_name);
    elf_delete(bin);
}
