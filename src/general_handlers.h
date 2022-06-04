#ifndef
#include "selector.h"
#include "general_handlers.h"
#include "sock_client.h"
#include "proxy_state_machine.h"
static const struct fd_handler socks5_handler = {
        .handle_read = socks5_read,
        .handle_write = socks5_write,
        .handle_block = socks5_block,
        .handle_close= socks5_close,

};


void socks5_read(struct selector_key * key);
void socks5_write(struct selector_key * key);
void socks5_block(struct selector_key * key);
void socks5_close(struct selector_key * key);
#endif
