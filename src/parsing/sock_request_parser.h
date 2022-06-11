#ifndef SOCK_REQUEST_PARSER_H
#define SOCK_REQUEST_PARSER_H

#include <stdint.h>
#include "parser.h"
#include "../buffer.h"
#include "../client_request_processor.h"
#define IPV4SIZE 4
#define IPV6SIZE 16
#define N(x) (sizeof(x)/sizeof((x)[0]))




struct sock_request_message{
    uint8_t  version;
    uint8_t cmd;
    uint8_t atyp;
    uint8_t  * ipv4;
    uint8_t * ipv6 ;
    uint8_t * address;
    uint8_t * port;
    uint8_t addrlen;
    uint8_t addr_character_read;
    uint8_t ipv4_character_read;
    uint8_t ipv6_character_read;
    uint8_t  port_character_read;
    uint8_t  connection_result;
};

struct parser * init_sock_request_parser();
struct sock_request_message * init_sock_request_message();
bool feed_sock_request_parser(struct parser * using_parser , struct sock_request_message * sock_data ,char * input,int input_size);
void close_sock_request_parser(struct parser  * parser);
void  close_sock_request_message(struct sock_request_message * message);

#endif