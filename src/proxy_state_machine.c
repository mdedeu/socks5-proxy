#include "proxy_state_machine.h"


//return the new state if corresponding
//should read from the fd associated (key->fd) and give the data read to the corresponding parser.
// How to save the parser for the current client??
static unsigned on_tcp_connected_handler_read(struct selector_key *key) {
    char aux_buff[READ_AMOUNT];
    sock_client *client_data = (sock_client *) key->data;
    size_t available_space;
    buffer_write_ptr(client_data->write_buffer, &available_space);
    if (available_space >= HELLO_ANSWER_LENGTH) {
        int read_amount = recv(key->fd, aux_buff, READ_AMOUNT, MSG_DONTWAIT);
        bool finished = feed_sock_hello_parser((struct sock_hello_message *) (client_data->current_parser.hello_message), aux_buff,
                                               read_amount);
        if (!finished)
            return TCP_CONNECTED;
        else {
            process_hello_message(client_data->current_parser.hello_message, key);
            selector_set_interest_key(key, OP_WRITE);
            return TCP_CONNECTED;
        }
    }
    return TCP_CONNECTED;
}

static unsigned hello_sock_received_handler_read(struct selector_key *key){
    char aux_buff[READ_AMOUNT];
    sock_client *client_data = (sock_client *) key->data;
    size_t available_space;
    buffer_write_ptr(client_data->write_buffer, &available_space);
    if (available_space >=  AUTHENTICATION_ANSWER_LENGTH) {
        int read_amount = recv(key->fd, aux_buff, READ_AMOUNT, MSG_DONTWAIT);
        bool finished = feed_sock_authentication_parser((struct sock_authentication_message *) (client_data->current_parser.authentication_message), aux_buff,
                                               read_amount);
        if(!finished)
                return HELLO_SOCK_RECEIVED;
        else {
            process_authentication_message(client_data->current_parser.authentication_message, key);
            selector_set_interest_key(key, OP_WRITE);
            return HELLO_SOCK_RECEIVED;
        }
    }
        return HELLO_SOCK_RECEIVED;
    }




static unsigned authenticated_handler_read(struct selector_key *key){
    char aux_buff[READ_AMOUNT];
    sock_client *client_data = (sock_client *) key->data;
    size_t available_space;
    buffer_write_ptr(client_data->write_buffer, &available_space);
    if (available_space >=  REQUEST_CONNECT_ANSWER) {
        int read_amount = recv(key->fd, aux_buff, READ_AMOUNT, MSG_DONTWAIT);
        bool finished = feed_sock_request_parser((struct sock_request_message *) (client_data->current_parser.request_message), aux_buff,
                                                        read_amount);
        if(!finished)
            return AUTHENTICATED;
        else {
            process_request_message(client_data->current_parser.request_message, key);
            return READY_TO_CONNECT;
        }
    }
    return AUTHENTICATED;

}

static void ready_to_connect_on_arrival(unsigned state , struct selector_key * key ){
    sock_client * client_information  = (sock_client*) key->data;

    int server_socket_fd;
    if(client_information->origin_address_length == IPV4SIZE){
        server_socket_fd = socket(AF_INET,SOCK_STREAM,0);
        selector_fd_set_nio(server_socket_fd);
        client_information->origin_fd = connect(server_socket_fd,(struct sockaddr *) &(client_information->origin_address), sizeof (struct sockaddr_in));
    }
    else if ( client_information->origin_address_length == IPV6SIZE){
        server_socket_fd = socket(AF_INET6,SOCK_STREAM,0);
        selector_fd_set_nio(server_socket_fd);
        client_information->origin_fd = connect(server_socket_fd,(struct sockaddr *) &(client_information->origin_address), sizeof (struct sockaddr_in6));
    }

    selector_register(key->s, server_socket_fd, &socks5_handler,OP_WRITE, client_information);
}

static unsigned ready_to_connect_block_handler(struct selector_key * key){
    sock_client  * client_information = (sock_client * ) key ->data ;
    struct addrinfo * current  = client_information->current_origin_resolution == NULL ? client_information->origin_resolutions : client_information->current_origin_resolution;
    int server_socket = socket(current->ai_family,current->ai_socktype,current->ai_protocol);
    selector_fd_set_nio(server_socket);
    client_information->origin_fd = connect(server_socket, current->ai_addr,current->ai_addrlen);
    selector_register(key->s, server_socket, &socks5_handler,OP_WRITE, client_information);
    return READY_TO_CONNECT;
}


static unsigned  ready_to_connect_write_handle(struct selector_key * key){
        struct sock_client  * client_information = (struct sock_client * )key->data;
    if(client_information->origin_fd < 0) {
        if(client_information->current_origin_resolution->ai_next != NULL){
            selector_unregister_fd(key->s,client_information->origin_fd);
            close(client_information->origin_fd);
            return ready_to_connect_block_handler(key);
        }
        return WRITING_REPLY;
    }
    return WRITING_REPLY;
}
//static unsigned connect_sock_received_handler_read(struct selector_key *key);
//
//static unsigned connected_handler_read(struct selector_key *key);

//return the new state if corresponding
static unsigned on_tcp_connected_handler_write(struct selector_key *key) {
    sock_client *client_data = (sock_client *) key->data;
    if (!buffer_can_read(client_data->write_buffer))
        return TCP_CONNECTED;
    size_t write_amount;
    uint8_t *reading_since = buffer_read_ptr(client_data->write_buffer, &write_amount);
    ssize_t written_bytes = send(client_data->client_fd, reading_since, write_amount, MSG_DONTWAIT);
    buffer_read_adv(client_data->write_buffer, written_bytes);
    buffer_compact(client_data->write_buffer);
    selector_set_interest_key(key, OP_READ);
    return HELLO_SOCK_RECEIVED;
}
static void on_tcp_connected_departure(const unsigned int leaving_state, struct selector_key *key){
    sock_client *client_data = (sock_client *) key->data;
    close_sock_hello_parser(client_data->current_parser.hello_message);
    client_data->current_parser.authentication_message= init_sock_authentication_parser();
}

static void on_hello_sock_departure(const unsigned int leaving_state, struct selector_key *key){
    sock_client *client_data = (sock_client *) key->data;
    close_sock_authentication_parser(client_data->current_parser.authentication_message);
    client_data->current_parser.request_message= init_sock_request_parser();
}



static unsigned hello_sock_received_handler_write(struct selector_key *key){
    sock_client *client_data = (sock_client *) key->data;
    if (!buffer_can_read(client_data->write_buffer))
        return HELLO_SOCK_RECEIVED;
    size_t write_amount;
    uint8_t *reading_since = buffer_read_ptr(client_data->write_buffer, &write_amount);
    ssize_t written_bytes = send(client_data->client_fd, reading_since, write_amount, MSG_DONTWAIT);
    buffer_read_adv(client_data->write_buffer, written_bytes);
    buffer_compact(client_data->write_buffer);
    selector_set_interest_key(key, OP_READ);
    return AUTHENTICATED;

}

//static unsigned authenticated_handler_write(struct selector_key *key);
//static unsigned connect_sock_received_handler_write(struct selector_key *key);
//
//static unsigned connected_handler_write(struct selector_key *key);


static const struct state_definition tcp_connected_state = {.state=TCP_CONNECTED, .on_read_ready=on_tcp_connected_handler_read, .on_write_ready=on_tcp_connected_handler_write,.on_departure=on_tcp_connected_departure};
static const struct state_definition hello_sock_received_state = {.state=HELLO_SOCK_RECEIVED, .on_read_ready=hello_sock_received_handler_read, .on_write_ready=hello_sock_received_handler_write,.on_departure=on_hello_sock_departure};
static const struct state_definition authenticated_state = {.state=AUTHENTICATED, .on_read_ready=authenticated_handler_read};
static const struct state_definition ready_to_connect = {.state=READY_TO_CONNECT, .on_block_ready=ready_to_connect_block_handler, .on_write_ready=ready_to_connect_write_handle,.on_arrival=ready_to_connect_on_arrival};

//static struct state_definition connect_sock_received_state = {.state=CONNECT_SOCK_RECEIVED, .on_read_ready=connect_sock_received_handler_read, .on_write_ready=connect_sock_received_handler_write};
//static struct state_definition connected_state = {.state=CONNECTED, .on_read_ready=connected_handler_read, .on_write_ready=connected_handler_write};

static const struct state_definition states[] = {
    tcp_connected_state,
    hello_sock_received_state,
    authenticated_state,
    ready_to_connect
//    {.state=AUTHENTICATED, .on_read_ready=authenticated_handler_read, .on_write_ready=authenticated_handler_write},
//    {.state=CONNECT_SOCK_RECEIVED, .on_read_ready=connect_sock_received_handler_read, .on_write_ready=connect_sock_received_handler_write},
//    {.state=CONNECTED, .on_read_ready=connected_handler_read, .on_write_ready=connected_handler_write}
};

static struct state_machine sock_client_machine = {
        .initial=TCP_CONNECTED,
        .states=states,
        .max_state = N(states)
};

struct state_machine *init_proxy_state_machine() {
    struct state_machine *aux = malloc(sizeof(state_machine));
    memcpy(aux, &sock_client_machine, sizeof(state_machine));
    //struct state_machine *current = stm_init(&aux);
    //TODO: mirar si esto esta bien
    stm_init(aux);
    return aux;
}

void destroy_sock_state(struct state_machine *sock_machine) {
    free(sock_machine);
}
