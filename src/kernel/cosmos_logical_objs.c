//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <cosmos_logical_objs.h>
#include <obj/logical/ethernet/ethernet.h>
#include <obj/logical/fs/initrd/initrd.h>
#include <obj/logical/fs/objfs/objfs.h>
#include <obj/logical/fs/voh/voh.h>
#include <obj/logical/group/group.h>
#include <obj/logical/hostid/hostid.h>
#include <obj/logical/null/null.h>
#include <obj/logical/ramdisk/ramdisk.h>
#include <obj/logical/rand/rand.h>
#include <obj/logical/serializer/serializer.h>
#include <obj/logical/tcpip/arp/arpdev.h>
#include <obj/logical/tcpip/icmp/icmpdev.h>
#include <obj/logical/tcpip/ip/ipdev.h>
#include <obj/logical/tcpip/tcp/tcpdev.h>
#include <obj/logical/tcpip/udp/udpdev.h>
#include <obj/logical/telnet/telnet.h>
#include <obj/logical/tick/tick.h>
#include <obj/logical/user/user.h>
#include <sys/fs/fs_facade.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <types.h>

void attach_logical_objects() {
    /*
    * kernel telnet
    * 
    */
    struct object* serial2 = objectmgr_find_object_by_name("serial0");
    struct object* telnet = 0;
    if (0 != serial2) {
        telnet = telnet_attach(serial2);
    }
    /*
    * console
    */
    //   struct object* serial = objectmgr_find_object_by_name("serial0");
    //   if (0 != serial) {
    // this makes "console1"
    //       serial_console_attach(serial);
    //   }
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
    struct object* rtc = objectmgr_find_object_by_name("rtc0");
    if (0 != rtc) {
        rand_attach(rtc);
    } else {
        kprintf("Unable to find %s\n", "rtc0");
    }
    /*
    * serializer
    */
    serializer_attach();
    /*
    * hostid.  how other cosmos nodes know this node
    */
    hostid_attach();
    /*
    * add groups
    */
    group_attach("admin");
    group_attach("users");
    /*
    * add users
    */
    user_attach("kurt");
    user_attach("jeff");
    user_attach("tom");

    /*
    * tick device
    */
    struct object* pit = objectmgr_find_object_by_name("pit0");
    if (0 != pit) {
        tick_attach(pit);
    } else {
        kprintf("Unable to find %s\n", "pit0");
    }
    /*
    * tcp/ip
    */
    struct object* vnic = objectmgr_find_object_by_name("vnic0");
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
    struct object* dsk = objectmgr_find_object_by_name(devicename);
    if (0 != dsk) {
        initrd_dev = initrd_attach(dsk, initrd_lba());
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
    /*
    * fs0
    */
    struct object* fs0_dev = objectmgr_find_object_by_name("fs0");
    if (0 == fs0_dev) {
        kprintf("Unable to find %s\n", "fs0_dev");
    }
    /*
    * voh
    */
    struct object* rootfs_obj = voh_attach("/");
    struct object* objfs_dev = objfs_attach();
    struct filesystem_node* fsnode_objfs = fsfacade_get_fs_rootnode(objfs_dev);
    struct filesystem_node* fsnode_initrd = fsfacade_get_fs_rootnode(initrd_dev);
    struct filesystem_node* fsnode_fs0 = fsfacade_get_fs_rootnode(fs0_dev);

    voh_add_child(rootfs_obj, fsnode_objfs);
    voh_add_child(rootfs_obj, fsnode_initrd);
    voh_add_child(rootfs_obj, fsnode_fs0);
}
