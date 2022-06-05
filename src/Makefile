CC = gcc
CFLAGS = -std=gnu11 -g -Wall -pedantic -D_POSIX_SOURCE=1 -pthread -D_GNU_SOURCE -fsanitize=address
OBJ=client_request_processor.o buffer.o selector.o sock_authentication_parser.o sock_hello_parser.o sock_request_parser.o proxy_state_machine.o sock_client.o stm.o bufferService.o general_handlers.o parser.o dns_query.o
all: clean main

main: dns_query.o client_request_processor.o buffer.o selector.o sock_authentication_parser.o sock_hello_parser.o sock_request_parser.o proxy_state_machine.o sock_client.o stm.o bufferService.o general_handlers.o parser.o
	$(CC) $(CFLAGS) -o server main.c $(OBJ)

bufferService.o: bufferService.c
	$(CC) $(CFLAGS) -c bufferService.c

buffer.o: buffer.c
	$(CC) $(CFLAGS) -c buffer.c

selector.o: selector.c
	$(CC) $(CFLAGS) -c selector.c

sock_authentication_parser.o: sock_authentication_parser.c
	$(CC) $(CFLAGS) -c sock_authentication_parser.c

sock_request_parser.o: sock_request_parser.c
	$(CC) $(CFLAGS) -c sock_request_parser.c
	
sock_hello_parser.o: sock_hello_parser.c
	$(CC) $(CFLAGS) -c sock_hello_parser.c

proxy_state_machine.o: proxy_state_machine.c
	$(CC) $(CFLAGS) -c proxy_state_machine.c

sock_client.o: sock_client.c
	$(CC) $(CFLAGS) -c sock_client.c

general_handlers.o: general_handlers.c
	$(CC) $(CFLAGS) -c general_handlers.c

stm.o: stm.c
	$(CC) $(CFLAGS) -c stm.c
	
parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c

client_request_processor.o : client_request_processor.c
	$(CC) $(CFLAGS) -c client_request_processor.c

dns_query.o: dns_query.c
	$(CC) $(CFLAGS) -c dns_query.c



clean:
	rm -f *.o server
