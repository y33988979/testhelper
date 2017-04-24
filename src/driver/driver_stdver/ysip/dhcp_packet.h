/*************************************************************************
* Copyright(c) 2009, xxxcorporation
* All rights reserved

* dhcp_socket.c : dhcp socket layer implement

* version: 	1.0
* author: 	ywg
* time:	2009-02-07

* history:	none
**************************************************************************/

#ifndef _DHCP_PACKET_H_
#define _DHCP_PACKET_H_

#include "dhcp_config.h"
#include "dhcp_type.h"
#include "dhcp_socket.h"

struct udp_dhcp_packet {
	struct iphdr ip;
	struct udphdr udp;
	u8_t data[1024]; /* max packet size */
}ATTRIBUTE_PACKED;

int read_interface(char *interface, int *ifindex, u32_t *addr, u8_t *arp);
SOCKET dhcp_open_raw_socket(int ifindex);
int dhcp_get_raw_packet(u8_t *payload, int fd);
int dhcp_send_raw_packet(u8_t *payload, int length, u32_t source_ip, int source_port,
		   u32_t dest_ip, int dest_port, u8_t *dest_arp, int ifindex);

#endif /* _DHCP_PACKET_H_ */

