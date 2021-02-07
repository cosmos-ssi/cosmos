/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/logical/console/serial_console.h>
#include <dev/logical/console/vga_console.h>
#include <dev/logical/ethernet/ethernet.h>
#include <dev/logical/fs/initrd/initrd.h>
#include <dev/logical/null/null.h>
#include <dev/logical/ramdisk/ramdisk.h>
#include <dev/logical/rand/rand.h>
#include <dev/logical/tcpip/arp/arpdev.h>
#include <dev/logical/tcpip/icmp/icmpdev.h>
#include <dev/logical/tcpip/ip/ipdev.h>
#include <dev/logical/tcpip/tcp/tcpdev.h>
#include <dev/logical/tcpip/udp/udpdev.h>
#include <dev/logical/tick/tick.h>
#include <sys/asm/asm.h>
#include <sys/deviceapi/deviceapi_console.h>
#include <sys/deviceapi/deviceapi_cpu.h>
#include <sys/deviceapi/deviceapi_dsp.h>
#include <sys/deviceapi/deviceapi_pit.h>
#include <sys/deviceapi/deviceapi_rtc.h>
#include <sys/deviceapi/deviceapi_serial.h>
#include <sys/deviceapi/deviceapi_speaker.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/init/init.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/iobuffers/iobuffers.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/sync/sync.h>
#include <sys/vfs/vfs.h>
#include <sys/x86-64/gdt/gdt.h>
#include <sys/x86-64/idt/idt.h>
#include <sys/x86-64/syscall/syscall.h>
#include <tests/tests.h>
#include <types.h>

void dev_tests();
void mount_ramdisks();
void mount_null();
void mount_tick();
void mount_rand();
void mount_tcpip();
void mount_initrd();
void load_init_binary();

void create_consoles();
void video_write(const uint8_t* s);

void CosmOS() {
    /*
     * Hello!
     */
    kprintf("Loading CosmOS 0.1\n");

    kprintf("Initializing IDT...\n");
    idt_init();

    /*
     * switch to new GDT.  causes GPF currently.....
     */
    // gdt_install();

    kprintf("Initializing Spinlocks...\n");
    spinlocks_init();

    kprintf("Initializing MMU...\n");
    mmu_init();

    kprintf("Initializing Interrupt Routing...\n");
    interrupt_router_init();

    kprintf("Initializing VFS\n");
    vfs_init();

    kprintf("Initializing IO buffers...\n");
    iobuffers_init();
    /*
     * init the device registry
     */
    kprintf("Initializing Device Registry...\n");
    devicemgr_init();

    kprintf("Initializing system call handler...\n");
    syscall_init();

    /*
     * Register all devices
     */
    devicemgr_register_devices();
    //   kprintf("Registered %llu devices\n", devicemgr_device_count());

    /*
     * Init all devices
     */
    devicemgr_init_devices();
    //  kprintf("There are %llu devices\n", devicemgr_device_count());
    kprintf("\n");
    kprintf("***** Hardware Initialization Complete *****\n");
    kprintf("\n");

    /*
     * mount two ram disks.  b/c we can.
     */
    mount_ramdisks();
    mount_null();
    mount_tick();
    mount_rand();
    mount_tcpip();
    mount_initrd();
    /*
     * create consoles
     */
    create_consoles();

    /*
     * say hi on the VGA console
     */
    //  video_write("Welcome to Cosmos");

    kprintf("\n");
    kprintf("***** Kernel Initialization Complete *****\n");
    kprintf("\n");

    /*
     * standard suite of tests to make sure things are as expected
     */
    tests_run();

    kprintf("\n");
    kprintf("***** Sanity Tests Complete *****\n");
    kprintf("\n");

    /*
     * enable interrupts
     */
    kprintf("Enabling Interrupts...\n");
    asm_sti();

    kprintf("\n");
    kprintf("***** Welcome to CosmOS! *****\n");
    kprintf("\n");

    // any dev tests we want to run
    dev_tests();

    // show the vfs
    //  kprintf("***** VFS *****\n");
    //  vfs_dump(cosmos_vfs);
    //  devicemgr_dump_devices();

    // load the init binary.  next step here would be to map it into memory and jump to userland
    kprintf("***** Loading Userland init from %s *****\n", "disk0");
    kprintf("\n");

    load_init_binary();

    while (1) {
        asm_hlt();
    }
}

/*
 * create consoles (devices which implement deviceapi_console). a vga_console on device vga0 and a serial_console on serial0
 */
void create_consoles() {
    // video
    //   struct device* vga = devicemgr_find_device("vga0");
    //   if (0 != vga) {
    // this makes "console0"
    // vga_console_attach(vga);
    //  }
    // serial
    struct device* serial = devicemgr_find_device("serial0");
    if (0 != serial) {
        // this makes "console1"
        serial_console_attach(serial);
    }
}

void mount_ramdisks() {
    const uint16_t sector_size = 512;
    const uint16_t sector_count1 = 1000;
    ramdisk_attach(sector_size, sector_count1);
    const uint16_t sector_count2 = 100;
    ramdisk_attach(sector_size, sector_count2);
}

void mount_null() {
    null_attach();
}

void mount_rand() {
    rand_attach();
}

// mount the init rd
void mount_initrd() {
    uint8_t devicename[] = {INITRD_DISK};

    struct device* dsk = devicemgr_find_device(devicename);
    if (0 != dsk) {
        // attach initrd fs
        struct device* initrd = initrd_attach(dsk, initrd_lba());

        initrd_dump_dir(initrd);

        // detach
        //   initrd_detach(initrd);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}

/*
* load the init binary from the initrd fs
*/
void load_init_binary() {
    uint8_t init_binary_name[] = {"cosmos_init"};
    init_load(INITRD_DISK, init_binary_name);
    kprintf("Loaded init binary '%s' from disk %s\n", init_binary_name, INITRD_DISK);
}

void mount_tick() {
    struct device* pit = devicemgr_find_device("pit0");
    if (0 != pit) {
        tick_attach(pit);
    }
}

/*
* if vnic0 is there, mount IP on it
*/
void mount_tcpip() {
    struct device* vnic = devicemgr_find_device("vnic0");
    if (0 != vnic) {
        struct device* eth = ethernet_attach(vnic);
        arp_attach(eth);
        icmp_attach(eth);
        struct device* ip_dev = ip_attach(eth);
        tcp_attach(ip_dev);
        udp_attach(ip_dev);

        // test arp!
        //        test_arp();
    }
}

/*
 * write to vga console which we created earlier (will be console0)
 */
void video_write(const uint8_t* s) {
    struct device* vga_console = devicemgr_find_device("console0");
    struct deviceapi_console* console0_api = (struct deviceapi_console*)vga_console->api;
    (*console0_api->write)(vga_console, s);
}

void dev_tests() {
    //	floppyread();
    //	test_ata();
    //	test_vblock();
    //	test_ata();
    //	test_ramdisk();
    //  test_cfs();
    //  test_sfs();
    //  test_tfs();
    //  test_fat();
    //	test_list();
    //
    //  test_parallel();
    //    test_rtl8139();
    // test_gpt();
    //   test_ramdisk();
    //  test_swap();
    // test_serial_console_dev();
    // test_vga_console_dev();
    // test_display();
    //  test_mbr();
    // test_trap();
    //  test_sb16();
    // test_gpt();
    // test_initrd();
    //  test_acpi();
}
