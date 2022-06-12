#ifndef PROXY_STATE_MACHINE_H
#define PROXY_STATE_MACHINE_H

#define HELLO_ANSWER_LENGTH 2
#define AUTHENTICATION_ANSWER_LENGTH 2
#define REQUEST_CONNECT_ANSWER 20
#define READ_AMOUNT 512
#define POP_PORT 110
#include <errno.h>
#include "stm.h"
#include <string.h>
#include "../parsing/sock_hello_parser.h"
#include "../selector.h"
#include <sys/types.h>
#include <sys/socket.h>
#include "../bufferService.h"
#include "../client_request_processor.h"
#include "../sock_client.h"
#include "../parsing/sock_authentication_parser.h"
#include "../parsing/sock_request_parser.h"
#include "../general_handlers.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "../metrics.h"
#include "time.h"

//states handlers
#include "sock_hello_reading.h"
#include "sock_hello_writing.h"
#include "authenticate_reading.h"
#include "authenticate_writing.h"
#include "request_reading.h"
#include "domain_connecting.h"
#include "address_connecting.h"
#include "sock_request_writing.h"
#include "sock_negative_request_writing.h"
#include "connected.h"
#include "closing_connection.h"



enum client_state{
    SOCK_HELLO_READING,
    SOCK_HELLO_WRITING,
    SOCK_AUTHENTICATE_READING,
    SOCK_AUTHENTICATE_WRITING,
    SOCK_REQUEST_READING,
    DOMAIN_CONNECTING,
    ADDRESS_CONNECTING,
    SOCK_REQUEST_WRITING,
    SOCK_NEGATIVE_REQUEST_WRITING,
    CONNECTED,
    CLOSING_CONNECTION
};

 enum socks_response_status{
    status_succeeded,
    status_general_socks_server_failure,
    status_connection_not_allowed_by_ruleset,
    status_network_unreachable,
    status_host_unreachable,
    status_connection_refused,
    status_ttl_expired,
    status_command_not_supported
};


struct state_machine *init_proxy_state_machine();
void destroy_sock_state(struct state_machine *sock_machine);
unsigned  errno_to_sock(const int e );
void print_connection_data(struct selector_key * key);


#endif
