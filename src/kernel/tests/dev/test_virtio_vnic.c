//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Jeff Schnurr                           *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/virtio/virtio.h>
#include <dev/logical/virtio/virtqueue.h>
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
    kprintf("test_virtio_vnic starting\n");
    uint8_t devicename[] = {"vnic0"};
    struct device* dev = devicemgr_find_device(devicename);
    if (0 == dev) {
        PANIC("nic0 not found");
    }

    struct deviceapi_nic* nic_api = (struct deviceapi_nic*)dev->api;
    struct vnic_devicedata* device_data = (struct vnic_devicedata*)dev->device_data;

    // status
    uint8_t txq[] = {"TXQ: "};
    uint8_t rxq[] = {"RXQ: "};

    virtq_print(rxq, device_data->receive_queue);
    uint8_t x = 0;
    for (x = 0; x < 20; x++) {
        virtq_print(txq, device_data->send_queue);

        uint8_t s[] = "this is a test of the emergency broadcasting system\0";
        uint8_t* buffer = (uint8_t*)kmalloc(strlen(s));
        strncpy(buffer, s, strlen(s) + 1);
        nic_api->write(dev, buffer, strlen(s));
    }
    virtq_print(rxq, device_data->receive_queue);
}
