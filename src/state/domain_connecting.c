#include "domain_connecting.h"

static bool try_one_ip_address(struct selector_key * key);


void domain_connecting_arrival(unsigned state, struct selector_key * key){
    struct sock_client * client_information = (struct sock_client *) key->data;
    client_information->close_after_write  = !try_one_ip_address(key);
}
unsigned domain_connecting_write_handler(struct selector_key * key){
    if(key == NULL || key->data == NULL )
        return CLOSING_CONNECTION;
    struct sock_client * client_information = (struct sock_client *) key->data;
    struct sock_request_message * client_message = (struct sock_request_message*) client_information->parsed_message;

    if(client_information->close_after_write ) {
        return SOCK_NEGATIVE_REQUEST_WRITING; //connect has already set the error code.
    }

    int error ;
    socklen_t  length = sizeof (error);

    if ( getsockopt(client_information->origin_fd, SOL_SOCKET, SO_ERROR, &error, &length) < 0 ) {
        client_message->connection_result = status_general_socks_server_failure;
        return SOCK_NEGATIVE_REQUEST_WRITING;
    }

    if(error != 0){
        if(client_information->current_origin_resolution!= NULL &&client_information->current_origin_resolution->ai_next != NULL){
            selector_unregister_fd(key->s,client_information->origin_fd);
            close(client_information->origin_fd);
            client_information->origin_fd = -1;
            try_one_ip_address(key);
            return DOMAIN_CONNECTING;
        }else {
            client_message->connection_result = errno_to_sock(error);
            return SOCK_NEGATIVE_REQUEST_WRITING;
        }
    }

    return SOCK_REQUEST_WRITING;
}

static bool try_one_ip_address(struct selector_key * key){
    if(key == NULL || key->data ==NULL)
        return false;
    sock_client * client_information = (sock_client *) key->data ;
    struct addrinfo * current = client_information->current_origin_resolution == NULL ? client_information->origin_resolutions
                                                                                                                                                            : client_information->current_origin_resolution;
    if(current == NULL )
        return false;

    client_information->origin_fd = socket(current->ai_family, current->ai_socktype, current->ai_protocol);
    if(client_information->origin_fd < 0 )
        return  false;

    if(SELECTOR_SUCCESS !=selector_fd_set_nio(client_information->origin_fd))
        return false;

    struct sock_request_message * client_message = (struct sock_request_message * )client_information->parsed_message;
    connect(client_information->origin_fd, current->ai_addr, current->ai_addrlen);
    switch (errno) {
        case ECONNREFUSED:
            client_message->connection_result = status_connection_refused;
            return false;
        case EHOSTUNREACH:
            client_message->connection_result = status_host_unreachable;
            return false;
        case ENETUNREACH:
            client_message->connection_result = status_network_unreachable;
            return false;
        case EINPROGRESS:
            if(SELECTOR_SUCCESS == selector_register(key->s, client_information->origin_fd, &socks5_handler, OP_WRITE, client_information))
                return true;
        default:
            client_message->connection_result = status_general_socks_server_failure;
            return false;
    }
}