/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <obj/x86-64/serial/serial.h>
#include <sys/asm/asm.h>
#include <sys/panic/panic.h>
#include <sys/string/string.h>

void panic(const char* s, const char* fn, uint32_t line) {
    uint8_t ll[255];
    uitoa3(line, ll, 255, 10);

    serial_write_string("Kernel panic: ");
    serial_write_string(s);
    serial_write_string(" in ");
    serial_write_string(fn);
    serial_write_string(" at line ");
    serial_write_string(ll);
    serial_write_string("\n");

    asm_cli();
    asm_hlt();
}
