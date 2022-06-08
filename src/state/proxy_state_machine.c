#include "proxy_state_machine.h"

static const struct state_definition tcp_connected_state = {.state=TCP_CONNECTED, .on_read_ready=on_tcp_connected_handler_read, .on_write_ready=on_tcp_connected_handler_write,.on_departure=on_tcp_connected_departure};
static const struct state_definition hello_sock_received_state = {.state=HELLO_SOCK_RECEIVED, .on_read_ready=hello_sock_received_handler_read, .on_write_ready=hello_sock_received_handler_write,.on_departure=on_hello_sock_departure};
static const struct state_definition authenticated_state = {.state=AUTHENTICATED, .on_read_ready=authenticated_handler_read};
static const struct state_definition ready_to_connect = {.state=READY_TO_CONNECT, .on_block_ready=ready_to_connect_block_handler, .on_write_ready=ready_to_connect_write_handle,.on_arrival=ready_to_connect_on_arrival};
static const struct state_definition writing_reply = {.state=WRITING_REPLY, .on_write_ready=writing_reply_write_handler,.on_arrival=writing_reply_on_arrival};
static const struct state_definition connected = {.state=CONNECTED, .on_write_ready=connected_write_handler,.on_arrival=connected_on_arrival,.on_read_ready=connected_read_handler};
static const struct state_definition closing_connection = {.state=CLOSING_CONNECTION,.on_departure=closing_connection_on_departure,.on_arrival=closing_connection_on_arrival};

static const struct state_definition states[] = {
    tcp_connected_state,
    hello_sock_received_state,
    authenticated_state,
    ready_to_connect,
    writing_reply,
    connected,
    closing_connection
};

static struct state_machine sock_client_machine = {
        .initial=TCP_CONNECTED,
        .states=states,
        .max_state = N(states)
};

struct state_machine *init_proxy_state_machine() {
    struct state_machine *aux = malloc(sizeof(state_machine));
    memcpy(aux, &sock_client_machine, sizeof(state_machine));
    stm_init(aux);
    return aux;
}

void destroy_sock_state(struct state_machine *sock_machine) {
    free(sock_machine);
}
