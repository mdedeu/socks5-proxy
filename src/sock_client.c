#include "sock_client.h"

struct sock_client * init_new_client_connection(int fd,struct sockaddr * client_information){
    sock_client  * new_client = calloc(1, sizeof (sock_client));
    buffer * temp_write;
    buffer * temp_read;
    if(new_client != NULL){
        new_client->client_fd = fd;
        new_client->client_state_machine = init_proxy_state_machine();
         temp_write = calloc(1, sizeof (buffer));
         temp_read = calloc(1, sizeof (buffer));
        if(temp_write == NULL || temp_read == NULL )
            goto free_client;
        buffer_init(temp_write,BUFFER_SIZE,new_client->raw_write_buffer);
        buffer_init(temp_read,BUFFER_SIZE,new_client->raw_read_buffer);
        new_client->read_buffer = temp_read;
        new_client->write_buffer = temp_write;
        new_client->origin_fd = -1;
        new_client->origin_address_length=-1;
        new_client->close_after_write = false;
        new_client->origin_resolutions = NULL ;
        new_client->current_origin_resolution = NULL;
        new_client->client_information = malloc(sizeof (struct sockaddr));
        if(new_client->client_information == NULL )
            goto free_client;
        memcpy(new_client->client_information,client_information, sizeof(struct sockaddr));
    }

    return new_client;

    free_client:
        free(new_client);
        if(temp_write != NULL) free(temp_write);
        if(temp_read != NULL ) free(temp_read);
        return NULL;
}

void destroy_sock_client(struct sock_client * sock_client){
    if(sock_client->origin_resolutions != NULL)   freeaddrinfo(sock_client->origin_resolutions);
    else free(sock_client->origin_address);
    free(sock_client->write_buffer);
    free(sock_client->read_buffer);
    destroy_sock_state(sock_client->client_state_machine);
    free(sock_client);
}