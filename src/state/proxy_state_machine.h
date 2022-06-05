#ifndef PROXY_STATE_MACHINE_H
#define PROXY_STATE_MACHINE_H

#define HELLO_ANSWER_LENGTH 2
#define AUTHENTICATION_ANSWER_LENGTH 2
#define REQUEST_CONNECT_ANSWER 20
#define READ_AMOUNT 512
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
//states handlers
#include "tcp_connected.h"
#include "hello_sock_received.h"
#include "authenticated.h"
#include "ready_to_connect.h"
#include "writing_reply.h"
#include "connected.h"
#include "closing_connection.h"

enum sock_state {
    TCP_CONNECTED,
    HELLO_SOCK_RECEIVED,
    AUTHENTICATED,
    READY_TO_CONNECT,
    WRITING_REPLY,
    CONNECTED,
    CLOSING_CONNECTION
};

struct state_machine *init_proxy_state_machine();

void destroy_sock_state(struct state_machine *sock_machine);

#endif
