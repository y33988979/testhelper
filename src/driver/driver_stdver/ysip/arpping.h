/*************************************************************************
* Copyright(c) 2009, xxxcorporation
* All rights reserved

* arpping.h : arp request test head file

* version: 	1.0
* author: 	ywg
* time:	2009-02-07

* history:	none
**************************************************************************/

#ifndef _ARPPING_H_
#define _ARPPING_H_

#include <netinet/if_ether.h>
#include <net/if_arp.h>
//#include <net/if.h>
#include <netinet/in.h>

//#include "dhcp_config.h"
#include "dhcp_type.h"

#define MAC_BCAST_ADDR		(u8_t *) "\xff\xff\xff\xff\xff\xff"

struct arpMsg {
    /* Ethernet header */
    u8_t   h_dest[6];           /* destination ether addr */
    u8_t   h_source[6];         /* source ether addr */
    u16_t  h_proto;             /* packet type ID field */

    /* ARP packet */
    u16_t  htype;               /* hardware type (must be ARPHRD_ETHER) */
    u16_t  ptype;               /* protocol type (must be ETH_P_IP) */
    u8_t   hlen;                /* hardware address length (must be 6) */
    u8_t   plen;                /* protocol address length (must be 4) */
    u16_t  operation;           /* ARP opcode */
    u8_t   sHaddr[6];           /* sender's hardware address */
    u8_t   sInaddr[4];          /* sender's IP address */
    u8_t   tHaddr[6];           /* target's hardware address */
    u8_t   tInaddr[4];          /* target's IP address */
    u8_t   pad[18];	            /* pad for min. Ethernet payload (60 bytes) */
} ATTRIBUTE_PACKED;

/* function prototypes */
int arpping(u32_t yiaddr, u32_t ip, u8_t *arp, char *interface);

#endif /* _ARPPING_H_ */

