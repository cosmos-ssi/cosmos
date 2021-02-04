//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/virtio/vblock/vblock.h>
#include <dev/logical/virtio/virtio.h>
#include <dev/logical/virtio/vnic/vnic.h>

void virtio_devicemgr_register_devices() {
    vblock_devicemgr_register_devices();
    devicemgr_register_vnic_devices();
}

bool virtio_isAligned(uint64_t address, uint32_t alignment) {
    return ((address / alignment) * alignment) == address;
}