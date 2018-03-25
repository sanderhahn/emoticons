CC=gcc
CFLAGS=-Wall

all: emoticons
emoticons: emoticons.o
emoticons.o: emoticons.c emoticons.h

clean:
	rm -f emoticons emoticons.o
run: emoticons
	./emoticons >emoticons.md
