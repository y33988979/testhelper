/*************************************************************************
* Copyright(c) 2009, xxxcorporation
* All rights reserved

* congif.h: ≈‰÷√∫Í∂®“Â

* version: 	1.0
* author: 	ywg
* time:	2009-01-19

* history:	none
**************************************************************************/

#ifndef _DHCP_CONFIG_H_
#define _DHCP_CONFIG_H_

//#define __WIN32__
#if defined(__WIN32__)
#include <Winsock2.h>
#include <stdio.h>
#include <windows.h>

#endif

#define __LINUX_SYS__
#define __LINUX__
#if defined(__LINUX__)
/* system */
#include "stdio.h"
#include "string.h"
#include "sys/types.h"
#include "sys/time.h"
#include "sys/stat.h"
#include "sys/ipc.h"
#include "fcntl.h"
#include "signal.h"
#include "unistd.h"
#include "time.h"
#include "pthread.h"
#include <sys/ioctl.h>
#include <ctype.h>
#include <errno.h>
//#include <asm/types.h>
/* socket */
#include "sys/socket.h"
#include "netinet/in.h"
#include "netdb.h"

#include <arpa/inet.h>
#include <net/if_arp.h>
#include <netinet/ether.h>
#include <linux/route.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/tcp.h>

#include <netinet/udp.h>
#include <netinet/ip.h>

#if (__GLIBC__ >= 2 && __GLIBC_MINOR >= 1) || defined _NEWLIB_VERSION
#include <netpacket/packet.h>
#include <net/ethernet.h>
#else
#include <asm/types.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#endif

/* msg queue */
#include "sys/msg.h"

/* semaphore */
#include "sys/sem.h"

#define ATTRIBUTE_PACKED __attribute__ ((__packed__))
#define DHCP_DEBUG printf

#endif

//#define __STSYSTEM__
#if defined(__STSYSTEM__)
#include <sttbx.h>
#include <stlite.h>
#include <stbtype.h>
#include <partitio.h>
/* tcp ip socket */
//#include "tcpip/sockets.h"
//#include "tcpip/lwipsock.h"

#endif

#define NORMAL_SOCKET_MODE  1
#define RAW_SOCKET_MODE     1

#endif /* _DHCP_CONFIG_H_ */

