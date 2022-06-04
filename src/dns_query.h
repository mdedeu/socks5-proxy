#include "selector.h"
#include "pthread.h"
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include "sock_client.h"

void * request_resolving_blocking(void * data );
