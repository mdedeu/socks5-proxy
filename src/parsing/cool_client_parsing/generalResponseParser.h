#ifndef GENERAL_RESPONSE_PARSER_H
#define GENERAL_RESPONSE_PARSER_H

#include <stdint.h>
#include "../parser.h"
#include <stdio.h>
#include <stdlib.h>
#define N(x) (sizeof(x)/sizeof((x)[0]))

struct general_response_message{
    uint8_t action;
    uint8_t method;

    uint16_t response_length;
    uint8_t response_length_characaters_read;

    unsigned char * response;
    uint8_t response_characters_read;

    struct parser * using_parser;
};

struct general_response_message * init_general_response_parser();
bool feed_general_response_parser(struct general_response_message * sock_data, unsigned char * input, int input_size);
void close_general_response_parser(struct general_response_message * current_data);

#endif