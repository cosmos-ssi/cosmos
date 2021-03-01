//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Jeff Schnurr                           *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/virtio/virtqueue.h>
#include <obj/logical/virtio/vnic/vnic.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_nic.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/panic/panic.h>
#include <sys/string/string.h>
#include <tests/obj/test_virtio_vnic.h>
#include <types.h>

void test_virtio_vnic() {
    kprintf("test_virtio_vnic starting\n");
    uint8_t devicename[] = {"vnic0"};
    struct object* obj = objectmgr_find_object_by_name(devicename);
    if (0 == obj) {
        PANIC("nic0 not found");
    }

    struct objectinterface_nic* nic_api = (struct objectinterface_nic*)obj->api;
    struct vnic_objectdata* object_data = (struct vnic_objectdata*)obj->object_data;

    // status
    uint8_t txq[] = {"TXQ: "};
    uint8_t rxq[] = {"RXQ: "};

    virtq_print(rxq, object_data->receive_queue);
    uint8_t x = 0;
    for (x = 0; x < 20; x++) {
        virtq_print(txq, object_data->send_queue);

        uint8_t s[] = "this is a test of the emergency broadcasting system\0";
        uint8_t* buffer = (uint8_t*)kmalloc(strlen(s));
        strncpy(buffer, s, strlen(s) + 1);
        nic_api->write(obj, buffer, strlen(s));
    }
    virtq_print(rxq, object_data->receive_queue);
}
