#include "dns_query.h"

 void * request_resolving_blocking(void * data ) {
    struct selector_key * key = (struct selector_key*) data;
    struct sock_client *  sock_client_information  = ( struct sock_client * ) key ->data;
    struct sock_request_message * request_message = sock_client_information->current_parser.request_message;

    pthread_detach(pthread_self());
    struct addrinfo hints = {
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM,
        .ai_flags = AI_PASSIVE,
        .ai_protocol = 0,
        .ai_canonname = NULL,
        .ai_addr  =NULL,
        .ai_next = NULL,
    };
    char buff[7];
    snprintf(buff, sizeof (buff),"%d", ntohs((uint16_t)*request_message->port));
    getaddrinfo((char * )sock_client_information->current_parser.request_message->address,buff,&hints,&sock_client_information->origin_resolutions);
    selector_notify_block(key->s, key->fd);
    free(data);
    return 0;
}