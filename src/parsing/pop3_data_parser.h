#ifndef POP3_DATA_PARSER_H
#define POP3_DATA_PARSER_H

#include <stdint.h>
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#define N(x) (sizeof(x)/sizeof((x)[0]))
#define BUFFER_SIZE 512



struct pop3_data_message{
    uint8_t data_characters_read;
    char data[BUFFER_SIZE];
    uint8_t prefix_len;
    uint8_t check_characters_read;
    char prefix[BUFFER_SIZE];
    struct parser * using_parser;
    bool connected;
};

struct pop3_data_message * init_pop3_data_parser(char * prefix_received,ssize_t prefix_received_length);
bool feed_pop3_data_parser(struct pop3_data_message * sock_data, char * input,int input_size);
void close_pop3_data_parser(struct pop3_data_message *  current_data);

#endif
