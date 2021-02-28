//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <cosmos_logical_objs.h>
#include <obj/logical/console/serial_console.h>
#include <obj/logical/ethernet/ethernet.h>
#include <obj/logical/fs/initrd/initrd.h>
#include <obj/logical/fs/vfs/vfs.h>
#include <obj/logical/null/null.h>
#include <obj/logical/ramdisk/ramdisk.h>
#include <obj/logical/rand/rand.h>
#include <obj/logical/tcpip/arp/arpdev.h>
#include <obj/logical/tcpip/icmp/icmpdev.h>
#include <obj/logical/tcpip/ip/ipdev.h>
#include <obj/logical/tcpip/tcp/tcpdev.h>
#include <obj/logical/tcpip/udp/udpdev.h>
#include <obj/logical/tick/tick.h>
#include <sys/obj/objectmgr/objectmgr.h>

#include <sys/fs/fs_facade.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/objectinterface/objectinterface_console.h>
#include <sys/obj/objectinterface/objectinterface_cpu.h>
#include <sys/obj/objectinterface/objectinterface_dsp.h>
#include <sys/obj/objectinterface/objectinterface_pit.h>
#include <sys/obj/objectinterface/objectinterface_rtc.h>
#include <sys/obj/objectinterface/objectinterface_serial.h>
#include <sys/obj/objectinterface/objectinterface_speaker.h>

void attach_logical_objects() {
    /*
    * console
    */
    struct object* serial = objectmgr_find_object("serial0");
    if (0 != serial) {
        // this makes "console1"
        serial_console_attach(serial);
    }
    /*
    * ramdisks
    */
    const uint16_t sector_size = 512;
    const uint16_t sector_count1 = 1000;
    ramdisk_attach(sector_size, sector_count1);
    const uint16_t sector_count2 = 100;
    ramdisk_attach(sector_size, sector_count2);
    /*
    * null device
    */
    null_attach();
    /*
    * rand device
    */
    rand_attach();
    /*
    * tick device
    */
    struct object* pit = objectmgr_find_object("pit0");
    if (0 != pit) {
        tick_attach(pit);
    }
    /*
    * tcp/ip
    */
    struct object* vnic = objectmgr_find_object("vnic0");
    if (0 != vnic) {
        struct object* eth = ethernet_attach(vnic);
        arp_attach(eth);
        icmp_attach(eth);
        struct object* ip_dev = ip_attach(eth);
        tcp_attach(ip_dev);
        udp_attach(ip_dev);

        // test arp!
        //        test_arp();
    }
    /*
    * initrd
    */
    uint8_t devicename[] = {INITRD_DISK};
    struct object* initrd_dev = 0;
    struct object* dsk = objectmgr_find_object(devicename);
    if (0 != dsk) {
        initrd_dev = initrd_attach(dsk, initrd_lba());

    } else {
        kprintf("Unable to find %s\n", devicename);
    }
    /*
    * vfs
    */
    struct object* rootfs_obj = vfs_attach("/");
    //   struct object* objfs_dev = devfs_attach();
    //   struct filesystem_node* fsnode_devfs = fsfacade_get_fs_rootnode(devfs_dev);
    struct filesystem_node* fsnode_initrd = fsfacade_get_fs_rootnode(initrd_dev);
    // vfs_add_child(rootfs_obj, fsnode_devfs);
    vfs_add_child(rootfs_obj, fsnode_initrd);
}
