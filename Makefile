CC= gcc
CFLAGS= -std=gnu11 -g -Wall -pedantic -D_POSIX_SOURCE=1 -pthread -D_GNU_SOURCE

#Si se agrega un directorio bajo src agregar una variable _DIR al final de estas siguiendo el patron
SRC_DIR= src
STATE_DIR= $(SRC_DIR)/state
PARSE_DIR= $(SRC_DIR)/parsing

EXEC= server

#Si se agrega un directorio para compilar agregar un SRC+= al final de estas siguiendo el patron
SRC= $(wildcard $(SRC_DIR)/*.c)
SRC+= $(wildcard $(PARSE_DIR)/*.c)
SRC+= $(wildcard $(STATE_DIR)/*.c)

OBJ= $(patsubst %.c, %.o, $(SRC))

.PHONY: all
all: $(OBJ)
	@$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

%.o : %.c
	@$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	@rm -f $(OBJ) server
#	find . -name '*.o' -type f -delete
