
#include "../../lib/abi/abi.h"

void _start() {
    syscall_serial_writechar('A');
    syscall_serial_writechar('B');
    syscall_serial_writechar('C');

    //    syscall_console_write("beer2\n");

    //  writestring("beer\n");

    //  syscall_serial_writechar('X');
    syscall_process_exit();
}

void writestring(uint8_t* str) {
    syscall_serial_writechar('L');
    uint16_t i = 0;
    uint8_t c = str[i];
    while (c != 0) {
        syscall_serial_writechar(c);
        i += 1;
        c = str[i];
    }
}
