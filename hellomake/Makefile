CC=gcc
CFLAGS=-I.
DEPS = hellomake.h A.h

%.o: %.c $(DEPS)
	$(CC) -c -g -Wall -o $@ $< $(CFLAGS)

hellomake.out: hellomake.o hellofunc.o A.o
	$(CC) -o hellomake.out hellomake.o hellofunc.o A.o

clean:
	rm -f hellomake.out *.o *~ core