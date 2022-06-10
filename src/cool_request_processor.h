#ifndef COOL_REQUEST_PROCESSOR_H
#define COOL_REQUEST_PROCESSOR_H

#include "selector.h"
#include "cool_request_processor.h"
#include "cool_client.h"
#include "buffer.h"
#include "string.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "dns_query.h"
#include "parsing/parserCoolProtocolAuth.h"


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



void process_cool_authentication_message(struct cool_protocol_authentication_message * data, struct selector_key * key);


#endif
