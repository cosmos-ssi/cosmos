/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020-2022 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <subsystems.h>

SUBSYSTEM_DRIVER_V2(ata, "ATA Controller", "ATA Controller", "Kurt M. Weber", "weberk294@gmail.com", "0.1",
                    0xc81c4c364c81f9d1, 0x281796614fc32b79, 0x4399a93b996b2d0a, 0xb76b57edfb77140a, SUBSYSTEM_ID_BLOCK);