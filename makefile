CC=g++
CFLAGS=-I.
DEPS = header.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

compile: socket.o main.o 
	$(CC) -o main socket.o main.o