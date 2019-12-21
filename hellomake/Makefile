CC=gcc
CFLAGS=-I.
DEPS = hellomake.h A.h

%.o: %.c $(DEPS)
	$(CC) -c -g -Wall -o $@ $< $(CFLAGS)

hellomake: hellomake.o hellofunc.o A.o
	$(CC) -o hellomake hellomake.o hellofunc.o A.o

clean:
	rm -f hellomake *.o *~ core