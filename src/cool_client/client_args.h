//
// Created by shadad on 19/06/22.
//

#ifndef SOCKS5_PROXY_CLIENT_ARGS_H
#define SOCKS5_PROXY_CLIENT_ARGS_H
#include <stdbool.h>



struct client_args {
    char           *socks_addr;
    unsigned short  socks_port;
    int version;

};

/**
 * Interpreta la linea de comandos (argc, argv) llenando
 * args con defaults o la seleccion humana. Puede cortar
 * la ejecuciÃ³n.
 */
void
client_parse_args(const int argc, char **argv, struct client_args *args);


#endif //SOCKS5_PROXY_CLIENT_ARGS_H
