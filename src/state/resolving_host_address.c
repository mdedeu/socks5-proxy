#include "resolving_host_address.h"

unsigned resolving_host_address_block(struct selector_key *key){
    return DOMAIN_CONNECTING;
}

void  resolving_host_address_arrival(const unsigned state, struct selector_key *key){
    struct selector_key * thread_copy = malloc(sizeof(struct selector_key));
    memcpy(thread_copy, key, sizeof(struct selector_key));
    pthread_t tid;
    pthread_create(&tid, 0, &request_resolving_blocking, thread_copy);
}
