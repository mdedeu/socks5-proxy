#ifndef SOCK_HELLO_PARSER_H
#define SOCK_HELLO_PARSER_H
#include <stdint.h>
#include "parser.h"
#include <stdio.h>
#include <unistd.h>
#define N(x) (sizeof(x)/sizeof((x)[0]))

enum states_and_events{
    INITIAL_STATE,
    VERSION_READ,
    NMETHODS_READ,
    READING_METHODS,
    END,

    VERSION_READ_EVENT,
    NMETHODS_READ_EVENT,
    METHODS_READ_EVENT,
    END_REACH_EVENT,
    ERROR_FOUND_EVENT
};


struct sock_hello_message{
    uint8_t  version;
    uint8_t nmethods;
    uint8_t * methods;
    uint8_t last_method_added;
    struct parser * using_parser;
};

struct sock_hello_message * init_sock_hello_parser();
bool feed_sock_hello_parser(struct sock_hello_message * sock_data ,char * input,int input_size);
void close_sock_hello_parser(struct sock_hello_message *  current_data);

#endif
