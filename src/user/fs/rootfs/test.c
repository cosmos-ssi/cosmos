
#include "syscall.h"

void _start() {
    syscall1(1401, "Beer\n");
    syscall1(1401, "Beer2\n");

    syscall0(1101);
    //    tge();
}