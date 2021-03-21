include ../../../mk/flags.mk
include ../../../mk/build.mk
include ../user.mk

SRC_DIR=.

all: test.bin

test.o: test.c ../../lib/abi/syscall.h
	$(CC) $(USER_CFLAGS) $(EXTRA_CFLAGS) -c -o test.o test.c

#syscall.o: syscall.asm
#	$(NASM) $(NASMARGS) -o syscall.o syscall.asm

test.elf: ../../lib/abi/syscall.o ../../lib/abi/abi.o test.o
	$(LD) -m elf_x86_64 -o test.elf test.o ../../lib/abi/syscall.o ../../lib/abi/abi.o
	
test.bin: test.elf
	$(OBJCOPY) -O binary test.elf test.bin  

clean:
	$(RM) *.o
	$(RM) *.bin
	$(RM) *.elf
