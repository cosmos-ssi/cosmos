
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

## Syscalls

The userland API is object-oriented.  As a result, there is a mapping of syscalls to C++ method.  Each C++ userland class is assigned a block of 100 64-bit syscall numbers.


## System Call Numbers
Class        | Method          |#     |
-------------|-----------------|------|
Process      | Sleep           | 1100
Process      | Exit            | 1101
BGA          | getResolution   | 1200
BGA          | setResolution   | 1201
BGA          | getBuffersize   | 1202
BGA          | blt             | 1203
Block        | readSectors     | 1300
Block        | writeSectors    | 1301
Block        | sectorSize      | 1302
Block        | totalSize       | 1303
Console      | setPos          | 1401
Console      | write           | 1402
Filesystem   | getRootNode     | 1500
Filesystem   | read            | 1501
Filesystem   | write           | 1502
Filesystem   | open            | 1503
Filesystem   | close           | 1504
Filesystem   | findNodeById    | 1505
Filesystem   | listDirectory   | 1506
Filesystem   | size            | 1507
Keyboard     | read            | 1601
Mouse        | read            | 1701
NIC          | read            | 1801
NIC          | write           | 1802
Null         | read            | 1901
Rand         | read            | 2001
RTC          | read            | 2101
Tick         | read            | 2201
Serial       | writeln         | 2301




