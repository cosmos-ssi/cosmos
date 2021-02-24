/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

void kernel_idle(void* arg) {
    // We don't actually use the argument, but the kernel_work object requires
    // that the function take one, so it's probably best to just pass it NULL.
    while (1) {
        asm("hlt");
    }
}