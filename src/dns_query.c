#include "dns_query.h"

void * request_resolving_blocking(void * data){
    struct selector_key * key = (struct selector_key *) data;
    struct sock_client * sock_client_information = (struct sock_client *) key->data;
    struct sock_request_message *  request_message = (struct sock_request_message *)sock_client_information->parsed_message;


    pthread_detach(pthread_self());

    struct addrinfo hints = {
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM,
        .ai_flags = AI_PASSIVE,
        .ai_protocol = 0,
        .ai_canonname = NULL,
        .ai_addr = NULL,
        .ai_next = NULL,
        .ai_addrlen=0,
    };

    uint16_t  port = 0 ;
    port = request_message->port[0];
    port <<= 8;
    port += request_message->port[1];

    char buff[16];
    snprintf(buff, sizeof (buff), "%d", port);

    getaddrinfo(
        (char *) request_message->address,
        buff,
        &hints,
        &sock_client_information->origin_resolutions
    ) ;


    selector_notify_block(key->s, key->fd);
    free(data);

    return 0;
}
