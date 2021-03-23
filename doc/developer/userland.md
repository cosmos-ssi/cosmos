
#Cosmos Userland

Cosmos provides C++ userland. There are C++ wrapper objects for all devices as well as fundamental objects such as Process.

## Programming in Userland

To use the userland runtime, simply `#include <cosmos.hpp>` and link the static library `libcosmos.a`.

The Cosmos userland runtime includes:

* `string.hpp`. A simple C++ string.
* `process.hpp`. A wrapper class for processes, including the current process
* `heap.hpp`. A simpler userland heap
* `malloc.h`.
* `device.hpp`. The base class for all devices.

libcosmos.a also includes wrapper classes for device types, including

* NIC
* BGA
* Filesystem
* mouse
* keyboard
* null
* rtc
* tick
* rand
* serial
* block
* console

## How userland starts

Userland start code is in /src/user/lib/start.c.  The symbol _start is the entry point for userland programs.  _start calls the userland main(argc, argv) and also calls `syscall_process_exit`.

## Syscalls

The userland API is object-oriented.  As a result, there is a mapping of syscalls to C++ method.  Each C++ userland class is assigned a block of 100 64-bit syscall numbers.

Syscall numbers are defined in userland.md, syscall.h (userland) and syscall.h (kernel)

## System Call Numbers
Class        | Method          | syscall #  | Kernel API   
-------------|-----------------|------------|-------------------|
Process      | Sleep           | 1100       | deviceapi_process
Process      | Exit            | 1101       |
BGA          | getResolution   | 1200       | deviceapi_bga
BGA          | setResolution   | 1201       |
BGA          | getBuffersize   | 1202       |
BGA          | blt             | 1203       |
Block        | readSectors     | 1300       | deviceapi_block
Block        | writeSectors    | 1301       |
Block        | sectorSize      | 1302       |
Block        | totalSize       | 1303       |
Console      | setPos          | 1400       | deviceapi_console
Console      | write           | 1401       |
Filesystem   | getRootNode     | 1500       | deviceapi_filesystem
Filesystem   | read            | 1501       |
Filesystem   | write           | 1502       |
Filesystem   | open            | 1503       |
Filesystem   | close           | 1504       |
Filesystem   | findNodeById    | 1505       |
Filesystem   | listDirectory   | 1506       |
Filesystem   | size            | 1507       |
Keyboard     | read            | 1600       | deviceapi_keyboard
Mouse        | read            | 1700       | deviceapi_mouse
NIC          | read            | 1800       | deviceapi_nic
NIC          | write           | 1801       |
Null         | read            | 1900       | deviceapi_null
Rand         | read            | 2000       | deviceapi_rand
RTC          | read            | 2100       | deviceapi_rtc
Tick         | read            | 2200       | deviceapi_tick
Serial       | readchar        | 2300       | deviceapi_serial
Serial       | writechar       | 2301       | deviceapi_serial
Memory		 | malloc		   | 2400		| to-be deviceapi_memory
Memory		 | free		       | 2401		| to-be deviceapi_memory
Memory		 | realloc		   | 2402		| to-be deviceapi_memory
User		 | get_name		   | 2500		| deviceapi_user
User		 | set_name		   | 2501		| 
User		 | set_pwd		   | 2502		| 
User		 | validate_pwd    | 2503		| 
Group        | get_name        | 2600       |
HostID       | get_id          | 2700       | deviceapi_hostid
ObjectMgr    | find_name       | 2800       |
ObjectMgr    | find_handle     | 2801       |



