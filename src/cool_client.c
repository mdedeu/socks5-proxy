#include "cool_client.h"

struct cool_client * init_cool_client_connection(int fd){
    struct cool_client  * new_client = calloc(1, sizeof (cool_client));
//    if(new_client == NULL)
    new_client->client_fd = fd;
    new_client->client_state_machine = init_cool_state_machine();
    buffer * temp_write = calloc(1, sizeof (buffer));
    buffer_init(temp_write, BUFFER_SIZE, new_client->raw_write_buffer);
    new_client->write_buffer = temp_write;
//    new_client->current_parser.hello_message = init_cool_hello_parser();
    return new_client;
}

void destroy_cool_client(struct cool_client * cool_client){
    if(cool_client == NULL)
        return;

    if(cool_client->write_buffer)
        free(cool_client->write_buffer);
    destroy_cool_state(cool_client->client_state_machine);
    free(cool_client);
}