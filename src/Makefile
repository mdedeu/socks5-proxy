CC = gcc
CFLAGS = -std=c11 -g -Wall -pedantic -D_POSIX_SOURCE=1 -pthread
OBJS= master.o shmem_posix.o sem.o


all:  main

main: buffer selector  sock_authentication_parser
	$(CC) $(CFLAGS) -o server main.c

bufferService: bufferService.c
	$(CC) $(CFLAGS) -c bufferService.c

buffer: buffer.c
	$(CC) $(CFLAGS) -c buffer.c

selector: selector.c
	$(CC) $(CFLAGS) -c selector.c

sock_authentication_parser: sock_authentication_parser.c
	$(CC) $(CFLAGS) -c sock_authentication_parser.c




sock_authentication_parser.c:
clean:
	rm -f *.o server
