#ifndef POP3_CONNECTED_PARSER_H
#define POP3_CONNECTED_PARSER_H

#include <stdint.h>
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define N(x) (sizeof(x)/sizeof((x)[0]))
#define CONNECTED_BUFFER_SIZE 512

struct pop3_connected_message{
    uint8_t check_characters_read;
    struct parser * using_parser;
    char prefix[CONNECTED_BUFFER_SIZE];
    uint8_t prefix_len;
    bool connected;
};

struct pop3_connected_message * init_pop3_connected_parser(char * prefix,size_t prefix_length);
bool feed_pop3_connected_parser(struct pop3_connected_message * sock_data, char * input,int input_size);
void close_pop3_connected_parser(struct pop3_connected_message *  current_data);

#endif
