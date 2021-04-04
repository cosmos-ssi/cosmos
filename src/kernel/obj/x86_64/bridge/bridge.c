//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/x86_64/bridge/bridge.h>
#include <obj/x86_64/bridge/intel82371ab/intel82371ab.h>
#include <obj/x86_64/bridge/inteli440fx/inteli440fx.h>
#include <obj/x86_64/bridge/intelpciisa/intelpciisa.h>

/**
 * find all bridge devices and register them
 */
void bridge_objectmgr_register_objects() {
    i440fx_bridge_register();
    i982371_bridge_register();
    pciisa_bridge_register();
}
