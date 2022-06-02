CC = gcc
CFLAGS = -std=c11 -g -Wall -pedantic -D_POSIX_SOURCE=1 -pthread
OBJS= master.o shmem_posix.o sem.o


all:  main

main: buffer selector bufferService
	$(CC) $(CFLAGS) -o server main.c

bufferService: bufferService.c
	$(CC) $(CFLAGS) -c bufferService.c

buffer: buffer.c
	$(CC) $(CFLAGS) -c buffer.c

selector: selector.c
	$(CC) $(CFLAGS) -c selector.c

clean:
	rm -f *.o server
