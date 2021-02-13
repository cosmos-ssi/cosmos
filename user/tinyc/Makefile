CC=gcc
CFLAGS=-I.

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

cosmos_c: tinyc.o
	$(CC) -o cosmos_c tinyc.o

clean:
	rm -f *.o
	rm -f cosmos_c
