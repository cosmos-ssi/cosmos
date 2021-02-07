include ../../../scripts/mk/flags.mk
include ../../../scripts/mk/build.mk

EXTRA_CFLAGS= -I.

SRC_DIR=.

all: posix_abi.a

posix_abi.a: posix_abi.o
	$(CC) -shared -o posix_abi.a posix_abi.o

posix_abi.o: posix_abi.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c posix_abi.c
	
clean:
	$(RM) *.o
