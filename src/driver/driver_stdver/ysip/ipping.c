/*************************************************************************
* Copyright(c) 2009, xxxcorporation
* All rights reserved

* ipping.c : ip ping if ---arp ping if

* version: 	1.0
* author: 	ywg
* time:	2009-02-07

* history:	none
**************************************************************************/
/* system */
#include "stdio.h"
#include "string.h"
#include <stdlib.h>
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
#include <asm/types.h>

//#include  "linux/if.h"  //zzx add
//#include  "linux/sockios.h"   //zzx  add
/* socket */
#include <arpa/inet.h>
#include <net/if_arp.h>
#include <netinet/ether.h>
#include "sys/socket.h"
#include "netinet/in.h"
#include "netdb.h"
#include <linux/route.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/tcp.h>
#include <linux/ethtool.h>
#include <linux/if.h>
#include <linux/sockios.h>
#include <linux/ip.h>

#include "type.h"
#include "ip_cfg.h"

#define ICMP_ECHO 8
#define ICMP_ECHOREPLY 0
#define BUFSIZE 4096
// ICMP Header 
typedef struct ICMPHDR
{
  union
  {
    u16_t tc;
    struct
    {
        u8_t Type; // Type
        u8_t Code; // Code
    }TYPECODE;
  }typecode;
  
  u16_t Checksum; // Checksum
  u16_t ID; // Identification
  u16_t Seq; // Sequence
}ICMPHDR;

 //ICMP Echo Request
typedef struct ECHOREQUEST
{
    struct ICMPHDR icmpHdr;
    u8_t cData[40];
}ECHOREQUEST;

// ICMP Echo Reply
typedef struct ECHOREPLY
{
  struct ECHOREQUEST echoRequest;
}ECHOREPLY;

extern unsigned short _checksum(unsigned short *buffer, int size);
int _WaitForEchoReply(int socket, unsigned int sec);
int icmp_parse_packet(char *buf, int len);

int IPCFG_PingTest( unsigned int ip, unsigned int sec)
{
    int sockfd;
    int nRet;
    ECHOREQUEST echoReq;
    ECHOREPLY icmpRecv;
    struct sockaddr_in addrDest;

    int Seq = 1;
    int Length = 32;
    int Plength;
    int addr_len;

    char recvbuf[BUFSIZE];

    memset(&echoReq, 0 ,sizeof(echoReq));
    memset(&icmpRecv, 0 ,sizeof(icmpRecv));

    Plength = sizeof(ICMPHDR)+Length;

    if((sockfd = socket(AF_INET, SOCK_RAW, 1)) < 0)
    { 
       printf("socket icmp error\n");
       return -1;
    } 

    addrDest.sin_family = AF_INET;
    addrDest.sin_addr.s_addr = htonl(ip);

    memset(addrDest.sin_zero, 0, 8); 
    echoReq.icmpHdr.typecode.TYPECODE.Type = ICMP_ECHO; 
    echoReq.icmpHdr.typecode.TYPECODE.Code = 0;
    echoReq.icmpHdr.ID = 0x0200;

    for (nRet = 0; nRet <Length; nRet++)
    {
        echoReq.cData[nRet] = 'a'+nRet; 
    }

    {
        echoReq.icmpHdr.Seq = Seq++;
        echoReq.icmpHdr.Checksum = 0;
        echoReq.icmpHdr.Checksum = _checksum((unsigned short*)&echoReq, Plength);
 

        addr_len = sizeof(struct sockaddr);
	    
        if(sendto(sockfd, (struct ECHOREQUEST*)&echoReq, Plength, 0, (struct sockaddr *)&addrDest, addr_len) < 0)
        { 
            close(sockfd);
	     return -1;
        } 

        if(_WaitForEchoReply(sockfd, sec)>0)
        {
            nRet = recvfrom(sockfd, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *)&addrDest, &addr_len);
            if(nRet>0)
            {
		if(icmp_parse_packet(recvbuf, nRet) == -1)  
                {
                    close(sockfd);
                    return -2;
                }
            }
            else
            {
                close(sockfd);
                return -2;
            }
        }
        else 
        {
            close(sockfd);
            return -2;
        }
    }
    close(sockfd);
    return 1;
}

#if 0
//CheckSum 计算校验和
unsigned short checksum(unsigned short *buffer, int size) 
{ 
	//将缓冲区中的数据加起来
	//数据（包括IP头和IP数据）在存储区中实际上是以二进制的形式存放的，
	//所以相当于将这些二进制数据加在一起
	unsigned long cksum=0; 

        if (buffer == NULL)
        {
            Err_Print("[MOTO]checksum parameter is null!\n");
            return 0;
        }
    
	while(size >1) 
	{ 
		cksum+=*buffer++; 
		size -=sizeof(unsigned short); 
	} 
	if(size ) 
	{ 
		cksum += *(unsigned short *)buffer; 
	} 
	//上面完成了求和运算
	//十六位运算有可能而且很有可能产生超过十六位的结果
	//回卷  取出高十六位 加上 原来的低十六位
	cksum = (cksum >> 16) + (cksum & 0xffff); 
	//再加一次可保证计算值在十六位内
	cksum += (cksum >>16); 
	//取反返回即为校验和
	return (unsigned short)(~cksum); 
} 
#endif

unsigned short _checksum(unsigned short *buffer, int size) 
{ 
	//将缓冲区中的数据加起来
	//数据（包括IP头和IP数据）在存储区中实际上是以二进制的形式存放的，
	//所以相当于将这些二进制数据加在一起
	unsigned long cksum=0; 
	while(size >1) 
	{ 
		cksum+=*buffer++; 
		size -=sizeof(unsigned short); 
	} 
	if(size ) 
	{ 
		cksum += *(unsigned short *)buffer; 
	} 
	//上面完成了求和运算
	//十六位运算有可能而且很有可能产生超过十六位的结果
	//回卷  取出高十六位 加上 原来的低十六位
	cksum = (cksum >> 16) + (cksum & 0xffff); 
	//再加一次可保证计算值在十六位内
	cksum += (cksum >>16); 
	//取反返回即为校验和
	return (unsigned short)(~cksum); 
} 

//Wait for echoRecv
int _WaitForEchoReply(int socket, unsigned int sec)
{
    struct timeval Timeout;
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(socket, &readfds);

    if(sec > 0){
        Timeout.tv_sec = sec;
        Timeout.tv_usec = 0;
    }else{
        Timeout.tv_sec = 0;
        Timeout.tv_usec = 100000;
    }

    if (select(socket+1, &readfds, NULL, NULL, &Timeout)>0)
    {
        return(FD_ISSET(socket, &readfds));
    }
    else
        return -1;
}

int icmp_parse_packet(char *buf, int len)
{
	int iphdrlen, icmplen;
	struct iphdr *ip;
	struct ECHOREPLY *icmpRecv;

	ip = (struct iphdr *)buf;		/*start of  IP header*/
	iphdrlen = ip->ihl << 2;	/*length of IP header*/
	icmpRecv = (struct ECHOREPLY *)(buf + iphdrlen);
	if ( (icmplen = len - iphdrlen) < 8 )
		return -1;

	if ((icmpRecv->echoRequest.icmpHdr.typecode.TYPECODE.Type == ICMP_ECHOREPLY) && 
			(icmpRecv->echoRequest.icmpHdr.ID == 0x0200))
		return 1;
	else 
		return -1;	
}
