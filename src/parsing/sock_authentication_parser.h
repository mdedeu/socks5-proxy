#ifndef SOCK_AUTHENTICATION_PARSER_H
#define SOCK_AUTHENTICATION_PARSER_H

#include <stdint.h>
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#define N(x) (sizeof(x)/sizeof((x)[0]))

struct sock_authentication_message{
    uint8_t  version;
    uint8_t  username_length;
    uint8_t username_characters_read;
    char * username;
    uint8_t  password_length;
    uint8_t password_characters_read ;
    char * password;
};

struct sock_authentication_message * init_sock_authentication_message();
struct parser * init_sock_authentication_parser();
bool feed_sock_authentication_parser(struct parser * using_parser , struct sock_authentication_message * sock_data ,char * input,int input_size);
void close_sock_authentication_message(struct sock_authentication_message *  current_data);
void close_sock_authentication_parser(struct parser*  using_parser);
#endif
