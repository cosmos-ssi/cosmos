
extern "C" {
#include <abi/abi.h>
}

void writestr(uint64_t serial0_device_handle, const char* str);

int main(int argc, char* argv[]) {
    uint64_t serial0_device_handle = syscall_objectmgr_get_device_by_name("serial0");

    // writestr(serial0_device_handle, "Welcome to userland\n");

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

void writestr(uint64_t serial0_device_handle, const char* str) {
    uint16_t i = 0;
    while (str[i] != 0) {
        syscall_serial_writechar(serial0_device_handle, str[i]);
        i += 1;
    }
}
