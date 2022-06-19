#ifndef COOL_STATE_MACHINE_H
#define COOL_STATE_MACHINE_H

#define HELLO_ANSWER_LENGTH 2
#define AUTHENTICATION_ANSWER_LENGTH 2
#define REQUEST_CONNECT_ANSWER 20
#define READ_AMOUNT 512
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "stm.h"
#include "../parsing/sock_hello_parser.h"
#include "../selector.h"
#include "../cool_request_processor.h"
#include "../cool_client.h"

#include "../parsing/parserCoolProtocolAuth.h"
#include "../parsing/cool_request_parser.h"

#include "cool_states/cool_authenticate_reading.h"
#include "cool_states/cool_authenticate_writing.h"
#include "cool_states/cool_request_reading.h"
#include "cool_states/cool_response_writing.h"
#include "cool_states/closing_cool_connection.h"

enum cool_client_state{
    COOL_AUTHENTICATE_READING,
    COOL_AUTHENTICATE_WRITING,
    COOL_REQUEST_READING,
    COOL_RESPONSE_WRITING,
    CLOSING_COOL_CONNECTION
};


struct state_machine *init_cool_state_machine();
void destroy_cool_state(struct state_machine *cool_machine);

#endif
