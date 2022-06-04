#include "proxy_state_machine.h"


//return the new state if corresponding
//should read from the fd associated (key->fd) and give the data read to the corresponding parser.
// How to save the parser for the current client??
static unsigned on_tcp_connected_handler_read(struct selector_key *key) {
    char aux_buff[READ_AMOUNT];
    sock_client *client_data = (sock_client *) key->data;
    uint8_t available_space;
    buffer_write_ptr(client_data->write_buffer, &available_space);
    if (available_space >= HELLO_SOCK_RECEIVED) {
        int read_amount = recv(key->fd, aux_buff, READ_AMOUNT, MSG_DONTWAIT);
        bool finished = feed_sock_hello_parser((struct sock_hello_message *) (client_data->current_parser), aux_buff,
                                               read_amount);
        if (!finished)
            return TCP_CONNECTED;
        else {
            process_hello_message(client_data->current_parser, key);
            selector_set_interest_key(key, OP_WRITE);
            return HELLO_SOCK_RECEIVED;
        }
    }
}

static unsigned hello_sock_received_handler_read(struct selector_key *key);

static unsigned authenticated_handler_read(struct selector_key *key);

static unsigned connect_sock_received_handler_read(struct selector_key *key);

static unsigned connected_handler_read(struct selector_key *key);

//return the new state if corresponding
static unsigned on_tcp_connected_handler_write(struct selector_key *key) {
    sock_client *client_data = (sock_client *) key->data;
    if (!buffer_can_read(client_data->write_buffer))
        return TCP_CONNECTED;
    size_t write_amount;
    uint8_t *reading_since = buffer_read_ptr(client_data->write_buffer, &write_amount);
    ssize_t written_bytes = send(client_data->fd, reading_since, write_amount, MSG_DONTWAIT);
    buffer_read_adv(client_data->write_buffer, written_bytes);
    buffer_compact(client_data->write_buffer);
    selector_set_interest_key(key, OP_READ);
}

static unsigned hello_sock_received_handler_write(struct selector_key *key);

static unsigned authenticated_handler_write(struct selector_key *key);

static unsigned connect_sock_received_handler_write(struct selector_key *key);

static unsigned connected_handler_write(struct selector_key *key);


static struct state_definition tcp_connected_state = {.state=TCP_CONNECTED, .on_read_ready=on_tcp_connected_handler_read, .on_write_ready=on_tcp_connected_handler_write};
static struct state_definition hello_sock_received_state = {.state=HELLO_SOCK_RECEIVED, .on_read_ready=hello_sock_received_handler_read, .on_write_ready=hello_sock_received_handler_write};
static struct state_definition authenticated_state = {.state=AUTHENTICATED, .on_read_ready=authenticated_handler_read, .on_write_ready=authenticated_handler_write};
static struct state_definition connect_sock_received_state = {.state=CONNECT_SOCK_RECEIVED, .on_read_ready=connect_sock_received_handler_read, .on_write_ready=connect_sock_received_handler_write};
static struct state_definition connected_state = {.state=CONNECTED, .on_read_ready=connected_handler_read, .on_write_ready=connected_handler_write};

static struct state_definition states[] = {
    {.state=TCP_CONNECTED, .on_read_ready=on_tcp_connected_handler_read, .on_write_ready=on_tcp_connected_handler_write},
    {.state=HELLO_SOCK_RECEIVED, .on_read_ready=hello_sock_received_handler_read, .on_write_ready=hello_sock_received_handler_write},
    {.state=AUTHENTICATED, .on_read_ready=authenticated_handler_read, .on_write_ready=authenticated_handler_write},
    {.state=CONNECT_SOCK_RECEIVED, .on_read_ready=connect_sock_received_handler_read, .on_write_ready=connect_sock_received_handler_write},
    {.state=CONNECTED, .on_read_ready=connected_handler_read, .on_write_ready=connected_handler_write}
};

static struct state_machine sock_client_machine = {
        .initial=TCP_CONNECTED,
        .states=states,
        .max_state = N(states)
};

struct state_machine *init_proxy_state_machine() {
    struct state_machine *aux = malloc(sizeof(state_machine));
    memcpy(aux, &sock_client_machine, sizeof(state_machine));
    struct state_machine *current = stm_init(&aux);
}

void destroy_sock_state(struct state_machine *sock_machine) {
    free(sock_machine);
}
