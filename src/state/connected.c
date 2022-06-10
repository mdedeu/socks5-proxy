#include "connected.h"

void connected_on_arrival(unsigned state, struct selector_key * key){
    struct sock_client * client_information = (struct sock_client *) key->data;

    buffer_reset(client_information->write_buffer);
    buffer_reset(client_information->read_buffer);

//    close_sock_request_parser(client_information->current_parser.request_message);

    selector_set_interest(key->s,client_information->client_fd, OP_READ);
    selector_set_interest(key->s,client_information->origin_fd, OP_READ);
}

unsigned connected_read_handler(struct selector_key * key){
    struct sock_client * client_information = (struct sock_client *) key->data;

    size_t  available_write;
    buffer * current_buffer;

    if(key->fd == client_information->origin_fd)
        current_buffer = client_information->read_buffer;
    else
        current_buffer = client_information->write_buffer;

    if(!buffer_can_write(current_buffer))
        return CONNECTED;

    uint8_t * writing_direction = buffer_write_ptr(current_buffer, &available_write);
    ssize_t read_amount = recv(key->fd, writing_direction, available_write, MSG_DONTWAIT);

    if(!read_amount){
        return CLOSING_CONNECTION;
    }

    buffer_write_adv(current_buffer, read_amount);

    if(key->fd == client_information->origin_fd)
        selector_set_interest(key->s, client_information->client_fd, OP_READ | OP_WRITE);
    else
        selector_set_interest(key->s, client_information->origin_fd, OP_READ | OP_WRITE) ;

    return CONNECTED;
}

 unsigned connected_write_handler(struct selector_key * key){
    struct sock_client * client_information = (struct sock_client *) key->data;

    size_t available_write;
    buffer * current_buffer;

    if(key->fd == client_information->origin_fd)
        current_buffer = client_information->write_buffer;
    else
        current_buffer=client_information->read_buffer;

    if(!buffer_can_read(current_buffer))
        return CONNECTED;

    uint8_t * reading_direction = buffer_read_ptr(current_buffer, &available_write);
    ssize_t write_amount = send(key->fd, reading_direction, available_write, MSG_DONTWAIT);

    buffer_read_adv(current_buffer, write_amount);

    if(available_write == write_amount)
        selector_set_interest_key(key, OP_READ);

    return CONNECTED;
}