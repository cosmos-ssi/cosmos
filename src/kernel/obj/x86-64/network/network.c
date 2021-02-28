// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/x86-64/network/network.h>
#include <obj/x86-64/network/rtl8139/rtl8139.h>

void network_objectmgr_register_objects() {
    rtl8139_objectmgr_register_objects();
    // ne2000pci_objectmgr_register_objects();
    //   ne2000isa_objectmgr_register_objects();
    //  e1000_objectmgr_register_objects();
    //   vnic_objectmgr_register_objects();
}
