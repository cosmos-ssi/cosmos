/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/objects/objects.h>
#include <sys/proc/proc.h>
#include <sys/string/mem.h>
#include <sys/sync/sync.h>
#include <sys/x86-64/mm/mm.h>
#include <sys/x86-64/mm/pagetables.h>
#include <types.h>

void proc_adjust_kernel_stack(pttentry cr3);
pttentry proc_obtain_cr3();
void proc_map_image(pttentry cr3, object_handle_t exe_obj);
void proc_map_kernelspace(pttentry cr3);
void proc_map_stack(pttentry cr3);
void* proc_initial_brk(object_handle_t exe_obj);

void proc_adjust_kernel_stack(pttentry cr3) {
    // Remove the page tables for the top eight megabytes in the address space
    // specified by cr3, then replace it with 16 kb of a process kernel stack.

    pttentry* pml4;
    uint8_t i;
    uint64_t stack_page;

    // Clear the last PML4 entry--this is where the stack is, and we don't want
    // to overwrite system page tables.
    pml4 = CONV_PHYS_ADDR(PTT_EXTRACT_BASE(cr3));
    pml4[511] = 0;

    for (i = 0; i < 4; i++) {
        stack_page = slab_allocate(1, PDT_INUSE);
        map_page_at(stack_page, (void*)(DEFAULT_PROC_KERNEL_STACK_START + (i * PAGE_SIZE)), cr3, false);
    }

    return;
}

pid_t proc_create() {
    proc_info_t* proc_info;

    proc_info = (proc_info_t*)kmalloc(sizeof(proc_info_t));
    ASSERT_NOT_NULL(proc_info);
    memset((uint8_t*)proc_info, 0, sizeof(proc_info_t));

    spinlock_acquire(&proc_table_lock);
    proc_info->pid = get_next_pid();

    proc_table_add(proc_info);

    spinlock_release(&proc_table_lock);
    return proc_info->pid;
}

void proc_map_image(pttentry cr3, object_handle_t exe_obj) {
    // Map the process image in the pages specified in the executable object,
    // into the PML4 table specified by cr3.

    object_executable_t* obj;
    uint64_t i;
    void* vaddr = LOAD_BASE_VIRTUAL;

    obj = OBJECT_DATA(exe_obj, object_executable_t);

    for (i = 0; i < obj->page_count; i++) {
        map_page_at(obj->page_base + i, vaddr + (PAGE_SIZE * i), cr3, true);
    }

    return;
}

void proc_map_kernelspace(pttentry cr3) {
    pttentry *sys_pml4, *proc_pml4;
    uint16_t i;

    sys_pml4 = (uint64_t*)CONV_PHYS_ADDR(PTT_EXTRACT_BASE(system_cr3));
    proc_pml4 = (uint64_t*)CONV_PHYS_ADDR(PTT_EXTRACT_BASE(cr3));

    for (i = 256; i < 512; i++) {
        proc_pml4[i] = sys_pml4[i];
    }
    return;
}

void proc_map_stack(pttentry cr3) {
    uint8_t i;
    uint64_t stack_page;

    for (i = 0; i < 4; i++) {
        stack_page = slab_allocate(1, PDT_INUSE);
        map_page_at(stack_page, (void*)(DEFAULT_PROC_USER_STACK_START + (i * PAGE_SIZE)), cr3, true);
    }
    return;
}

pttentry proc_obtain_cr3() {
    // Find a page to put our PML4 table in, and return a value suitable for
    // loading into cr3.

    uint64_t cr3_page;
    pttentry proc_cr3;

    cr3_page = slab_allocate(1, PDT_INUSE);
    ASSERT_NOT_NULL(cr3_page);

    proc_cr3 = cr3_page * PAGE_SIZE;  // we don't need to set any flags
    memset(CONV_PHYS_ADDR(proc_cr3), 0, PAGE_SIZE);

    return proc_cr3;
}

void* proc_initial_brk(object_handle_t exe_obj) {
    void* vaddr = LOAD_BASE_VIRTUAL;

    vaddr = (void*)(OBJECT_DATA(exe_obj, object_executable_t)->page_count * PAGE_SIZE);

    return vaddr;
}

void setup_user_process(pid_t pid, object_handle_t exe_obj) {
    // do ALL the things!

    proc_table_get(pid)->cr3 = proc_obtain_cr3();
    ASSERT_NOT_NULL(proc_table_get(pid)->cr3);

    proc_map_image(proc_table_get(pid)->cr3, exe_obj);

    proc_table_get(pid)->brk = proc_initial_brk(exe_obj);
    ASSERT_NOT_NULL(proc_table_get(pid)->brk);

    proc_map_kernelspace(proc_table_get(pid)->cr3);

    proc_adjust_kernel_stack(proc_table_get(pid)->cr3);

    proc_map_stack(proc_table_get(pid)->cr3);

    // No need to clear registers; struct is cleared on allocation.  So we just set rsp and rbp.
    proc_table_get(pid)->rsp = USER_HALF_MAX_ADDR + 1;
    proc_table_get(pid)->rbp = USER_HALF_MAX_ADDR + 1;

    // and set RFLAGS
    // Bit 2 is reserved must be one, and EI flag is set; all others unset/0
    proc_table_get(pid)->rflags = 0x000000000202;

    proc_table_get(pid)->rip = LOAD_BASE_VIRTUAL;
    return;
}