CC = gcc
CFLAGS = -std=c11 -g -Wall -pedantic -D_POSIX_SOURCE=1 -pthread
OBJS= master.o shmem_posix.o sem.o


all:  main

main: buffer selector  sock_authentication_parser proxy_state_machine sock_hello_parser sock_client stm
	$(CC) $(CFLAGS) -o server main.c

bufferService: bufferService.c
	$(CC) $(CFLAGS) -c bufferService.c

buffer: buffer.c
	$(CC) $(CFLAGS) -c buffer.c

selector: selector.c
	$(CC) $(CFLAGS) -c selector.c

sock_authentication_parser: sock_authentication_parser.c
	$(CC) $(CFLAGS) -c sock_authentication_parser.c

proxy_state_machine: proxy_state_machine.c 
	$(CC) $(CFLAGS) -c proxy_state_machine.c

sock_hello_parser: sock_hello_parser.c
	$(CC) $(CFLAGS) -c sock_hello_parser.c

sock_client: sock_client.c
	$(CC) $(CFLAGS) -c sock_client.c

stm:stm.c
	$(CC) $(CFLAGS) -c stm.c










sock_authentication_parser.c:
clean:
	rm -f *.o server
