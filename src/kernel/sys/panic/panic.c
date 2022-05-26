/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/communications/serial/com1.h>
#include <sys/asm/misc.h>
#include <sys/panic/panic.h>
#include <sys/string/string.h>

void panic(const char* s, const char* fn, uint32_t line) {
    uint8_t ll[255];
    uitoa3(line, ll, 255, 10);

    com1_serial_write("Kernel panic: ");
    com1_serial_write(s);
    com1_serial_write(" in ");
    com1_serial_write(fn);
    com1_serial_write(" at line ");
    com1_serial_write(ll);
    com1_serial_write("\n");

    asm_cli();
    asm_hlt();
}
