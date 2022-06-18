#ifndef SOCK_CLIENT_H
#define SOCK_CLIENT_H
#include "buffer.h"
#include <stdlib.h>
#include "state/proxy_state_machine.h"
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "./state/pop3_dissector.h"

#define BUFFER_SIZE 2048

typedef struct sock_client{
    uint16_t client_fd;
    struct sockaddr_storage * client_information;
    uint8_t raw_read_buffer[BUFFER_SIZE];
    uint8_t raw_write_buffer[BUFFER_SIZE];
    buffer * read_buffer;
    buffer * write_buffer;
    struct state_machine * client_state_machine;


    struct addrinfo * origin_resolutions;
    struct addrinfo * current_origin_resolution;

    struct sockaddr_storage * origin_address;
    socklen_t  origin_address_length;
    uint16_t  origin_port;
    int origin_fd;

    struct parser * using_parser;
    void * parsed_message;
    bool close_after_write;
    pop3_dissector * dissector;

    char * username ;


} sock_client;

struct sock_client * init_new_client_connection(int fd,struct sockaddr * client_information);

void destroy_sock_client(struct sock_client * sock_client);

#endif