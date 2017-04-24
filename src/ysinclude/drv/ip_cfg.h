/*************************************************************************
* Copyright(c) 2009, xxxcorporation
* All rights reserved

* ip_cfg.h : ip cfg head file

* version: 	1.0
* author: 	ywg
* time:	2009-02-07

* history:	none
**************************************************************************/
#ifndef _IP_CFG_H_
#define _IP_CFG_H_

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif

#include "stddefs.h"
//#include "type.h"
//#include "dhcp_client.h"
#define __LINUX__

typedef struct __net_device_stats {
	unsigned long long rx_packets;	/* total packets received       */
	unsigned long long tx_packets;	/* total packets transmitted    */
	unsigned long long rx_bytes;	/* total bytes received         */
	unsigned long long tx_bytes;	/* total bytes transmitted      */
	unsigned long rx_errors;	/* bad packets received         */
	unsigned long tx_errors;	/* packet transmit problems     */
	unsigned long rx_dropped;	/* no space in linux buffers    */
	unsigned long tx_dropped;	/* no space available in linux  */
	unsigned long rx_multicast;	/* multicast packets received   */
	unsigned long rx_compressed;
	unsigned long tx_compressed;
	unsigned long collisions;

	/* detailed rx_errors: */
	unsigned long rx_length_errors;
	unsigned long rx_over_errors;	/* receiver ring buff overflow  */
	unsigned long rx_crc_errors;	/* recved pkt with crc error    */
	unsigned long rx_frame_errors;	/* recv'd frame alignment error */
	unsigned long rx_fifo_errors;	/* recv'r fifo overrun          */
	unsigned long rx_missed_errors;	/* receiver missed packet     */
	/* detailed tx_errors */
	unsigned long tx_aborted_errors;
	unsigned long tx_carrier_errors;
	unsigned long tx_fifo_errors;
	unsigned long tx_heartbeat_errors;
	unsigned long tx_window_errors;
}net_device_stats_t;

#define dhcp_ip_addr_set(dest, src) (dest)->addr = \
                               ((src) == NULL? 0:\
                               (src)->addr)

typedef void (*DRV_EthLinkCallback)(BOOL bLink);

int IPCFG_UpInterface(char *if_name);
int IPCFG_DownInterface(char *if_name);
unsigned int IPCFG_GetIpAddr(char *if_name);
int IPCFG_SetIpAddr(char *if_name, unsigned int ip_addr);
unsigned int IPCFG_GetSubMask(char *if_name);
int IPCFG_SetSubMask(char *if_name, unsigned int submask);
unsigned int IPCFG_GetGateWay();
int IPCFG_SetGateWay(char *if_name, unsigned int gw);
unsigned int IPCFG_GetBroadAddr(char *if_name);
int IPCFG_SetBroadAddr(char *if_name, unsigned int brdaddr);
void IPCFG_GetIfFlag();
void IPCFG_SetIfFlag();
unsigned int IPCFG_GetDns();
int IPCFG_SetDns(unsigned int dnsaddr);
int IPCFG_GetMacAddr(char *if_name, unsigned char *mac_addr);
int IPCFG_SetMacAddr(char *if_name, char *mac_addr);
char IPCFG_GetNICMode(char *if_name);
int IPCFG_SetNICMode(char *if_name, unsigned char mode);
int IPCFG_GetDevStat(char *if_name, net_device_stats_t *dev_stat);
int IPCFG_GetLinkStatus(char * if_name);
void IPCFG_InitDhcp();
void IPCFG_ResetDhcp(char *if_name, char *our_opt60, int our_len, char *you_opt60, int you_len);
unsigned int IPCFG_GetHostByName(char *cname);

unsigned short    ipcfg_htons(unsigned short n);
unsigned short    ipcfg_ntohs(unsigned short n);
unsigned int        ipcfg_htonl(unsigned int n);
unsigned int        ipcfg_ntohl(unsigned int n);

//extern typedef void (*dhcp_app_callback)(OFFERED_INFO_T info);
#if 0
void dhcp_init(void);
void register_dhcpok_callback(dhcp_app_callback callback);
int dhcp_start(char * if_name, char * our_opt60, int our_len, char * you_opt60, int you_len);
void dhcp_stop();
void register_dhcpfail_callback(dhcp_app_callback callback);
void dhcp_term(void);
#endif

int IPCFG_PingTest( unsigned int ip, unsigned int sec);

/*设置使用的网口: Eth=0 使用eth0；Eth=1 使用eth1*/
void IPCFG_SetEth(unsigned char Eth);
void IPCFG_RegEthLinkCallback(DRV_EthLinkCallback Callback_f);
void IPCFG_Init();

void DRV_GetMAC(unsigned char *AddrMAC);

unsigned int DRV_GetIPAddr_InUse(void);

unsigned int DRV_GetMask_InUse(void);

unsigned int DRV_GetGW_InUse(void);

unsigned int DRV_GetDNS_InUse(void);

unsigned int DRV_GetDNSEx_InUse();

void DRV_IP_RESET(void);


#ifdef __cplusplus
}
#endif
#endif

