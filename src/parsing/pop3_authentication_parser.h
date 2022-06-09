#ifndef POP3_AUTHENTICATION_PARSER_H
#define POP3_AUTHENTICATION_PARSER_H

#include <stdint.h>
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#define N(x) (sizeof(x)/sizeof((x)[0]))
#define BUFFER_SIZE 512

struct pop3_authentication_message{
    uint8_t check_characters_read;
    uint8_t username_characters_read;
    char username[BUFFER_SIZE];
    uint8_t password_characters_read;
    char password[BUFFER_SIZE];
    struct parser * using_parser;
};

struct pop3_authentication_message * init_pop3_authentication_parser();
bool feed_pop3_authentication_parser(struct pop3_authentication_message * sock_data, char * input,int input_size);
void close_pop3_authentication_parser(struct pop3_authentication_message *  current_data);

#endif
