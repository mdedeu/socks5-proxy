CC = gcc
CFLAGS = -std=c11 -g -Wall -pedantic -D_POSIX_SOURCE=1
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