#ifndef COOL_CLIENT_H
#define COOL_CLIENT_H
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "state/cool_state_machine.h"
#include "buffer.h"

#define BUFFER_SIZE 2048

enum cool_state{
    COOL_CONNECTED,
    COOL_AUTHENTICATING
};

typedef struct cool_client{
    uint8_t client_fd;
    uint8_t raw_write_buffer[BUFFER_SIZE];
    buffer * write_buffer;
    struct state_machine * client_state_machine;

    struct parser * using_parser;
    void * parsed_message;
    int state;

} cool_client;

struct cool_client * init_cool_client_connection(int fd);

void destroy_cool_client(struct cool_client * cool_client);

#endif