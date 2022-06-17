#ifndef COOL_REQUEST_PROCESSOR_H
#define COOL_REQUEST_PROCESSOR_H

#include "selector.h"
#include "cool_client.h"
#include "buffer.h"
#include "string.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "dns_query.h"
#include "metrics.h"

#include "parsing/parserCoolProtocolAuth.h"
#include "parsing/cool_request_parser.h"


#define USERNAME_AUTHENTICATION 2
#define VERSION 5
#define NON_METHODS_ACCEPTED 255
#define VALID_USER 0
#define NO_VALID_USER 1
#define IPV4ADDRESS 1
#define IPV6ADDRESS 4
#define DOMAIN_NAME 3
#define CONNECT_COMMAND 1
#define HOST_UNREACHABLE 4
#define SUCCEEDED 0

#define ADD_USER 0x00
#define REMOVE_USER 0x01
#define ENABLE_SPOOFING 0x02
#define DISABLE_SPOOFING 0x03
#define CHANGE_BUFFER_SIZE 0x04

#define GET_TOTAL_CONNECTIONS 0x01
#define GET_CURRENT_CONNECTIONS 0x02
#define GET_MAX_CURRENT_CONNECTIONS 0x03
#define GET_TOTAL_BYTES_SENT 0x04
#define GET_TOTAL_BYTES_RECV 0x05
#define GET_CONNECTED_USERS 0x06
#define GET_USER_LIST 0x07




void process_cool_authentication_message(struct cool_protocol_authentication_message * data, struct selector_key * key);
void process_cool_request_message(struct general_request_message * data, struct selector_key * key);


#endif
