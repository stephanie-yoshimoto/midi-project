CC=gcc
CFLAGS=-g -Wall -I.
DEPS = hellomake.h A.h
LOCAL_SRC_FILES = hellomake.c hellofunc.c A.c

OBJS := $(addsuffix .o,$(basename $(LOCAL_SRC_FILES)))
BIN = hellomake.out

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BIN): $(OBJS)
	$(CC) -o $(BIN) $(OBJS)

clean:
	rm -f $(BIN) $(OBJS) *~ core