/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/x86-64/pci/pci.h>
#include <sys/kprintf/kprintf.h>
#include <dev/x86-64/pci/pci_device.h>
#include <sys/collection/arraylist/arraylist.h>

void pci_init() {
    kprintf("Initializing PCI bus...\n");

    pci_devices = arraylist_new();

    pci_scan();

    return;
}
