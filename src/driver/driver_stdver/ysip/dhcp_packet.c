/*************************************************************************
* Copyright(c) 2009, xxxcorporation
* All rights reserved

* dhcp_socket.c : dhcp socket layer implement

* version: 	1.0
* author: 	ywg
* time:	2009-02-07

* history:	none
**************************************************************************/

#include "dhcp_config.h"
#include "dhcp_type.h"
#include "dhcp_packet.h"
#include "dhcp_client.h"
#include "dhcp_socket.h"

u16_t dhcp_checksum(void *addr, int count);

/* 
   get if info by raw socket 
   -1  error
   0   ok
*/
int read_interface(char *interface, int *ifindex, u32_t *addr, u8_t *arp)
{
	int fd;
	struct ifreq ifr;
	struct sockaddr_in *our_ip;

	memset(&ifr, 0, sizeof(struct ifreq));
	if((fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) >= 0) {
		ifr.ifr_addr.sa_family = AF_INET;
		strcpy(ifr.ifr_name, interface);

		if (addr) {
			if (ioctl(fd, SIOCGIFADDR, &ifr) == 0) {
				our_ip = (struct sockaddr_in *) &ifr.ifr_addr;
				*addr = our_ip->sin_addr.s_addr;
				DHCP_DEBUG("%s (our ip) = %s \n", ifr.ifr_name, inet_ntoa(our_ip->sin_addr));
			} else {
				DHCP_DEBUG("SIOCGIFADDR failed, is the interface up and configured?: %m \n");
				close(fd);
				return -1;
			}
		}

		if (ioctl(fd, SIOCGIFINDEX, &ifr) == 0) {
			DHCP_DEBUG("adapter index %d \n", ifr.ifr_ifindex);
			*ifindex = ifr.ifr_ifindex;
		} else {
			DHCP_DEBUG("SIOCGIFINDEX failed!: %m \n");
			close(fd);
			return -1;
		}
		if (ioctl(fd, SIOCGIFHWADDR, &ifr) == 0) {
			memcpy(arp, ifr.ifr_hwaddr.sa_data, 6);
			DHCP_DEBUG("adapter hardware address %02x:%02x:%02x:%02x:%02x:%02x \n",
				arp[0], arp[1], arp[2], arp[3], arp[4], arp[5]);
		} else {
			DHCP_DEBUG("SIOCGIFHWADDR failed!: %m \n");
			close(fd);
			return -1;
		}
	} else {
		DHCP_DEBUG("socket failed!: %m \n");
		return -1;
	}
	close(fd);
	return 0;
}

SOCKET dhcp_open_raw_socket(int ifindex)
{
	int fd;
	struct sockaddr_ll sock;

	DHCP_DEBUG("Opening raw socket on ifindex %d \n", ifindex);
	if ((fd = socket(PF_PACKET, SOCK_DGRAM, htons(ETH_P_IP))) < 0) {
		DHCP_DEBUG("open socket call failed: %m \n");
		return -1;
	}

	sock.sll_family = AF_PACKET;
	sock.sll_protocol = htons(ETH_P_IP);
	sock.sll_ifindex = ifindex;
	if (bind(fd, (struct sockaddr *) &sock, sizeof(sock)) < 0) {
		DHCP_DEBUG("bind call failed: %m \n");
		close(fd);
		return -1;
	}

	return fd;
}

#if 0
void udhcp_init_header(struct dhcpMessage *packet, char type)
{
	memset(packet, 0, sizeof(struct dhcpMessage));
	switch (type) {
	case DHCPDISCOVER:
	case DHCPREQUEST:
	case DHCPRELEASE:
	case DHCPINFORM:
		packet->op = BOOTREQUEST;
		break;
	case DHCPOFFER:
	case DHCPACK:
	case DHCPNAK:
		packet->op = BOOTREPLY;
	}
	packet->htype = ETH_10MB;
	packet->hlen = ETH_10MB_LEN;
	packet->cookie = htonl(DHCP_MAGIC);
	packet->options[0] = DHCP_END;
	add_simple_option(packet->options, DHCP_MESSAGE_TYPE, type);
}
#endif

/* return -1 on errors that are fatal for the socket, -2 for those that aren't */
int dhcp_get_raw_packet(u8_t *payload, int fd)
{
        int i;
        u8_t *pdata;
	int bytes;
	struct udp_dhcp_packet packet;
	u32_t source, dest;
	u16_t check;
    
	memset(&packet, 0, sizeof(struct udp_dhcp_packet));
	bytes = read(fd, &packet, sizeof(struct udp_dhcp_packet));
	if (bytes < 0) {
		DHCP_DEBUG("couldn't read on raw listening socket -- ignoring \n");
		usleep(500000); /* possible down interface, looping condition */
		return -1;
	}
    
	if (bytes < (int) (sizeof(struct iphdr) + sizeof(struct udphdr))) {
		DHCP_DEBUG("message too short, ignoring \n");
		return -2;
	}
    
	if (bytes < ntohs(packet.ip.tot_len)) {
		//DHCP_DEBUG("Truncated packet %d  <  %d total %d\n", bytes, ntohs(packet.ip.tot_len), sizeof(struct udp_dhcp_packet));
		//return -2;
        #if 0
                pdata = (u8_t *)&packet;
                DHCP_DEBUG("packet is \n");
                for(i=0; i<bytes; i++){
                    if((i%16) == 0){
                        printf("\n");
                    }
                    DHCP_DEBUG("%02X    ",*(pdata + i));
                }
         #endif
                return -3;
	}
    
	/* ignore any extra garbage bytes */
	bytes = ntohs(packet.ip.tot_len);
	//DHCP_DEBUG("recv....[%d]?=[%d] \n",bytes,sizeof(struct udp_dhcp_packet));

	/* Make sure its the right packet for us, and that it passes sanity checks */
	if (packet.ip.protocol != IPPROTO_UDP || packet.ip.version != IPVERSION ||
	    packet.ip.ihl != sizeof(packet.ip) >> 2 || packet.udp.dest != htons(DHCP_CLIENT_PORT) ||
	    bytes > (int) sizeof(struct udp_dhcp_packet) ||
	    ntohs(packet.udp.len) != (u16_t) (bytes - sizeof(packet.ip))) {
		//DHCP_DEBUG("unrelated/bogus packet \n");
		return -3;
	}
	/* check IP checksum */
	check = packet.ip.check;
	packet.ip.check = 0;
	if (check != dhcp_checksum(&(packet.ip), sizeof(packet.ip))) {
		DHCP_DEBUG("bad IP header checksum, ignoring \n");
		return -1;
	}
	/* verify the UDP checksum by replacing the header with a psuedo header */
	source = packet.ip.saddr;
	dest = packet.ip.daddr;
	check = packet.udp.check;
	packet.udp.check = 0;
	memset(&packet.ip, 0, sizeof(packet.ip));

	packet.ip.protocol = IPPROTO_UDP;
	packet.ip.saddr = source;
	packet.ip.daddr = dest;
	packet.ip.tot_len = packet.udp.len; /* cheat on the psuedo-header */
	if (check && check != dhcp_checksum(&packet, bytes)) {
		DHCP_DEBUG("packet with bad UDP checksum received, ignoring \n");
		return -2;
	}
      
	memcpy(payload, &(packet.data), bytes - (sizeof(packet.ip) + sizeof(packet.udp)));
	//if (ntohl(payload->cookie) != DHCP_MAGIC) {
	//	DHCP_DEBUG("received bogus message (bad magic) -- ignoring");
	//	return -2;
	//}
	//DHCP_DEBUG("oooooh!!! got some!bytes%d - s-ip%d - s-udp%d %d\n",bytes, sizeof(packet.ip),sizeof(packet.udp),(bytes - (sizeof(packet.ip) + sizeof(packet.udp))));
	return (bytes - (sizeof(packet.ip) + sizeof(packet.udp)));

}

#if 0
/* read a packet from socket fd, return -1 on read error, -2 on packet error */
int dhcp_get_raw_packet_ex(struct dhcpMessage *packet, int fd)
{
	static const char broken_vendors[][8] = {
		"MSFT 98",
		""
	};
	int bytes;
	int i;
	char unsigned *vendor;

	memset(packet, 0, sizeof(struct dhcpMessage));
	bytes = read(fd, packet, sizeof(struct dhcpMessage));
	if (bytes < 0) {
		DHCP_DEBUG("couldn't read on listening socket, ignoring");
		return -1;
	}

	if (ntohl(packet->cookie) != DHCP_MAGIC) {
		DHCP_DEBUG("received bogus message, ignoring");
		return -2;
	}
	DHCP_DEBUG("Received a packet");

	if (packet->op == BOOTREQUEST && (vendor = get_option(packet, DHCP_VENDOR))) {
		for (i = 0; broken_vendors[i][0]; i++) {
			if (vendor[OPT_LEN - 2] == (uint8_t) strlen(broken_vendors[i]) &&
			    !strncmp((char*)vendor, broken_vendors[i], vendor[OPT_LEN - 2])) {
				DEBUG(LOG_INFO, "broken client (%s), forcing broadcast",
					broken_vendors[i]);
				packet->flags |= htons(BROADCAST_FLAG);
			}
		}
	}

	return bytes;
}
#endif

#if 0
/* Let the kernel do all the work for packet generation */
int udhcp_kernel_packet(struct dhcpMessage *payload, uint32_t source_ip, int source_port,
		   uint32_t dest_ip, int dest_port)
{
	int n = 1;
	int fd, result;
	struct sockaddr_in client;

	if ((fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		return -1;

	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *) &n, sizeof(n)) == -1) {
		close(fd);
		return -1;
	}

	memset(&client, 0, sizeof(client));
	client.sin_family = AF_INET;
	client.sin_port = htons(source_port);
	client.sin_addr.s_addr = source_ip;

	if (bind(fd, (struct sockaddr *)&client, sizeof(struct sockaddr)) == -1) {
		close(fd);
		return -1;
	}

	memset(&client, 0, sizeof(client));
	client.sin_family = AF_INET;
	client.sin_port = htons(dest_port);
	client.sin_addr.s_addr = dest_ip;

	if (connect(fd, (struct sockaddr *)&client, sizeof(struct sockaddr)) == -1) {
		close(fd);
		return -1;
	}

	result = write(fd, payload, sizeof(struct dhcpMessage));
	close(fd);
	return result;
}
#endif

/* Construct a ip/udp header for a packet, and specify the source and dest hardware address */
int dhcp_send_raw_packet(u8_t *payload, int length, u32_t source_ip, int source_port,
		   u32_t dest_ip, int dest_port, u8_t *dest_arp, int ifindex)
{
#if 0
{
	int fd;
	int result;
	struct sockaddr_ll dest;
	struct udp_dhcp_packet packet;

    int iplen,udplen;

	if ((fd = socket(PF_PACKET, SOCK_DGRAM, ipcfg_htons(ETH_P_IP))) < 0) {
		DHCP_DEBUG("socket call failed: %m \n");
		return -1;
	}

	memset(&dest, 0, sizeof(dest));
	memset(&packet, 0, sizeof(packet));

	dest.sll_family = AF_PACKET;
	dest.sll_protocol = ipcfg_htons(ETH_P_IP);
	dest.sll_ifindex = ifindex;
	dest.sll_halen = 6;
	memcpy(dest.sll_addr, dest_arp, 6);
	if (bind(fd, (struct sockaddr *)&dest, sizeof(struct sockaddr_ll)) < 0) {
		DHCP_DEBUG("bind call failed: %m \n");
		close(fd);
		return -1;
	}

    iplen = sizeof(packet.ip);
    udplen = sizeof(packet.udp);
    
	packet.ip.protocol = IPPROTO_UDP;
	packet.ip.saddr = source_ip;
	packet.ip.daddr = dest_ip;
	packet.udp.source = ipcfg_htons(source_port);
	packet.udp.dest = ipcfg_htons(dest_port);
	packet.udp.len = ipcfg_htons(udplen + length); /* cheat on the psuedo-header */
	packet.ip.tot_len = packet.udp.len;
      memcpy(&(packet.data), &(packet.ip), iplen);
      memcpy(&(packet.data)+iplen, &(packet.udp), udplen);
	memcpy(&(packet.data)+iplen+udplen, payload, length);
	packet.udp.check = dhcp_checksum(&packet.data, iplen+udplen+length);
    
	packet.ip.tot_len = ipcfg_htons(iplen+udplen+length);
	packet.ip.ihl = sizeof(packet.ip) >> 2;
	packet.ip.version = IPVERSION;
	packet.ip.ttl = IPDEFTTL;
	packet.ip.check = dhcp_checksum(&(packet.ip), iplen);
      memcpy(&(packet.data), &(packet.ip), iplen);
    
	result = sendto(fd, &packet.data, (iplen+udplen+length), 0, (struct sockaddr *)&dest, sizeof(dest));
	if (result <= 0) {
		DHCP_DEBUG("write on socket failed: %m \n");
	}
	close(fd);
	return result;
}
#endif

#if 1
{
	int fd;
	int result;
	struct sockaddr_ll dest;
	struct udp_dhcp_packet packet;
	int iplen,udplen;

	if ((fd = socket(PF_PACKET, SOCK_DGRAM, htons(ETH_P_IP))) < 0) {
		DHCP_DEBUG("socket call failed: %m \n");
		return -1;
	}

	memset(&dest, 0, sizeof(dest));
	memset(&packet, 0, sizeof(packet));

	dest.sll_family = AF_PACKET;
	dest.sll_protocol = htons(ETH_P_IP);
	dest.sll_ifindex = ifindex;
	dest.sll_halen = 6;
	memcpy(dest.sll_addr, dest_arp, 6);
	if (bind(fd, (struct sockaddr *)&dest, sizeof(struct sockaddr_ll)) < 0) {
		DHCP_DEBUG("bind call failed: %m \n");
		close(fd);
		return -1;
	}

	iplen = sizeof(packet.ip);
	udplen = sizeof(packet.udp);

	packet.ip.protocol = IPPROTO_UDP;
	packet.ip.saddr = source_ip;
	packet.ip.daddr = dest_ip;
	packet.udp.source = htons(source_port);
	packet.udp.dest = htons(dest_port);
	packet.udp.len = htons(sizeof(packet.udp) + length); /* cheat on the psuedo-header */
	packet.ip.tot_len = packet.udp.len;
	memcpy(&(packet.data), payload, length);
	packet.udp.check = dhcp_checksum(&packet, (iplen+udplen+length));

	packet.ip.tot_len = htons((iplen+udplen+length));
	packet.ip.ihl = sizeof(packet.ip) >> 2;
	packet.ip.version = IPVERSION;
	packet.ip.ttl = IPDEFTTL;
	packet.ip.check = dhcp_checksum(&(packet.ip), sizeof(packet.ip));

	result = sendto(fd, &packet, (iplen+udplen+length), 0, (struct sockaddr *) &dest, sizeof(dest));
	if (result <= 0) {
		DHCP_DEBUG("write on socket failed: %m \n");
	}
	close(fd);
	return result;
}
#endif
}

u16_t dhcp_checksum(void *addr, int count)
{
	/* Compute Internet Checksum for "count" bytes
	 *         beginning at location "addr".
	 */
	register int sum = 0;
	u16_t *source = (u16_t *) addr;

	while (count > 1)  {
		/*  This is the inner loop */
		sum += *source++;
		count -= 2;
	}

	/*  Add left-over byte, if any */
	if (count > 0) {
		/* Make sure that the left-over byte is added correctly both
		 * with little and big endian hosts */
		u16_t tmp = 0;
		*(u8_t *) (&tmp) = * (u8_t *) source;
		sum += tmp;
	}
	/*  Fold 32-bit sum to 16 bits */
	while (sum >> 16)
		sum = (sum & 0xffff) + (sum >> 16);

	return ~sum;
}

