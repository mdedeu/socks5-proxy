#ifndef GENERAL_HANDLERS_H
#define GENERAL_HANDLERS_H

#include "sock_client.h"
#include <stddef.h>

void socks5_read(struct selector_key * key);
void socks5_write(struct selector_key * key);
void socks5_block(struct selector_key * key);
void socks5_close(struct selector_key * key);
void socks5_timeout(struct selector_key * key);

static const struct fd_handler socks5_handler = {
        .handle_read = &socks5_read,
        .handle_write = &socks5_write,
        .handle_block = &socks5_block,
        .handle_timeout = &socks5_timeout,
        .handle_close= NULL
};

#endif
