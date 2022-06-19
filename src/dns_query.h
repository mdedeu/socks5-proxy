#ifndef  DNS_QUERY_H
#define  DNS_QUERY_H

#include "pthread.h"
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include "./state/proxy_state_machine.h"


 void * request_resolving_blocking(void * data ) ;

#endif

