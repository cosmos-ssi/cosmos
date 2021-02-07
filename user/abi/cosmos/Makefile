include ../../../scripts/mk/flags.mk
include ../../../scripts/mk/build.mk

EXTRA_CFLAGS= -I.

SRC_DIR=.

all: cosmos_abi.a

cosmos_abi.a: cosmos_abi.o cosmos_syscall.o
	$(CC) -shared -o cosmos_abi.a cosmos_abi.o cosmos_syscall.o

cosmos_abi.o: cosmos_abi.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c cosmos_abi.c

cosmos_syscall.o: cosmos_syscall.asm
	$(NASM) $(NASMARGS) cosmos_syscall.asm
	
clean:
	$(RM) *.o
