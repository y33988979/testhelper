/*************************************************************************
* Copyright(c) 2009, xxxcorporation
* All rights reserved

* dhcp_socket.h : dhcp socket layer head file

* version: 	1.0
* author: 	ywg
* time:	2009-02-07

* history:	none
**************************************************************************/

#ifndef _DHCP_SOCKET_H_
#define _DHCP_SOCKET_H_

#include "dhcp_config.h"
#include "dhcp_type.h"

#ifndef SOCKET 
typedef int     SOCKET;
#endif

#ifndef ip_addr
typedef struct ip_addr
{
    u32_t addr;
};
#endif

SOCKET dhcp_socket_register(void);

int dhcp_socket_send(SOCKET sock, char *sendbuf, int sendsize, int sendflag, 
                            struct sockaddr *remote_addr, int socksize);

int dhcp_socket_recv(SOCKET sock, char *recvbuf, int recvsize, int recvflag);

#endif /* _DHCP_SOCKET_H_ */

