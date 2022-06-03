#include <stdint.h>
#define BUFFER_SIZE 2048

typedef struct sock_client{
    uint8_t client_fd;
    uint8_t raw_read_buffer[BUFFER_SIZE];
    uint8_t raw_write_buffer[BUFFER_SIZE];
    buffer * read_buffer;
    buffer * write_buffer;
    struct state_machine * client_state_machine;
    union{
        struct sock_hello_message  * hello_message;
        struct sock_request_message *  request_message;
        struct sock_authentication_message * authentication_message;
    } current_parser;
}
sock_client;


struct sock_client * init_new_client_connection(int fd);

void destroy_sock_client(struct sock_client * sock_client);