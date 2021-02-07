include ../../../scripts/mk/flags.mk
include ../../../scripts/mk/build.mk

EXTRA_CFLAGS= -I.

SRC_DIR=.

all: bdos_abi.a

bdos_abi.a: bdos_abi.o
	$(CC) -shared -o bdos_abi.a bdos_abi.o

bdos_abi.o: bdos_abi.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c bdos_abi.c
	
clean:
	$(RM) *.o
