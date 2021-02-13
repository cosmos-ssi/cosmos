//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _AOUT_H
#define _AOUT_H

#include <types.h>

struct aout_exec_header {
    uint32_t a_midmag;
    uint32_t a_text;
    uint32_t a_data;
    uint32_t a_bss;
    uint32_t a_syms;
    uint32_t a_entry;
    uint32_t a_trsize;
    uint32_t a_drsize;
};

struct aout_relocation_info {

    int32_t r_address;

    uint32_t r_symbolnum : 24, r_pcrel : 1, r_length : 2, r_extern : 1, r_baserel : 1, r_jmptable : 1, r_relative : 1,
        r_copy : 1;
};

#endif
