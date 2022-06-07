#include <stdio.h>
#include <stdlib.h>
#include <check.h>

#include "../parser.h"

enum states{
	INIT_STATE,
	ERROR,
	ULEN_READ,
	UREAD,
	PASSLEN_READ,
	PASS_READ,
	AUTH_OK
};

enum event_types{


};