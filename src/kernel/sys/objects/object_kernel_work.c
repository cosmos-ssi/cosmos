/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/kmalloc/kmalloc.h>
#include <sys/objects/objects.h>

object_handle_t object_kernel_work_create(void* (*work_func)(void*)) {
    object_kernel_work_t* obj;

    obj = (object_kernel_work_t*)kmalloc(sizeof(object_kernel_work_t));
    ASSERT_NOT_NULL(obj);

    obj->work_func = work_func;

    return object_create(OBJECT_KERNEL_WORK, (void*)obj);
}