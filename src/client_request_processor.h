#ifndef CLIENT_REQUEST_PROCESSOR_H
#define CLIENT_REQUEST_PROCESSOR_H

#include "selector.h"
#include "sock_hello_parser.h"
#include "client_request_processor.h"
#include "sock_client.h"
#include "buffer.h"
#include "string.h"

#define USERNAME_AUTHENTICATION 2
#define VERSION 5
#define NON_METHODS_ACCEPTED 255
#define VALID_USER 0
#define NO_VALID_USER 1

void process_hello_message(struct sock_hello_message * data, struct selector_key * key);
void process_authentication_message(struct sock_authentication_message * data, struct selector_key * key);
void process_request_message(struct sock_request_message  * data, struct selector_key * key);

#endif