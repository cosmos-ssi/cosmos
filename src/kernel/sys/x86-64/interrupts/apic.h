/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _APIC_H
#define _APIC_H

// APIC registers are 32-bits wide, but they're spaced 16 bytes (128 bits)
// apart.  This will take care of calculating the proper offset for us.
#define APIC_REGISTER(x) (apic_register_base[(x)*4])

typedef uint32_t apic_register_t;

void apic_init();

#endif