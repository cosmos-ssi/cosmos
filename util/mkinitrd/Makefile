CC=gcc
CFLAGS=-I.
DEPS = hellomake.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mkinitrd: mkinitrd.o 
	$(CC) -o mkinitrd mkinitrd.o