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
        client_information->origin_fd = server_socket_fd;
        connect(server_socket_fd,(struct sockaddr *) (client_information->origin_address), sizeof (struct sockaddr_in));
    }
    else if ( client_information->origin_address_length == IPV6SIZE){
        server_socket_fd = socket(AF_INET6,SOCK_STREAM,0);
        selector_fd_set_nio(server_socket_fd);
        client_information->origin_fd = server_socket_fd;
        connect(server_socket_fd,(struct sockaddr *) (client_information->origin_address), sizeof (struct sockaddr_in6));
    }

    selector_register(key->s, server_socket_fd, &socks5_handler,OP_WRITE, client_information);
}

static unsigned ready_to_connect_block_handler(struct selector_key * key){
    sock_client  * client_information = (sock_client * ) key ->data ;
    struct addrinfo * current  = client_information->current_origin_resolution == NULL ? client_information->origin_resolutions : client_information->current_origin_resolution;
    int server_socket = socket(current->ai_family,current->ai_socktype,current->ai_protocol);
    selector_fd_set_nio(server_socket);
    client_information->origin_fd = server_socket;
    connect(server_socket, current->ai_addr,current->ai_addrlen);
    selector_register(key->s, server_socket, &socks5_handler,OP_WRITE, client_information);
    return READY_TO_CONNECT;
}


static unsigned  ready_to_connect_write_handle(struct selector_key * key){
        struct sock_client  * client_information = (struct sock_client * )key->data;
    getsockopt(client_information->origin_fd,SOL_SOCKET,SO_ERROR,NULL,0);
    if( errno == ENOTCONN) {
        if(client_information->current_origin_resolution->ai_next != NULL){
            selector_unregister_fd(key->s,client_information->origin_fd);
            close(client_information->origin_fd);
            return ready_to_connect_block_handler(key);
        }
        return WRITING_REPLY;
    }
    return WRITING_REPLY;
}

static void writing_reply_on_arrival(unsigned state , struct selector_key * key ){
    struct sock_client  * client_information = (struct sock_client * )key->data;
    generate_request_answer(client_information->current_parser.request_message,key);
    selector_set_interest(key->s,client_information->client_fd,OP_WRITE);
}

static void connected_on_arrival(unsigned  state , struct selector_key * key){
    struct sock_client  * client_information = (struct sock_client * )key->data;
    buffer_reset(client_information->write_buffer);
    buffer_reset(client_information->read_buffer);
    close_sock_request_parser(client_information->current_parser.request_message);
    selector_set_interest(key->s,client_information->client_fd,OP_READ);
    selector_set_interest(key->s,client_information->origin_fd,OP_READ);

}

static unsigned  connected_read_handler(struct  selector_key * key){
    struct sock_client  * client_information = (struct sock_client * )key->data;
    size_t  available_write;
    buffer * current_buffer;
    if(key->fd == client_information->origin_fd )
        current_buffer = client_information->read_buffer;
    else
        current_buffer=client_information->write_buffer;

   if(!buffer_can_write(current_buffer))
       return CONNECTED;

    uint8_t * writing_direction =    buffer_write_ptr(current_buffer,&available_write);
    ssize_t read_amount = recv(key->fd,writing_direction,available_write,MSG_DONTWAIT);
    if(!read_amount){
        return CLOSING_CONNECTION;
    }
    buffer_write_adv(current_buffer,read_amount);

    if(read_amount > 0 &&key->fd == client_information->origin_fd )
        selector_set_interest(key->s,client_information->client_fd,OP_READ|OP_WRITE);
    else
        selector_set_interest(key->s,client_information->origin_fd,OP_READ|OP_WRITE) ;

    return CONNECTED;
}

static unsigned connected_write_handler(struct selector_key * key ){
    struct sock_client  * client_information = (struct sock_client * )key->data;
    size_t  available_write;
    buffer * current_buffer;
    if(key->fd == client_information->origin_fd )
        current_buffer = client_information->write_buffer;
    else
        current_buffer=client_information->read_buffer;

    if(!buffer_can_read(current_buffer))
        return CONNECTED;

    uint8_t  * reading_direction = buffer_read_ptr(current_buffer,&available_write);
    ssize_t write_amount = send(key->fd,reading_direction,available_write,MSG_DONTWAIT);
    buffer_read_adv(current_buffer,write_amount);

    if(available_write == write_amount)
        selector_set_interest_key(key,OP_READ);


    return CONNECTED;

}



static unsigned  writing_reply_write_handler(struct selector_key * key){
    struct sock_client  * client_information = (struct sock_client * )key->data;

    if(!buffer_can_write(client_information->write_buffer))
        return WRITING_REPLY;

    size_t  available_write ;
    uint8_t  * writing_direction = buffer_write_ptr(client_information->write_buffer,&available_write);

    size_t available_read;
    uint8_t * reading_direction = buffer_read_ptr(client_information->current_parser.request_message->answer_buffer,&available_read);

    int minimum_space = available_read>=available_write?available_write:available_read;
    memcpy(writing_direction, reading_direction,minimum_space);
    buffer_write_adv(client_information->write_buffer,minimum_space);
    buffer_read_adv(client_information->current_parser.request_message->answer_buffer,minimum_space);

    reading_direction = buffer_read_ptr(client_information->write_buffer,&available_read);
    int sent_bytes= send(client_information->client_fd,reading_direction,available_read,MSG_DONTWAIT);
    buffer_read_adv(client_information->write_buffer,sent_bytes);

    if(!buffer_can_read(client_information->current_parser.request_message->answer_buffer) && !buffer_can_read(client_information->write_buffer)){

        return CONNECTED;
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


static unsigned  closing_connection_on_departure(const unsigned state ,struct selector_key * key){
    close(key->fd);
    return CLOSING_CONNECTION;
}

static unsigned closing_connection_on_arrival(const unsigned state ,struct selector_key * key ){
    selector_unregister_fd(key->s,key->fd);
    sock_client * client_information = (sock_client*) key->data;
    if(key->fd == client_information->origin_fd)
        selector_unregister_fd(key->s,client_information->client_fd);
    else
        selector_unregister_fd(key->s,client_information->origin_fd);
    destroy_sock_client(client_information);
    return CLOSING_CONNECTION;
}


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
    //struct state_machine *current = stm_init(&aux);
    //TODO: mirar si esto esta bien
    stm_init(aux);
    return aux;
}

void destroy_sock_state(struct state_machine *sock_machine) {
    free(sock_machine);
}
