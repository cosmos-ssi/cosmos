/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2021 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <cosmos_logical_objs.h>
#include <obj/logical/fs/initrd/initrd.h>
#include <sys/asm/misc.h>
#include <sys/debug/assert.h>
#include <sys/fs/fs_facade.h>
#include <sys/gui/gui.h>
#include <sys/init/init.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/iobuffers/iobuffers.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_console.h>
#include <sys/obj/objectinterface/objectinterface_filesystem.h>
#include <sys/obj/objectinterface/objectinterface_telnet.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/objects/objects.h>
#include <sys/proc/proc.h>
#include <sys/sched/sched.h>
#include <sys/sync/sync.h>
#include <sys/x86-64/acpi/acpi.h>
#include <sys/x86-64/acpi/madt.h>
#include <sys/x86-64/idt/idt.h>
#include <sys/x86-64/mm/mm.h>
#include <sys/x86-64/syscall/syscall.h>
#include <tests/tests.h>
#include <types.h>

void dev_tests();
void load_init_binary();
void dump_VOH();
void video_write(const uint8_t* s);
filesystem_node_t* load_test_binary();

void CosmOS() {
    /*
     * Hello!
     */
    kprintf("Loading CosmOS 0.1\n");

    /*
     * switch to new GDT.  causes GPF currently.....
     */
    // gdt_install();

    kprintf("Initializing Spinlocks...\n");
    spinlocks_init();

    kprintf("Initializing MMU...\n");
    mmu_init();

    kprintf("Initializing ACPI...\n");
    acpi_init();

    kprintf("Initializing IDT...\n");
    idt_init();

    kprintf("Initializing Interrupt Routing...\n");
    interrupt_router_init();

    kprintf("Initializing IO buffers...\n");
    iobuffers_init();
    /*
     * init the object manager
     */
    kprintf("Initializing Object Manager...\n");
    objectmgr_init();

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
    objectmgr_register_objects();
    //   kprintf("Registered %llu devices\n", objectmgr_object_count());

    /*
     * Init all devices
     */
    objectmgr_init_objects();
    //  kprintf("There are %llu devices\n", objectmgr_object_count());
    kprintf("\n");
    kprintf("***** Hardware Initialization Complete *****\n");
    kprintf("\n");

    /*
     * mount logical objects
     */
    attach_logical_objects();

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

    // load the init binary.  next step here would be to map it into memory and jump to userland
    kprintf("\n");
    kprintf("***** Loading Userland init from %s *****\n", "disk0");
    kprintf("\n");

    load_init_binary();

    /*gui_init();
    gui_draw();*/

    object_handle_t idle_kernel_work;
    object_handle_t idle_process;
    object_handle_t idle_task;

    idle_kernel_work = object_kernel_work_create(&kernel_idle, NULL);
    idle_process = object_process_create(idle_kernel_work);
    idle_task = object_task_create(idle_process);
    sched_set_state(idle_task, SCHED_LASTRESORT);

    object_handle_t test_exe;
    object_handle_t test_process;
    object_handle_t test_task;

    test_exe = object_executable_create_from_presentation(object_presentation_create(load_test_binary()));
    test_process = object_process_create(test_exe);
    test_task = object_task_create(test_process);

    /*
    * start telnet
    */

    /*struct object* telnet = objectmgr_find_object_by_name("telnet0");
    kprintf("\n");
    kprintf("***** Starting Kernel Telnet *****\n");
    kprintf("\n");
    if (0 != telnet) {
        struct objectinterface_telnet* telnet_api = (struct objectinterface_telnet*)telnet->api;
        (*telnet_api->start)(telnet);
    } else {
        kprintf("Unable to find telnet0\n");
    }*/

    // we never get here currently... well.... eventually the telnet over serial needs to be a on a thread
    sched_switch(task_select());
}

filesystem_node_t* load_test_binary() {
    struct object* voh_dev;
    filesystem_node_t *voh_node, *initrd_node, *file_node;

    voh_dev = objectmgr_find_object_by_name("voh0");
    ASSERT_NOT_NULL(voh_dev);

    voh_node = fsfacade_get_fs_rootnode(voh_dev);
    ASSERT_NOT_NULL(voh_node);

    initrd_node = fsfacade_find_node_by_name(voh_node, "fs2");
    ASSERT_NOT_NULL(initrd_node);

    file_node = fsfacade_find_node_by_name(initrd_node, "test.bin");
    ASSERT_NOT_NULL(file_node);

    return file_node;
}

/*
* load the init binary from the initrd fs
*/
void load_init_binary() {
    uint8_t init_binary_name[] = {"init"};
    init_load(INITRD_DISK, init_binary_name);
    kprintf("Loaded init binary '%s' from disk %s\n", init_binary_name, INITRD_DISK);
}

/*
 * write to vga console which we created earlier (will be console0)
 */
void video_write(const uint8_t* s) {
    struct object* vga_console = objectmgr_find_object_by_name("console0");
    struct objectinterface_console* console0_api = (struct objectinterface_console*)vga_console->api;
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
    // test_virtio_vnic();
}
