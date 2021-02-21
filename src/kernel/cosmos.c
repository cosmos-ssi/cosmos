/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <cosmos_logical_devs.h>
#include <dev/logical/console/vga_console.h>
#include <dev/logical/fs/initrd/initrd.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_console.h>
#include <sys/fs/fs_facade.h>
#include <sys/gui/gui.h>
#include <sys/init/init.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/iobuffers/iobuffers.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objects/objects.h>
#include <sys/proc/proc.h>
#include <sys/sched/sched.h>
#include <sys/sync/sync.h>
#include <sys/syscall/syscall.h>
#include <sys/video/video_util.h>
#include <sys/x86-64/gdt/gdt.h>
#include <sys/x86-64/idt/idt.h>
#include <sys/x86-64/syscall/syscall.h>
#include <tests/tests.h>
#include <types.h>

void dev_tests();
void load_init_binary();
void dump_vfs();
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

    kprintf("Initializing IO buffers...\n");
    iobuffers_init();
    /*
     * init the device registry
     */
    kprintf("Initializing Device Registry...\n");
    devicemgr_init();

    kprintf("Initializing system call handler...\n");
    syscall_init();

    kprintf("Initializing process loader and manager...\n");
    proc_init();

    kprintf("Initializing scheduler...\n");
    sched_init();

    kprintf("Initializing object manager...\n");
    object_init();

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
     * mount logical devices
     */
    attach_logical_devices();

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
    // kprintf("***** Devices *****\n");
    //  devicemgr_dump_devices();

    //   dump_vfs();

    // load the init binary.  next step here would be to map it into memory and jump to userland
    kprintf("\n");
    kprintf("***** Loading Userland init from %s *****\n", "disk0");
    kprintf("\n");

    load_init_binary();

    gui_init();
    gui_draw();

    while (1) {
        asm_hlt();
    }
}

void dump_vfs() {
    kprintf("\n");
    kprintf("***** VFS *****\n");
    kprintf("\n");
    struct device* vfs_dev = devicemgr_find_device("vfs0");
    ASSERT_NOT_NULL(vfs_dev);
    struct filesystem_node* fs_node = fsfacade_get_fs_rootnode(vfs_dev);
    ASSERT_NOT_NULL(fs_node);

    fsfacade_dump(fs_node);
}

/*
* load the init binary from the initrd fs
*/
void load_init_binary() {
    uint8_t init_binary_name[] = {"cosmos_init"};
    init_load(INITRD_DISK, init_binary_name);
    kprintf("Loaded init binary '%s' from disk %s\n", init_binary_name, INITRD_DISK);
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
    // test_virtio_virtqueue();
}
