//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Jeff Schnurr                           *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/virtio/virtio.h>
#include <dev/logical/virtio/vnic/vnic.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_nic.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>
#include <tests/dev/test_virtio_vnic.h>

void test_virtio_vnic() {
    uint8_t device_status;

    kprintf("test_virtio_vnic starting\n");
    uint8_t devicename[] = {"vnic0"};
    struct device* dev = devicemgr_find_device(devicename);
    if (0 == dev) {
        panic("nic0 not found");
    }

    struct deviceapi_nic* nic_api = (struct deviceapi_nic*)dev->api;

    uint8_t num = 42;
    nic_api->write(dev, (uint64_t*)&num, 8);

    device_status = (uint8_t)vnic_read_register(VIRTIO_DEVICE_STATUS);
    kprintf("   device status is %hX\n", device_status);
}
