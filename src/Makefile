CC = gcc
CFLAGS = -std=c99 -g -Wall -pedantic
OBJS= master.o shmem_posix.o sem.o


all:  main

main: buffer.o selector.o
	$(CC) $(CFLAGS) -o server main.c


buffer.o:buffer.c
	$(CC) $(CFLAGS) -c buffer.c

selector.o: selector.c
	$(CC) $(CFLAGS) -c selector.c

clean:
	rm -f *.o view main selector.o buffer.o