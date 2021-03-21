
#include "../../lib/abi/abi.h"

void _start() {
    syscall_console_write("beer\n");
    syscall_console_write("beer2\n");

    syscall_process_exit();
}