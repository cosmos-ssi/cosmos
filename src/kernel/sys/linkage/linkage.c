//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/kprintf/kprintf.h>
#include <sys/linkage/linkage.h>

// defined in cosmos.ld
extern uint64_t _text_start;
extern uint64_t _text_end;
extern uint64_t _data_start;
extern uint64_t _data_end;
extern uint64_t _bss_start;
extern uint64_t _bss_end;
extern uint64_t _debug_start;
extern uint64_t _debug_end;

uint64_t text_start = (uint64_t)&_text_start;
uint64_t text_end = (uint64_t)&text_end;
uint64_t data_start = (uint64_t)&data_start;
uint64_t data_end = (uint64_t)&data_end;
uint64_t bss_start = (uint64_t)&bss_start;
uint64_t bss_end = (uint64_t)&bss_end;
uint64_t debug_start = (uint64_t)&debug_start;
uint64_t debug_end = (uint64_t)&debug_end;

void linkage_show_kernel_section_data() {
    kprintf("_text_start %#llX\n", text_start);
    kprintf("_text_end %#llX\n", text_end);
    kprintf("_data_start %#llX\n", data_start);
    kprintf("_data_end %#llX\n", data_start);
    kprintf("_bss_start %#llX\n", bss_start);
    kprintf("_bss_end %#llX\n", bss_end);
    kprintf("_debug_start %#llX\n", debug_start);
    kprintf("_debug_end %#llX\n", debug_end);
}

uint64_t linkage_get_kernel_size() {
    return bss_end - text_start;
}

uint64_t linkage_get_kernel_sector_count() {
    uint64_t size = bss_end - text_start;
    uint64_t ret = size / 512;
    if (0 != size / 512) {
        ret += 1;
    }
    return ret;
}