include ../scripts/mk/flags.mk
include ../scripts/mk/build.mk

EXTRA_CFLAGS= -I.

SRC_DIR=.

all: abis init.a

abis:
	cd abi/posix && $(MAKE) all
	cd abi/bdos && $(MAKE) all
	cd abi/cosmos && $(MAKE) all

init.a: 
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c init.c
	
clean_abis:
	cd abi/posix && $(MAKE) clean
	cd abi/bdos && $(MAKE) clean
	cd abi/cosmos && $(MAKE) clean

clean: clean_abis
	$(RM) *.o
