#ifndef COOL_REQUEST_PARSER_H
#define COOL_REQUEST_PARSER_H

#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#define QUERY 0xD0
#define MODIFY 0xBE
#define N(x) (sizeof(x)/sizeof((x)[0]))

struct general_request_message {
    uint8_t action; 
    uint8_t method; 
    uint8_t protocol; 
    uint8_t ulen; 
    uint8_t plen; 
    uint8_t * username; 
    uint8_t * password; 
    uint8_t username_characters_read; 
    uint8_t password_characters_read; 
    bool clients_need_authentication;
    struct parser * using_parser; 
};

struct general_request_message * init_general_parser();
void destroy_general_request_message(struct general_request_message * message);
bool feed_general_request_parser(struct general_request_message * general_request_data, char * input, int input_size);
void destroy_general_request_parser(struct parser*  using_parser);


#endif