/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2021 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _ACPI_H
#define _ACPI_H

typedef enum acpi_table_types_t { ACPI_RSDP, ACPI_RSDT, ACPI_XSDT, ACPI_MADT } acpi_table_types_t;

// These must be in the same order as their respective types are in
// acpi_table_types_t
extern const char* acpi_table_signatures[];

void* acpi_find_table(acpi_table_types_t type);
void acpi_init();

#endif