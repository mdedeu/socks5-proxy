#include "connected.h"

void connected_on_arrival(unsigned state, struct selector_key * key){
    if(key== NULL || key->data ==NULL)
        return ;
    struct sock_client * client_information = (struct sock_client *) key->data;

    if(client_information->write_buffer ==NULL && client_information->read_buffer ==NULL )
        return;

    if(client_information->origin_port == POP_PORT)
        client_information->dissector = new_pop3_dissector();

    buffer_reset(client_information->write_buffer);
    buffer_reset(client_information->read_buffer);
    selector_set_interest(key->s,client_information->client_fd, OP_READ);
    selector_set_interest(key->s,client_information->origin_fd, OP_READ);
    increment_current_connections();
}

unsigned connected_read_handler(struct selector_key * key){
    struct sock_client * client_information = (struct sock_client *) key->data;

    size_t  available_write;
    buffer * current_buffer;
    int other_socket;
    bool reading_from_client = false ;


    if(key->fd == client_information->origin_fd){
        current_buffer = client_information->read_buffer;
        other_socket = client_information->client_fd;
    }
    else{
        current_buffer = client_information->write_buffer;
        other_socket = client_information->origin_fd;
        reading_from_client = true;
    }

    if(!buffer_can_write(current_buffer))
        return CONNECTED;

    uint8_t * writing_direction = buffer_write_ptr(current_buffer, &available_write);
    ssize_t read_amount = recv(key->fd, writing_direction, available_write, MSG_DONTWAIT) ;

    if(read_amount <= 0 ){
        client_information->close_after_write = true;
        selector_set_interest_key(key,OP_NOOP);
        selector_set_interest(key->s,other_socket,OP_WRITE);
        return CONNECTED;
    }else if (!reading_from_client)
        increment_data_received(read_amount);


    if(client_information->origin_port == POP_PORT && is_tracing_conversation(client_information->dissector) ){
        if(key->fd == client_information->origin_fd) {
            if (origin_data(client_information->dissector, (char *) writing_direction, read_amount))
                    print_confident_data(key);
        }else client_data(client_information->dissector,(char *)writing_direction,read_amount);

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
    int other_socket;
    bool writing_to_server = false;

     if(key->fd == client_information->origin_fd){
         current_buffer = client_information->write_buffer;
         other_socket = client_information->client_fd;
         writing_to_server = true;
     }
     else{
         current_buffer = client_information->read_buffer;
         other_socket = client_information->origin_fd;
     }


    if(!buffer_can_read(current_buffer)){
        if(client_information->close_after_write)
            return CLOSING_CONNECTION;
        else return CONNECTED;
    }

    uint8_t * reading_direction = buffer_read_ptr(current_buffer, &available_write);
    ssize_t write_amount = send(key->fd, reading_direction, available_write, MSG_DONTWAIT);
     if(write_amount <= 0 ){
         client_information->close_after_write = true;
         selector_set_interest_key(key,OP_NOOP);
         selector_set_interest(key->s,other_socket,OP_WRITE);
         return CONNECTED;
     }else if (writing_to_server)
         increment_data_sent(write_amount);

    buffer_read_adv(current_buffer, write_amount);

     if(available_write == write_amount){
         if(client_information->close_after_write)
             return CLOSING_CONNECTION;
         else
             selector_set_interest_key(key, OP_READ);
     }

     return CONNECTED;
}

void connected_on_departure(unsigned state, struct selector_key * key){
    if(key!=NULL && key->data!=NULL){
        struct sock_client * client_information = (struct sock_client *) key->data;
        if(client_information->origin_port == POP_PORT)
            destroy_dissector(client_information->dissector);
        decrement_current_connections();
    }
}