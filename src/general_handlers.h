#ifndef GENERAL_DANDLERS_H
#define GENERAL_DANDLERS_H

#include "selector.h"
#include "general_handlers.h"
#include "sock_client.h"
#include "state/proxy_state_machine.h"
#include "client_request_processor.h"
#include <stddef.h>

void socks5_read(struct selector_key * key);
void socks5_write(struct selector_key * key);
void socks5_block(struct selector_key * key);
void socks5_close(struct selector_key * key);

static const struct fd_handler socks5_handler = {
        .handle_read = &socks5_read,
        .handle_write = &socks5_write,
        .handle_block = &socks5_block,
        .handle_close= &socks5_close,
};

#endif
