#include "address_connecting.h"

void address_connecting_arrival(unsigned state, struct selector_key * key){
    sock_client * client_information  = (sock_client*) key->data;
    process_request_message((struct sock_request_message *) client_information->parsed_message,key);

    int server_socket_fd;
    if(client_information->origin_address_length == IPV4SIZE){
        server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        selector_fd_set_nio(server_socket_fd);

        client_information->origin_fd = server_socket_fd;
        connect(server_socket_fd, (struct sockaddr *) (client_information->origin_address), sizeof(struct sockaddr_in));
        selector_register(key->s, server_socket_fd, &socks5_handler, OP_WRITE, client_information);
    }
    else if(client_information->origin_address_length == IPV6SIZE){
        server_socket_fd = socket(AF_INET6, SOCK_STREAM, 0);
        selector_fd_set_nio(server_socket_fd);

        client_information->origin_fd = server_socket_fd;
        connect(server_socket_fd, (struct sockaddr *) (client_information->origin_address), sizeof(struct sockaddr_in6));
        selector_register(key->s, server_socket_fd, &socks5_handler, OP_WRITE, client_information);
    }
}


unsigned address_connecting_write_handler(struct selector_key * key){
    struct sock_client * client_information = (struct sock_client *) key->data;
    getsockopt(client_information->origin_fd, SOL_SOCKET, SO_ERROR, NULL, 0);

    if(errno == ENOTCONN){
        //no more attempts, save the error cause
        return CLOSING_CONNECTION;
    }
    return SOCK_REQUEST_WRITING;
}

void address_connecting_departure(struct selector_key * key){
    struct sock_client * client_information = (struct sock_client *) key->data;
    selector_set_interest(key->s,client_information->origin_fd , OP_NOOP);
    selector_set_interest(key->s,client_information->client_fd,OP_NOOP);
}