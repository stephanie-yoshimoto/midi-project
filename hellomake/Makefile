CC=gcc
CFLAGS=-g -Wall -Wextra -ansi -pedantic -I.
DEPS = hellomake.h A.h
OBJS = hellomake.o hellofunc.o A.o
BIN = hellomake.out

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BIN): $(OBJS)
	$(CC) -o $(BIN) $^

clean:
	rm -f $(BIN) $(OBJS) *~ core