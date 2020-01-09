#ifndef __T_NET_H__
#define __T_NET_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

extern int sock_bind(int domain, int type, int port);

#endif //__T_NET_H__

