#ifndef SIMPLE_RESPONSE_PARSER_H
#define SIMPLE_RESPONSE_PARSER_H

#include <stdint.h>
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#define N(x) (sizeof(x)/sizeof((x)[0]))

struct simple_response_message{
    uint8_t version;
    uint8_t response;
    struct parser * using_parser;
};

struct simple_response_message * init_simple_response_parser();
bool feed_simple_response_parser(struct simple_response_message * sock_data, char * input, int input_size);
void close_simple_response_parser(struct simple_response_message * current_data);

#endif