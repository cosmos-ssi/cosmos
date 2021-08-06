/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2021 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _ACPI_H
#define _ACPI_H

typedef enum acpi_table_types_t { RSDP, RSDT, XSDT, MADT } acpi_table_types_t;

// These must be in the same order as their respective types are in
// acpi_table_types_t
char* acpi_table_signatures[] = {"RSD PTR ", "RSDT", "XSDT", "MADT"};

void acpi_init();

#endif