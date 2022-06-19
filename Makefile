CC= gcc #TODO: deshardcodear el compilador
CFLAGS= --std=c11  -pedantic -pedantic-errors -Wall -Wextra -Werror  -Wno-unused-parameter -Wno-implicit-fallthrough -D_POSIX_C_SOURCE=200112L -g
LDFLAGS= -pthread

#Si se agrega un directorio bajo src agregar una variable _DIR al final de estas siguiendo el patron
SRC_DIR= src
STATE_DIR= $(SRC_DIR)/state
COOL_STATES_DIR= $(STATE_DIR)/cool_states
PARSE_DIR= $(SRC_DIR)/parsing
COOL_PARSE_DIR= $(PARSE_DIR)/cool_client_parsing
COOL_CLIENT_DIR= $(SRC_DIR)/cool_client
SERVER_DIR= $(SRC_DIR)/main

SERVER_EXEC= socks5d
COOL_EXEC= client

#Si se agrega un directorio para compilar agregar un SRC+= al final de estas siguiendo el patron
SRC= $(wildcard $(SRC_DIR)/*.c)
SRC+= $(wildcard $(PARSE_DIR)/*.c)
SRC+= $(wildcard $(STATE_DIR)/*.c)
SRC+= $(wildcard $(COOL_STATES_DIR)/*.c)
SRC+= $(wildcard $(COOL_PARSE_DIR)/*.c)
COOL_SRC= $(wildcard $(COOL_CLIENT_DIR)/*.c)
SERVER_SRC= $(wildcard $(SERVER_DIR)/*.c)


OBJ= $(patsubst %.c, %.o, $(SRC))
SERVER_OBJ= $(patsubst %.c, %.o, $(SERVER_SRC))
COOL_OBJ= $(patsubst %.c, %.o, $(COOL_SRC))

.PHONY: all
all: server cool

.PHONY: cool
cool: $(OBJ) $(COOL_OBJ)
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(COOL_EXEC) $(OBJ) $(COOL_OBJ)

.PHONY: server
server: $(OBJ) $(SERVER_OBJ)
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(SERVER_EXEC) $(OBJ) $(SERVER_OBJ)

%.o : %.c %.h
	@$(CC) -c $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: clean
clean:
	@rm -f $(OBJ) $(SERVER_OBJ) $(COOL_OBJ) $(COOL_EXEC) $(SERVER_EXEC)
#find . -name '*.o' -type f -delete
