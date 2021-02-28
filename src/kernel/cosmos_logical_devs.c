//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <cosmos_logical_devs.h>
#include <dev/logical/console/serial_console.h>
#include <dev/logical/console/vga_console.h>
#include <dev/logical/ethernet/ethernet.h>
#include <dev/logical/fs/devfs/devfs.h>
#include <dev/logical/fs/initrd/initrd.h>
#include <dev/logical/fs/vfs/vfs.h>
#include <dev/logical/null/null.h>
#include <dev/logical/ramdisk/ramdisk.h>
#include <dev/logical/rand/rand.h>
#include <dev/logical/tcpip/arp/arpdev.h>
#include <dev/logical/tcpip/icmp/icmpdev.h>
#include <dev/logical/tcpip/ip/ipdev.h>
#include <dev/logical/tcpip/tcp/tcpdev.h>
#include <dev/logical/tcpip/udp/udpdev.h>
#include <dev/logical/tick/tick.h>
#include <sys/objectmgr/objectmgr.h>

#include <sys/fs/fs_facade.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objecttype/objecttype_console.h>
#include <sys/objecttype/objecttype_cpu.h>
#include <sys/objecttype/objecttype_dsp.h>
#include <sys/objecttype/objecttype_pit.h>
#include <sys/objecttype/objecttype_rtc.h>
#include <sys/objecttype/objecttype_serial.h>
#include <sys/objecttype/objecttype_speaker.h>

void attach_logical_devices() {
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
    struct object* rootfs_dev = vfs_attach("/");
    //   struct object* devfs_dev = devfs_attach();
    //   struct filesystem_node* fsnode_devfs = fsfacade_get_fs_rootnode(devfs_dev);
    struct filesystem_node* fsnode_initrd = fsfacade_get_fs_rootnode(initrd_dev);
    // vfs_add_child(rootfs_dev, fsnode_devfs);
    vfs_add_child(rootfs_dev, fsnode_initrd);
}
