#include "ready_to_connect.h"

void ready_to_connect_on_arrival(unsigned state, struct selector_key * key){
    sock_client * client_information  = (sock_client*) key->data;

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


unsigned ready_to_connect_block_handler(struct selector_key * key){
    sock_client * client_information = (sock_client *) key->data ;
    struct addrinfo * current = client_information->current_origin_resolution == NULL ? client_information->origin_resolutions : client_information->current_origin_resolution;

    client_information->origin_fd = socket(current->ai_family, current->ai_socktype, current->ai_protocol);
    selector_fd_set_nio(client_information->origin_fd);

    connect(client_information->origin_fd, current->ai_addr, current->ai_addrlen);
    selector_register(key->s, client_information->origin_fd, &socks5_handler, OP_WRITE, client_information);

    return READY_TO_CONNECT;
}

unsigned ready_to_connect_write_handle(struct selector_key * key){
    struct sock_client * client_information = (struct sock_client *) key->data;
    getsockopt(client_information->origin_fd, SOL_SOCKET, SO_ERROR, NULL, 0);

    if(errno == ENOTCONN){
        //PREG_SALTA: cuanto vale current_origin_resolution si nos mandaron directo la ip?
        if(client_information->current_origin_resolution->ai_next != NULL){
            selector_unregister_fd(key->s, client_information->origin_fd); //todo: implement close on ready_to_connect
            close(client_information->origin_fd);
            return ready_to_connect_block_handler(key);
        }
        return CLOSING_CONNECTION;
    }
    return WRITING_REPLY;
}
