#ifndef SOCK_AUTHENTICATION_PARSER_H
#define SOCK_AUTHENTICATION_PARSER_H

#include <stdint.h>
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#define N(x) (sizeof(x)/sizeof((x)[0]))



enum states_and_events{
    INITIAL_STATE,
    VERSION_READ,
    ULEN_READ,
    READING_USERNAME,
    FINISH_USERNAME,
    PLEN_READ,
    READING_PASSWORD,
    END,

    VERSION_READ_EVENT,
    ULEN_READ_EVENT,
    USERNAME_READ_EVENT,
    PLEN_READ_EVENT,
    READING_PASSWORD_EVENT,
    END_REACH_EVENT,
    ERROR_FOUND_EVENT
};


struct sock_authentication_message{
    uint8_t  version;
    uint8_t  username_length;
    uint8_t username_characters_read;
    char * username;
    uint8_t  password_length;
    uint8_t password_characters_read ;
    char * password;
    struct parser * using_parser;
};

struct sock_authentication_message * init_sock_authentication_parser();
void feed_sock_authentication_parser(struct sock_authentication_message * sock_data ,char * input,int input_size);
void close_sock_authentication_parser(struct sock_authentication_message *  current_data);

#endif