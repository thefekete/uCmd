P=tests
OBJECTS=../ucmd.o
CFLAGS=-Wall -Werror `pkg-config --cflags glib-2.0` -I.. -D_GNU_SOURCE
LDLIBS=`pkg-config --libs glib-2.0`
CC=gcc -std=c99


$(P): $(OBJECTS)

test: $(P)
	gtester $(P)

run: $(P)
	./$(P)

clean:
	rm -f $(OBJECTS) $(P)

.PHONY: run clean
