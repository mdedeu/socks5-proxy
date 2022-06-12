#include "proxy_state_machine.h"

static const struct state_definition sock_hello_reading_state= {.state=SOCK_HELLO_READING, .on_arrival=sock_hello_reading_on_arrival,.on_read_ready=sock_hello_read_handler,.on_departure=sock_hello_reading_on_departure};
static const struct state_definition sock_hello_writing_state = {.state=SOCK_HELLO_WRITING, .on_arrival=sock_hello_writing_on_arrival,.on_write_ready=sock_hello_write_handler,.on_departure=sock_hello_writing_on_departure};
static const struct state_definition authenticate_reading_state = {.state=SOCK_AUTHENTICATE_READING, .on_arrival=authenticate_reading_arrival,.on_read_ready=authenticate_read_handler,.on_departure=authenticate_reading_departure};
static const struct state_definition authenticate_writing_state= {.state=SOCK_AUTHENTICATE_WRITING, .on_arrival = authenticate_writing_arrival,.on_write_ready=authenticate_write_handler,.on_departure=authenticate_writing_departure};
static const struct state_definition sock_request_reading = {.state=SOCK_REQUEST_READING, .on_arrival=request_reading_arrival,.on_read_ready=request_reading_read_handler,.on_block_ready=request_reading_block_handler,.on_departure=request_reading_departure};
static const struct state_definition domain_connecting = {.state=DOMAIN_CONNECTING, .on_arrival=domain_connecting_arrival,.on_write_ready=domain_connecting_write_handler};
static const struct state_definition address_connecting = {.state=ADDRESS_CONNECTING,.on_write_ready=address_connecting_write_handler};
static const struct state_definition sock_request_writing = {.state=SOCK_REQUEST_WRITING, .on_write_ready=sock_request_writing_write_handler,.on_arrival=sock_request_writing_arrival,.on_departure=sock_request_writing_departure};
static const struct state_definition sock_negative_request_writing = {.state=SOCK_NEGATIVE_REQUEST_WRITING, .on_write_ready=sock_negative_request_write_handler,.on_arrival=sock_negative_request_writing_arrival,.on_departure=sock_negative_request_writing_departure};
static const struct state_definition connected = {.state=CONNECTED, .on_write_ready=connected_write_handler,.on_arrival=connected_on_arrival,.on_read_ready=connected_read_handler,.on_departure=connected_on_departure};
static const struct state_definition closing_connection = {.state=CLOSING_CONNECTION,.on_arrival=closing_connection_on_arrival};

static const struct state_definition states[] = {
        sock_hello_reading_state,
        sock_hello_writing_state,
        authenticate_reading_state,
        authenticate_writing_state,
        sock_request_reading,
        domain_connecting,
        address_connecting,
        sock_request_writing,
        sock_negative_request_writing,
        connected,
        closing_connection
};

static struct state_machine sock_client_machine = {
        .initial=SOCK_HELLO_READING,
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


unsigned  errno_to_sock(const int e ){
    switch(e){
        case 0:
            return status_succeeded;
        case ECONNREFUSED:
            return status_connection_refused;
        case EHOSTUNREACH:
            return status_host_unreachable;
        case ENETUNREACH:
            return status_network_unreachable;
        case ETIMEDOUT:
            return status_ttl_expired;
        default:
            return status_general_socks_server_failure;
    }
}