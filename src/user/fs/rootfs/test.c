
#include "syscall.h"

void _start() {
    syscall1(SYSCALL_CONSOLE_WRITE, "Beer\n");
    syscall1(SYSCALL_CONSOLE_WRITE, "Beer2\n");

    syscall0(SYSCALL_PROCESS_EXIT);
    //    tge();
}