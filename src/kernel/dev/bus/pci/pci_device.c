//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/bus/pci/pci_device.h>
#include <sys/collection/arraylist/arraylist.h>
#include <sys/debug/assert.h>
#include <types.h>

struct arraylist* pci_devices;

void pci_objectmgr_search_device(pci_class_codes pci_class, uint8_t pci_subclass, uint16_t vendor_id,
                                 uint16_t device_id, pciobjectSearchCallback cb) {
    ASSERT_NOT_NULL(pci_devices);
    uint16_t i = 0;
    for (i = 0; i < arraylist_count(pci_devices); i++) {
        struct pci_device* dev = (struct pci_device*)arraylist_get(pci_devices, i);
        if ((dev->pci_class == pci_class) && (dev->pci_subclass == pci_subclass)) {
            if ((dev->vendor_id == vendor_id) && (dev->device_id == device_id)) {
                (*cb)(dev);
            }
        }
    }
}

struct arraylist* pci_objectmgr_search_objectype(pci_class_codes pci_class, uint8_t pci_subclass,
                                                 struct arraylist* out) {
    // If out is null, return a newly-allocated arraylist with results;
    // otherwise, add results to out and return it
    uint16_t i = 0;
    struct arraylist* results;

    ASSERT_NOT_NULL(pci_devices);

    if (!out) {
        results = arraylist_new();
        ASSERT_NOT_NULL(results);
    } else {
        results = out;
    }

    for (i = 0; i < arraylist_count(pci_devices); i++) {
        struct pci_device* dev = (struct pci_device*)arraylist_get(pci_devices, i);

        if ((dev->pci_class == pci_class) && (dev->pci_subclass == pci_subclass)) {
            arraylist_add(results, dev);
        }
    }

    return results;
}
