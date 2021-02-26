include ../../../mk/flags.mk
include ../../../mk/build.mk

TERM=term
COSMOS_ABI_ARCHIVE=../../lib/libcosmos.a
EXTRA_CFLAGS= -I. -I../../lib/

SRC_DIR=.

all: $(TERM)

term.o: term.hpp term.cpp
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o term.o term.cpp

$(TERM): $(COSMOS_ABI_ARCHIVE) term.o
	$(LD) -m elf_x86_64 -o $(TERM) -L $(COSMOS_ABI_ARCHIVE) term.o   
	
clean:
	$(RM) *.o
	$(RM) $(TERM)
