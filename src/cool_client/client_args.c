#include "client_args.h"
#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <limits.h>    /* LONG_MIN et al */
#include <string.h>    /* memset */
#include <errno.h>
#include <getopt.h>


static unsigned short
port(const char *s) {
    char *end     = 0;
    const long sl = strtol(s, &end, 10);

    if (end == s|| '\0' != *end
        || ((LONG_MIN == sl || LONG_MAX == sl) && ERANGE == errno)
        || sl < 0 || sl > USHRT_MAX) {
        fprintf(stderr, "port should in in the range of 1-65536: %s\n", s);
        exit(1);
        return 1;
    }
    return (unsigned short)sl;
}





static void
usage(const char *progname) {
    fprintf(stderr,
            "Usage: %s [OPTION]...\n"
            "\n"
            "   -h               Print some help information.\n"
            "   -L              Specify address to connect.\n"
            "   -P              Specify port to connect.\n"
            "   [-4 | -6]              Specify which ip version use.\n"
            "\n",
            progname);
    exit(1);
}

void
client_parse_args(const int argc, char **argv, struct client_args *args) {
    memset(args, 0, sizeof(*args));

    args->socks_addr = "127.0.0.1";
    args->socks_port = 8080;


    int c;

    while (true) {
        int option_index = 0;

        c = getopt_long(argc, argv, "hL:P:46", NULL, &option_index);
        if (c == -1)
            break;

        switch (c) {
            case 'h':
                usage(argv[0]);
                break;
            case 'L':
                args->socks_addr = optarg;
                break;
            case 'P':
                args->socks_port = port(optarg);
                break;
            case '4':
                args->version=4;
                break;
            case '6':
                args->version=6;
                break;
            default:
                fprintf(stderr, "unknown argument %d.\n", c);
                exit(1);
        }
    }
    if (optind < argc) {
        fprintf(stderr, "argument not accepted: ");
        while (optind < argc) {
            fprintf(stderr, "%s ", argv[optind++]);
        }
        fprintf(stderr, "\n");
        exit(1);
    }
}
