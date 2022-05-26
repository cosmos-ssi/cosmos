/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/bus/pci/pci.h>
#include <dev/bus/pci/pci_device.h>
#include <sys/collection/arraylist/arraylist.h>
#include <sys/kprintf/kprintf.h>

void pci_init() {
    kprintf("Initializing PCI bus...\n");

    pci_devices = arraylist_new();

    pci_scan();

    return;
}
