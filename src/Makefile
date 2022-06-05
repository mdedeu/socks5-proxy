CC = gcc
CFLAGS = -std=gnu11 -g -Wall -pedantic -D_POSIX_SOURCE=1 -pthread -D_GNU_SOURCE
OBJ= client_request_processor.o buffer.o selector.o parsing/sock_authentication_parser.o parsing/sock_hello_parser.o parsing/sock_request_parser.o state/proxy_state_machine.o sock_client.o state/stm.o bufferService.o general_handlers.o parsing/parser.o dns_query.o main.o state/authenticated.o state/closing_connection.o state/connected.o state/hello_sock_received.o state/ready_to_connect.o state/tcp_connected.o state/writing_reply.o

all: $(OBJ)
	$(CC) $(CFLAGS) -o server $(OBJ)

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

# bufferService.o: bufferService.c
# 	$(CC) $(CFLAGS) -c bufferService.c

# buffer.o: buffer.c
# 	$(CC) $(CFLAGS) -c buffer.c

# selector.o: selector.c
# 	$(CC) $(CFLAGS) -c selector.c

# sock_authentication_parser.o: parsing/sock_authentication_parser.c
# 	$(CC) $(CFLAGS) -c sock_authentication_parser.c

# sock_request_parser.o: parsing/sock_request_parser.c
# 	$(CC) $(CFLAGS) -c sock_request_parser.c
	
# sock_hello_parser.o: parsing/sock_hello_parser.c
# 	$(CC) $(CFLAGS) -c sock_hello_parser.c

# proxy_state_machine.o: state/proxy_state_machine.c
# 	$(CC) $(CFLAGS) -c proxy_state_machine.c

# sock_client.o: sock_client.c
# 	$(CC) $(CFLAGS) -c sock_client.c

# general_handlers.o: general_handlers.c
# 	$(CC) $(CFLAGS) -c general_handlers.c

# stm.o: state/stm.c
# 	$(CC) $(CFLAGS) -c stm.c
	
# parser.o: parsing/parser.c
# 	$(CC) $(CFLAGS) -c parser.c

# client_request_processor.o : client_request_processor.c
# 	$(CC) $(CFLAGS) -c client_request_processor.c

# dns_query.o: dns_query.c
# 	$(CC) $(CFLAGS) -c dns_query.c



clean:
	rm -f *.o server
