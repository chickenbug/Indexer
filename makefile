CC = gcc
OFLAGS  = -Wall -g -o
CFLAGS  = -g -Wall -Werror -c
COMPILEO = $(CC) $(OFLAGS)
COMPILEC = $(CC) $(CFLAGS)

all: indexer

indexer: indexer.o
	$(COMPILEO) indexer indexer.c
indexer.o: indexer.c indexer.h hash.c hash.h
	$(COMPILEC) indexer.c hash.c

clean:
	rm -f *.o hash indexer