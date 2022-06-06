CC= gcc
CFLAGS= -std=gnu11 -g -Wall -pedantic -D_POSIX_SOURCE=1 -pthread -D_GNU_SOURCE

#Si se agrega un directorio bajo src agregar una variable _DIR al final de estas siguiendo el patron
SRC_DIR= src
STATE_DIR= $(SRC_DIR)/state
PARSE_DIR= $(SRC_DIR)/parsing

EXEC=server

#Si se agrega un directorio para compilar agregar un SRC+= al final de estas siguiendo el patron
SRC= $(wildcard $(SRC_DIR)/*.c)
SRC+= $(wildcard $(PARSE_DIR)/*.c)
SRC+= $(wildcard $(STATE_DIR)/*.c)

OBJ= $(patsubst %.c, %.o, $(SRC))

.PHONY: all
all: $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	find . -name '*.o' -type f -delete
	rm -f server

#SRC:= $(wildcard $(foreach dir,$(DIRS),$(dir))/*.c)
#OBJ= client_request_processor.o buffer.o selector.o $(PARSE_DIR)/sock_authentication_parser.o $(PARSE_DIR)/sock_hello_parser.o $(PARSE_DIR)/sock_request_parser.o $(STATE_DIR)/proxy_state_machine.o sock_client.o $(STATE_DIR)/stm.o bufferService.o general_handlers.o $(PARSE_DIR)/parser.o dns_query.o main.o $(STATE_DIR)/authenticated.o $(STATE_DIR)/closing_connection.o $(STATE_DIR)/connected.o $(STATE_DIR)/hello_sock_received.o $(STATE_DIR)/ready_to_connect.o $(STATE_DIR)/tcp_connected.o $(STATE_DIR)/writing_reply.o
#.PHONY: all
#all: $(OBJ)
#	mv $(OBJ) $(OBJ_DIR)

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


