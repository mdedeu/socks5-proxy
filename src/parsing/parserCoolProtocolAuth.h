#ifndef PARSER_H_AUTH_COOLPROTOCOL
#define PARSER_H_AUTH_COOLPROTOCOL

#include <stdint.h>
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define N(x) (sizeof(x)/sizeof((x)[0]))

struct cool_protocol_authentication_message {
	uint8_t  version;
	uint8_t  username_length;
	uint8_t username_characters_read;
	char * username;
	uint8_t  password_length;
	uint8_t password_characters_read;
	char * password;
	struct parser * using_parser;
};



struct cool_protocol_authentication_message * init_cool_protocol_authentication_message();
struct parser * init_cool_protocol_authentication_parser();
bool feed_cool_protocol_authentication_parser(struct parser * using_parser, struct cool_protocol_authentication_message * sock_data , char * input, int input_size);
void close_cool_protocol_authentication_message(struct cool_protocol_authentication_message *  current_data);
void close_cool_protocol_authentication_parser(struct parser * using_parser);

#endif
