/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2021 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/acpi/xsdt.h>
#include <sys/string/string.h>

rsdp_t* find_rsdp_address() {
    /* Returns address of RSDP struct--NOT address of RSDT/XSDT.  Read the RSDP
     * struct to find that.
     */

    // RSDP can be found in one of two areas--either the first KB of the extended BIOS data area

    return 0;
}