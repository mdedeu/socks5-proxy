#include "selector.h"
#include "sock_hello_parser.c"

void process_hello_message(struct sock_hello_message data, struct selector_key * key);
void process_authentication_message(struct sock_authentication_message data, struct selector_key * key);
void process_request_message(struct sock_request_message data, struct selector_key * key);