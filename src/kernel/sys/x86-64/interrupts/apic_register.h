/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _APIC_REGISTER_H
#define _APIC_REGISTER_H

#include <types.h>

/* 
 * This separate file is necessary to avoid circular dependencies between
 * acpi/madt.h and interrupts/apic.h, while still keeping everything in a logical
 * place in source file hierarchy.
 */

typedef uint32_t apic_register_t;

#endif