

#include <rpc/rpc.h>
#include <stdio.h>
#define RPC_CLNT
#include "rfile.h"

extern CLIENT *handle;
static char** ret;


char* fileSend(char* inputt)
{
	char **arg;
	arg = &inputt;
	ret = fileSend_1(arg, handle);
	return ret == NULL? "no data read in the client interface" : *ret;

}
