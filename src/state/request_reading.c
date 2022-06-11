#include "request_reading.h"
#define  MAX_READ_LENGTH 512

static bool try_connect( struct selector_key * key);

void request_reading_arrival(const unsigned state , struct selector_key * key ){
    if(key!=NULL && key->data!=NULL){
        sock_client *client_data = (sock_client *) key->data;
        client_data->using_parser = init_sock_request_parser();
        client_data->parsed_message = init_sock_request_message();
        selector_set_interest_key(key,OP_READ);
    }
}


unsigned request_reading_read_handler(struct selector_key *key){
    if(key==NULL || key->data == NULL )
        return CLOSING_CONNECTION;
    sock_client *client_data = (sock_client *) key->data;

    char temp_buffer[MAX_READ_LENGTH];

    int received_amount = recv(key->fd, temp_buffer, MAX_READ_LENGTH, MSG_DONTWAIT);
    if(received_amount <= 0 )
        return CLOSING_CONNECTION;


    struct sock_request_message * received_message = (struct sock_request_message *)client_data->parsed_message;
    if(received_message == NULL)
        return CLOSING_CONNECTION;

    bool finished = feed_sock_request_parser(
            client_data->using_parser,
            received_message,
            temp_buffer,
            received_amount
    );

    if(!finished)
        return SOCK_REQUEST_READING;

    if(received_message->cmd != CONNECT_COMMAND){
        received_message->connection_result = status_command_not_supported;
        return SOCK_NEGATIVE_REQUEST_WRITING;
    }

    if(received_message->atyp == DOMAIN_NAME)
        return RESOLVING_HOST_ADDRESS;
    else if(received_message->atyp == IPV4ADDRESS || received_message->atyp == IPV6ADDRESS){
        if(try_connect(key))
            return ADDRESS_CONNECTING;
        else return CLOSING_CONNECTION;
    }
    else return CLOSING_CONNECTION;

}


void request_reading_departure(const unsigned state , struct selector_key * key ){
    if(key!=NULL && key->data !=NULL){
        sock_client *client_data = (sock_client *) key->data;
        close_sock_request_parser(client_data->using_parser);
        selector_set_interest_key(key,OP_NOOP);
    }
}


static bool try_connect( struct selector_key * key){
    if(key == NULL || key->data == NULL )
        return false;
    sock_client * client_information  = (sock_client*) key->data;
    if(client_information->parsed_message == NULL)
        return false;

    process_request_message((struct sock_request_message *) client_information->parsed_message,key);

    int server_socket_fd;
    if(client_information->origin_address_length == IPV4SIZE){
        server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if(server_socket_fd < 0 )
            return false;
        selector_fd_set_nio(server_socket_fd);

        client_information->origin_fd = server_socket_fd;
        connect(server_socket_fd, (struct sockaddr *) (client_information->origin_address), sizeof(struct sockaddr_in));
        selector_register(key->s, server_socket_fd, &socks5_handler, OP_WRITE, client_information);
    }
    else if(client_information->origin_address_length == IPV6SIZE){
        server_socket_fd = socket(AF_INET6, SOCK_STREAM, 0);
        if(server_socket_fd < 0 )
            return false;
        selector_fd_set_nio(server_socket_fd);

        client_information->origin_fd = server_socket_fd;
        connect(server_socket_fd, (struct sockaddr *) (client_information->origin_address), sizeof(struct sockaddr_in6));
        selector_register(key->s, server_socket_fd, &socks5_handler, OP_WRITE, client_information);
    }
    return true;
}