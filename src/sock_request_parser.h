#ifndef SOCK_REQUEST_PARSER_H
#define SOCK_REQUEST_PARSER_H

#include <stdint.h>
#include "parser.h"
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
    struct parser * using_parser;
};

void read_version(struct  parser_event * event , uint8_t c);

void read_cmd(struct  parser_event * event , uint8_t c);

void read_rsv(struct  parser_event * event , uint8_t c);

void read_ipv4_atyp(struct  parser_event * event , uint8_t c);

void read_ipv6_atyp(struct  parser_event * event , uint8_t c);

void read_address_atyp(struct  parser_event * event , uint8_t c);

void reading_ipv4(struct  parser_event * event , uint8_t c);

void reading_ipv6(struct  parser_event * event , uint8_t c);

void read_addr_len(struct  parser_event * event , uint8_t c);

void reading_addr(struct  parser_event * event , uint8_t c);

void reading_port(struct  parser_event * event , uint8_t c);

void handle_cmd_read_event(struct sock_request_message * sock_data ,uint8_t current_character);

void handle_ipv4_atyp_read_event(struct sock_request_message * sock_data ,uint8_t current_character);

void handle_ipv6_atyp_read_event(struct sock_request_message * sock_data ,uint8_t current_character);

void handle_addr_atyp_read_event(struct sock_request_message * sock_data ,uint8_t current_character);

void handle_addrlen_read_event(struct sock_request_message * sock_data ,uint8_t current_character);

void handle_ipv4_reading_event(struct sock_request_message * sock_data , uint8_t current_character);

void handle_ipv6_reading_event(struct sock_request_message * sock_data ,uint8_t current_character);

void handle_addr_reading_event(struct sock_request_message * sock_data ,uint8_t current_character);

void handle_port_reading_event(struct sock_request_message * sock_data ,uint8_t current_character);

struct sock_request_message * init_sock_request_parser();
void feed_sock_request_parser(struct sock_request_message * sock_data ,char * input,int input_size);
void close_sock_request_parser(struct sock_request_message * message);

#endif