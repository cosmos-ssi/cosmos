CC=gcc
CFLAGS=-I.

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mkinitrd: mkinitrd.o 
	$(CC) -o mkinitrd mkinitrd.o

clean:
	rm -f *.o
	rm -f mkinitrd
	rm -f *.img