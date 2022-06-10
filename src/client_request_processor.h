#ifndef CLIENT_REQUEST_PROCESSOR_H
#define CLIENT_REQUEST_PROCESSOR_H

#include "selector.h"
#include "parsing/sock_hello_parser.h"
#include "client_request_processor.h"
#include "sock_client.h"
#include "buffer.h"
#include "string.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "dns_query.h"
#include "sock_client.h"
#include "parsing/sock_authentication_parser.h"
#include "parsing/sock_request_parser.h"
#include "parsing/sock_hello_parser.h"


#define USERNAME_AUTHENTICATION 2
#define CURRENT_SOCK_VERSION 5
#define NON_METHODS_ACCEPTED 255
#define VALID_USER 0
#define NO_VALID_USER 1
#define IPV4ADDRESS 1
#define IPV6ADDRESS 4
#define DOMAIN_NAME 3
#define CONNECT_COMMAND 1
#define HOST_UNREACHABLE 4
#define SUCCEEDED 0
#define SUBNEGOTIATION_CURRENT_VERSION 1



bool process_hello_message(struct sock_hello_message * data, struct selector_key * key);
bool process_authentication_message(struct sock_authentication_message * data, struct selector_key * key);
void process_request_message(struct sock_request_message  * data, struct selector_key * key);
void generate_request_answer(struct sock_request_message * data ,struct  selector_key * key);


#endif
