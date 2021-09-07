/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _SYS_TIMING_H
#define _SYS_TIMING_H

#define TIMER_SOURCE_INIT(name, string_label, initfunc)                                                                \
    void timer_source_init_##name() {                                                                                  \
        return;                                                                                                        \
    }

void timing_init();

#endif