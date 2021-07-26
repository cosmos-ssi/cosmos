# System calls

## ABI

* `rax` - system call # (see table below)
* `rbx` - address of first argument

then execute the `syscall` instruction.

Return value will be placed in `rax`.

In addition to `rax` and `rbx`, the `syscall` instruction will store the return
address in `rcx` and the FLAGS register in `r11`.  The system call handler will
nave no way to access the original values in these registers before they are
overwritten by `syscall`, so their values will not be preserved.  Every other
register will be preserved.

Arguments are passed in sequential, 8-byte-aligned memory locations, starting at
the location pointed to by `rbx`.  Arguments smaller than 8 bytes (64 bits) must
still be 8-byte-aligned.  Arguments larger than 8 bytes (e.g. structures,
arrays) must be referenced by a pointer.  Userspace system call wrappers may
accept a structure directly; they will still transform it into a pointer to that
structure "under the hood."

## System Call Numbers
Call mnemonic|#|Handler function and file|Arguments
-------------|-|-------------------------|---------
exit|0|sys/syscall/syscalls.c: syscall_exit()|uint64_t exit_code
print_console|1|sys/syscall/syscalls.c: syscall_print_console()|char *buf
sleep|5|sys/syscall/syscalls.c: syscall_sleep()|uint64_t secs