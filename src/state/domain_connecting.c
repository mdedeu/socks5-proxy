#include "domain_connecting.h"

static void try_one_ip_address(struct selector_key * key);


void domain_connecting_arrival(unsigned state, struct selector_key * key){
    try_one_ip_address(key);
}
unsigned domain_connecting_write_handler(struct selector_key * key){
    struct sock_client * client_information = (struct sock_client *) key->data;
    getsockopt(client_information->origin_fd, SOL_SOCKET, SO_ERROR, NULL, 0);

    if(errno == ENOTCONN){
        //PREG_SALTA: cuanto vale current_origin_resolution si nos mandaron directo la ip?
        if(client_information->current_origin_resolution->ai_next != NULL){
            selector_unregister_fd(key->s, client_information->origin_fd); //todo: implement close on ready_to_connect
            close(client_information->origin_fd);
            try_one_ip_address(key);
            return DOMAIN_CONNECTING;
        }
        return CLOSING_CONNECTION; //respuesta negativa
    }

    return SOCK_REQUEST_WRITING;
}

static void try_one_ip_address(struct selector_key * key){
    sock_client * client_information = (sock_client *) key->data ;

    struct addrinfo * current = client_information->current_origin_resolution == NULL ? client_information->origin_resolutions : client_information->current_origin_resolution;

    client_information->origin_fd = socket(current->ai_family, current->ai_socktype, current->ai_protocol);
    selector_fd_set_nio(client_information->origin_fd);

    connect(client_information->origin_fd, current->ai_addr, current->ai_addrlen);
    selector_register(key->s, client_information->origin_fd, &socks5_handler, OP_WRITE, client_information);
}