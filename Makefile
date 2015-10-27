all: hash
hash: hash.o
	gcc -g -o hash hash.c
hash.o: hash.c hash.h
	gcc -Wall -Werror -c hash.c
clean:
	rm -f hash hash.o
