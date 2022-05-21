/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>

typedef volatile bool generic_spinlock;
typedef volatile generic_spinlock
    kernel_spinlock;  // 64 bytes to take up a full cache line, which improves performance on atomic operations
typedef volatile generic_spinlock module_spinlock;
typedef volatile generic_spinlock function_spinlock;

#define FUNCTION_SPINLOCK_INIT(lock) spinlock_initialize((generic_spinlock*)&lock)
#define FUNCTION_SPINLOCK_ACQUIRE(lock) spinlock_acquire((generic_spinlock*)&lock)
#define FUNCTION_SPINLOCK_RELEASE(lock) spinlock_release((generic_spinlock*)&lock)

#define MODULE_SPINLOCK_INIT(lock) spinlock_module_init((generic_spinlock*)&lock)
#define MODULE_SPINLOCK_ACQUIRE(lock) spinlock_acquire((generic_spinlock*)&lock)
#define MODULE_SPINLOCK_RELEASE(lock) spinlock_release((generic_spinlock*)&lock)

// spinlock.c
extern kernel_spinlock dma_buf_lock;
extern kernel_spinlock dma_list_lock;
extern kernel_spinlock page_dir_lock;
extern kernel_spinlock page_table_lock;
extern kernel_spinlock proc_table_lock;
extern kernel_spinlock task_list_lock;

void spinlocks_init();
void spinlock_acquire(kernel_spinlock* lock);
void spinlock_release(kernel_spinlock* lock);

void spinlock_module_init(module_spinlock* lock);