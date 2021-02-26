include ../../../mk/flags.mk
include ../../../mk/build.mk
include ../user.mk

TERM=term

SRC_DIR=.

all: $(TERM)

term.o: term.hpp term.cpp
	$(CC) $(USER_CFLAGS) $(EXTRA_CFLAGS) -c -o term.o term.cpp

$(TERM): $(COSMOS_ABI_ARCHIVE) term.o
	$(LD) -m elf_x86_64 -o $(TERM) term.o $(COSMOS_ABI_ARCHIVE)  
	
clean:
	$(RM) *.o
	$(RM) $(TERM)
