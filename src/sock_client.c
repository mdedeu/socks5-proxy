#include "sock_client.h"

struct sock_client * init_new_client_connection(int fd){
    sock_client  * new_client = calloc(1, sizeof (sock_client));
//    if(new_client == NULL)
    new_client->client_fd = fd;
    new_client->client_state_machine = init_proxy_state_machine();
    buffer * temp_write = calloc(1, sizeof (buffer));
    buffer * temp_read = calloc(1, sizeof (buffer));
    buffer_init(temp_write,BUFFER_SIZE,new_client->raw_write_buffer);
    buffer_init(temp_read,BUFFER_SIZE,new_client->raw_read_buffer);
    new_client->read_buffer = temp_read;
    new_client->write_buffer = temp_write;
//    new_client->current_parser.hello_message = init_sock_hello_parser();
    return new_client;
}

void destroy_sock_client(struct sock_client * sock_client){
    free(sock_client->write_buffer);
    free(sock_client->read_buffer);
    destroy_sock_state(sock_client->client_state_machine);
    free(sock_client->origin_address);
    free(sock_client);
}