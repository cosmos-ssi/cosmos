
extern "C" {
#include <abi/abi.h>
}

int main(int argc, char* argv[]) {
    syscall_serial_writechar('A');
    syscall_serial_writechar('B');
    syscall_serial_writechar('C');

    //    syscall_console_write("beer2\n");

    //  writestring("beer\n");

    //  syscall_serial_writechar('X');
    syscall_process_exit();

    return 0;
}
