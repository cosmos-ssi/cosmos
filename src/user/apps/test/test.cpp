
extern "C" {
#include <abi/abi.h>
}

void writestr(uint64_t serial0_device_handle, const uint8_t* str);

int main(int argc, char* argv[]) {
    uint64_t serial0_device_handle = syscall_objectmgr_get_device_by_name("serial0");

    writestr(serial0_device_handle, (const uint8_t*)"Welcome to userland\n");

    syscall_serial_writechar(serial0_device_handle, 'A');
    syscall_serial_writechar(serial0_device_handle, 'B');
    syscall_serial_writechar(serial0_device_handle, 'C');

    //    syscall_serial_writechar(serial0_device_handle);

    //    syscall_console_write("beer2\n");

    //  writestring("beer\n");

    //  syscall_serial_writechar('X');
    syscall_process_exit(0);

    return 0;
}

void writestr(uint64_t serial0_device_handle, const uint8_t* str) {

    uint64_t len = 10;
    for (int i = 0; i < len; i++) {
        syscall_serial_writechar(serial0_device_handle, 'X');
    }

    uint64_t i = 0;
    while (str[i] != 0) {
        uint8_t c = str[i];
        c = c + 1;
        syscall_serial_writechar(serial0_device_handle, c);
        i = i + 1;
    }
    syscall_serial_writechar(serial0_device_handle, 'Q');
}
