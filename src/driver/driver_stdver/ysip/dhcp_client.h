/*************************************************************************
* Copyright(c) 2009, xxxcorporation
* All rights reserved

* dhcp_client.h : dhcp client head file

* version: 	1.0
* author: 	ywg
* time:	2009-02-07

* history:	none
**************************************************************************/

#ifndef _DHCP_CLIENT_H_
#define _DHCP_CLIENT_H_

#include "dhcp_config.h"
#include "dhcp_type.h"
#include "dhcp_socket.h"

#ifndef UINT32BIT
typedef union
{
    unsigned int    uiWord32;
    struct
    {
        unsigned char   ucByte0;        /* LSB */
        unsigned char   ucByte1;
        unsigned char   ucByte2;
        unsigned char   ucByte3;        /* MSB */
    } byte;
} UINT32BIT;
#endif

#ifndef UIN16BIT
typedef union
{
    unsigned  short int     uiWord16;
    struct
    {
        unsigned char   ucByte0;        /* LSB */
        unsigned char   ucByte1;        /* MSB */
    } byte;
} UIN16BIT;
#endif


//////////////////////////////////////////////////////////////////////

/** period (in seconds) of the application calling dhcp_coarse_tmr() */
#define DHCP_COARSE_TIMER_SECS  60 
/** period (in milliseconds) of the application calling dhcp_fine_tmr() */
#define DHCP_FINE_TIMER_MSECS   500 

#define DHCP_MAGIC		0x63825363

#define MAC_BCAST_ADDR		(u8_t *) "\xff\xff\xff\xff\xff\xff"
/*
typedef struct dhcp_msg
{
 char op;
 char htype;
 char hlen;
 char hops;
 char xid[4];
 char secs[2];
 char flags[2];
 char ciaddr[4];
 char yiaddr[4];
 char siaddr[4];
 char giaddr[4];
 char chaddr[16];
 char sname[64];
 char file[128];
 char *option; // variable
}
*/

/** minimum set of fields of any DHCP message */
struct dhcp_msg
{
  u8_t op;
  u8_t htype;
  u8_t hlen;
  u8_t hops;
  u32_t xid;
  u16_t secs;
  u16_t flags;
  struct ip_addr ciaddr;
  struct ip_addr yiaddr;
  struct ip_addr siaddr;
  struct ip_addr giaddr;
#define DHCP_CHADDR_LEN 16U
  u8_t chaddr[DHCP_CHADDR_LEN];
#define DHCP_SNAME_LEN 64U
  u8_t sname[DHCP_SNAME_LEN];
#define DHCP_FILE_LEN 128U
  u8_t file[DHCP_FILE_LEN];
  u32_t cookie;
#define DHCP_MIN_OPTIONS_LEN 68U
#define DHCP_OPTIONS_LEN 512
/** make sure user does not configure this too small */
#if ((defined(DHCP_OPTIONS_LEN)) && (DHCP_OPTIONS_LEN < DHCP_MIN_OPTIONS_LEN))
#undef DHCP_OPTIONS_LEN
#endif
/** allow this to be configured in lwipopts.h, but not too small */
#if (!defined(DHCP_OPTIONS_LEN))
/** set this to be sufficient for your options in outgoing DHCP msgs */
#define DHCP_OPTIONS_LEN DHCP_MIN_OPTIONS_LEN
#endif
  u8_t options[DHCP_OPTIONS_LEN];
};

struct dhcp_info
{
  int sockfd; /* socket fd for dhcp */
  int ifindex; /* interface index */
  u32_t timer_thread_id;
  u32_t mloop_thread_id;
  /** current DHCP state machine state */
  u8_t state;
  u8_t tried;
  /** retries of current request */
  u8_t retries;
  u8_t t2_test;
  int timeout;  /* ms ? */
  /** transaction identifier of last sent request */ 
  u32_t xid;
  /** incoming msg */
  struct dhcp_msg *msg_in;
  /** incoming msg options */
  //struct dhcp_msg *options_in; 
  /** ingoing msg options length */
  u16_t options_in_len;
  struct dhcp_msg *msg_out; /* outgoing msg */
  u16_t options_out_len; /* outgoing msg options length */
  u16_t request_timeout; /* #ticks with period DHCP_FINE_TIMER_SECS for request timeout */
  u16_t t1_timeout;  /* #ticks with period DHCP_COARSE_TIMER_SECS for renewal time */
  u16_t t2_timeout;  /* #ticks with period DHCP_COARSE_TIMER_SECS for rebind time */
  struct ip_addr server_ip_addr; /* dhcp server address that offered this lease */
  struct ip_addr offered_ip_addr;
  struct ip_addr offered_sn_mask;
  struct ip_addr offered_gw_addr;
  struct ip_addr offered_bc_addr;
#define DHCP_MAX_DNS 2
  u32_t dns_count; /* actual number of DNS servers obtained */
  struct ip_addr offered_dns_addr[DHCP_MAX_DNS]; /* DNS server addresses */
 
  u32_t offered_t0_lease; /* lease period (in seconds) */
  u32_t offered_t1_renew; /* recommended renew time (usually 50% of lease period) */
  u32_t offered_t2_rebind; /* recommended rebind time (usually 66% of lease period)  */
/** Patch #1308
 *  TODO: See dhcp.c "TODO"s
 */
#if 0
  struct ip_addr offered_si_addr;
  u8_t *boot_file_name;
#endif

  /* client options */
  u8_t *clientid; /* optional client id to use */
  u8_t *vendorclass; /* optional vendor class-id to use */
  u8_t *hostname; /* optional hostname to use */
  u8_t arp[6]; /* our arp address */
};

typedef enum callback_type
{
    DHCP_DISCOVER_FAIL,
    DHCP_REQUEST_FAIL,
    DHCP_REBINDING_TIMEOUT,
    DHCP_BOUND_OK,
    DHCP_BOUND_FAIL,
    DHCP_RENEWING_OK,
    DHCP_REBINDING_OK,
    DHCP_REQUESTING_NAK,
    DHCP_RENEWING_NAK,
    DHCP_REBINDING_NAK
}CALLBACK_TYPE_T;

typedef void (*dhcp_core_callback)(CALLBACK_TYPE_T type, struct dhcp_info *dhcp);

typedef enum msg_type
{
    MSG_IDLE,
    MSG_DHCP,
    MSG_TIMEOUT,
    MSG_STOP,
    MSG_QUIT
}MSG_TYPE_T;

typedef struct app_msg
{
    int sockfd;
    MSG_TYPE_T type;
    //dhcp_callback callback;
    
}APP_MSG_T;

typedef struct offered_info
{
    struct ip_addr offered_ip;
    struct ip_addr offered_gw;
    struct ip_addr offered_sn;
    struct ip_addr offered_dns[2];
    int offered_timeout;
    
    /* ... */
}OFFERED_INFO_T;

typedef void (*dhcp_app_callback)(OFFERED_INFO_T info);


//////////////////////////////////////////////////////////////////////
/** DHCP message item offsets and length */
#define DHCP_MSG_OFS        (UDP_DATA_OFS)  
#define DHCP_OP_OFS         (DHCP_MSG_OFS + 0)
#define DHCP_HTYPE_OFS      (DHCP_MSG_OFS + 1)
#define DHCP_HLEN_OFS       (DHCP_MSG_OFS + 2)
#define DHCP_HOPS_OFS       (DHCP_MSG_OFS + 3)
#define DHCP_XID_OFS        (DHCP_MSG_OFS + 4)
#define DHCP_SECS_OFS       (DHCP_MSG_OFS + 8)
#define DHCP_FLAGS_OFS      (DHCP_MSG_OFS + 10)
#define DHCP_CIADDR_OFS     (DHCP_MSG_OFS + 12)
#define DHCP_YIADDR_OFS     (DHCP_MSG_OFS + 16)
#define DHCP_SIADDR_OFS     (DHCP_MSG_OFS + 20)
#define DHCP_GIADDR_OFS     (DHCP_MSG_OFS + 24)
#define DHCP_CHADDR_OFS     (DHCP_MSG_OFS + 28)
#define DHCP_SNAME_OFS      (DHCP_MSG_OFS + 44)
#define DHCP_FILE_OFS       (DHCP_MSG_OFS + 108)
#define DHCP_MSG_LEN        236

#define DHCP_COOKIE_OFS     (DHCP_MSG_OFS + DHCP_MSG_LEN)
#define DHCP_OPTIONS_OFS    (DHCP_MSG_OFS + DHCP_MSG_LEN + 4)

#define DHCP_CLIENT_PORT    68  
#define DHCP_SERVER_PORT    67

/** DHCP client states */
#define DHCP_REQUESTING     1
#define DHCP_INIT           2
#define DHCP_REBOOTING      3
#define DHCP_REBINDING      4
#define DHCP_RENEWING       5
#define DHCP_SELECTING      6
#define DHCP_INFORMING      7
#define DHCP_CHECKING       8
#define DHCP_PERMANENT      9
#define DHCP_BOUND          10
/** not yet implemented #define DHCP_RELEASING 11 */
#define DHCP_BACKING_OFF    12
#define DHCP_OFF            13
#define DHCP_RELEASED       14
#define DHCP_INIT1          22


#define DHCP_BOOTREQUEST    1
#define DHCP_BOOTREPLY      2

#define DHCP_DISCOVER       1
#define DHCP_OFFER          2
#define DHCP_REQUEST        3
#define DHCP_DECLINE        4
#define DHCP_ACK            5
#define DHCP_NAK            6
#define DHCP_RELEASE        7
#define DHCP_INFORM         8

#define DHCP_HTYPE_ETH      1

#define DHCP_HLEN_ETH       6

#define DHCP_BROADCAST_FLAG 15
#define DHCP_BROADCAST_MASK (1 << DHCP_FLAG_BROADCAST)

/** BootP options */
#define DHCP_OPTION_PAD 0
#define DHCP_OPTION_SUBNET_MASK 1 /* RFC 2132 3.3 */
#define DHCP_OPTION_ROUTER 3
#define DHCP_OPTION_DNS_SERVER 6 
#define DHCP_OPTION_HOSTNAME 12
#define DHCP_OPTION_IP_TTL 23
#define DHCP_OPTION_MTU 26
#define DHCP_OPTION_BROADCAST 28
#define DHCP_OPTION_TCP_TTL 37
#define DHCP_OPTION_END 255

/** DHCP options */
#define DHCP_OPTION_REQUESTED_IP 50 /* RFC 2132 9.1, requested IP address */
#define DHCP_OPTION_LEASE_TIME 51 /* RFC 2132 9.2, time in seconds, in 4 bytes */
#define DHCP_OPTION_OVERLOAD 52 /* RFC2132 9.3, use file and/or sname field for options */

#define DHCP_OPTION_MESSAGE_TYPE 53 /* RFC 2132 9.6, important for DHCP */
#define DHCP_OPTION_MESSAGE_TYPE_LEN 1


#define DHCP_OPTION_SERVER_ID 54 /* RFC 2132 9.7, server IP address */
#define DHCP_OPTION_PARAMETER_REQUEST_LIST 55 /* RFC 2132 9.8, requested option types */

#define DHCP_OPTION_MAX_MSG_SIZE 57 /* RFC 2132 9.10, message size accepted >= 576 */
#define DHCP_OPTION_MAX_MSG_SIZE_LEN 2

#define DHCP_OPTION_T1 58 /* T1 renewal time */
#define DHCP_OPTION_T2 59 /* T2 rebinding time */
#define DHCP_OPTION_VENDOR_SPECIFIC_INFO    43
#define DHCP_OPTION_VENDOR_CLASS_ID 60
#define DHCP_OPTION_CLIENT_ID 61
#define DHCP_OPTION_TFTP_SERVERNAME 66
#define DHCP_OPTION_BOOTFILE 67

/** possible combinations of overloading the file and sname fields with options */
#define DHCP_OVERLOAD_NONE 0
#define DHCP_OVERLOAD_FILE 1
#define DHCP_OVERLOAD_SNAME  2
#define DHCP_OVERLOAD_SNAME_FILE 3

#define DHCP_MEM        -1
#define DHCP_BUF        -2
#define DHCP_LINK       -3
#define DHCP_SOCK_ERR    -4
#define DHCP_PARAM      -5
#define DHCP_SERVER_NOT_FOUND   -6
#define DHCP_FILE_NOT_FOUND -7
#define DHCP_RQ_ERR   -8
#define DHCP_ACK_ERR   -9
#define DHCP_RCV_ERR  -10
#define DHCP_NOSUPPORT_OPT -11

#endif /* _DHCP_CLIENT_H_ */

