//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/tcpip/arp/arpdev.h>
#include <sys/debug/debug.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_arp.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/string/mem.h>
#include <tests/obj/test_arp.h>
#include <types.h>

void test_arp() {
    uint8_t devicename[] = {"arp0"};

    struct object* arp = objectmgr_find_object_by_name(devicename);
    if (0 != arp) {
        struct objectinterface_arp* arp_api = (struct objectinterface_arp*)arp->api;

        struct arp arp_request;
        arp_packet_init(&arp_request, ARP_REQUEST);

        struct arp arp_response;
        memzero((uint8_t*)&arp_response, sizeof(struct arp));

        (*arp_api->request)(arp, &arp_request, &arp_response);

        debug_show_memblock((uint8_t*)&arp_response, sizeof(struct arp));

    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
