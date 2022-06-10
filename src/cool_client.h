#ifndef COOL_CLIENT_H
#define COOL_CLIENT_H
#include "buffer.h"
#include <stdlib.h>
#include "state/cool_state_machine.h"
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFFER_SIZE 2048

typedef struct cool_client{
    uint8_t client_fd;
    uint8_t raw_write_buffer[BUFFER_SIZE];
    buffer * write_buffer;
    struct state_machine * client_state_machine;
    union{
        struct sock_authentication_message * authentication_message;
    } current_parser;

    int origin_fd;

    struct parser * using_parser;
    void * parsed_message;

} cool_client;

struct cool_client * init_cool_client_connection(int fd);

void destroy_cool_client(struct cool_client * cool_client);

#endif