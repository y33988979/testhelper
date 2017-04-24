/*************************************************************************
* Copyright(c) 2009, xxxcorporation
* All rights reserved

* ip_cfg.c : ip config if

* version:  1.0
* author:   ywg
* time: 2009-02-07

* history:  none
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

#include "drv_typedef.h"
#include "ys_typedef.h"

/* msg queue */
//#include "sys/msg.h"

#include "ip_cfg.h"

#define IPCFG_DEBUG printf

#if 1
/******************* global variable   ******************/
const char eth_name_array[2][5] =
{
    "eth0",
    "eth1"
};

const char *eth_name = eth_name_array[0];

static int g_s32ETHFd[2] = {0, 0};

typedef enum hiEth_Port_E
{
    ETH_PORT_UP = 0,
    ETH_PORT_DOWN,
    ETH_PORT_MAX
} Eth_Port_E;
#endif

int IPCFG_UpInterface(char *if_name);
int IPCFG_DownInterface(char *if_name);

unsigned short
ipcfg_htons(unsigned short n)
{
    return ((n & 0xff) << 8) | ((n & 0xff00) >> 8);
}

unsigned short
ipcfg_ntohs(unsigned short n)
{
    return ipcfg_htons(n);
}

unsigned int
ipcfg_htonl(unsigned int n)
{
    return ((n & 0xff) << 24) |
           ((n & 0xff00) << 8) |
           ((n & 0xff0000) >> 8) |
           ((n & 0xff000000) >> 24);
}

unsigned int
ipcfg_ntohl(unsigned int n)
{
    return ipcfg_htonl(n);
}




/**********************************
func: IPCFG_GetIpAddr
desp: 获取ip地址
param: if_name 接口名称 一般为 eth0
return: ip地址
**********************************/
unsigned int IPCFG_GetIpAddr(char *if_name)
{
    unsigned int ip_addr;
#if defined(__WIN32__)
#endif

#if defined(__LINUX__)
    {
        int sockfd;
        int ret;
        struct ifreq ifr;
        struct sockaddr_in   *sin;

        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(sockfd < 0)
        {
            IPCFG_DEBUG("get ip addr : socket open error !\n");
            return 0;
        }

        memset(&ifr, 0, sizeof(ifr));
        //strcpy(ifr.ifr_ifrn.ifrn_name, if_name);
        strcpy(ifr.ifr_name, if_name);

        ret = ioctl(sockfd, SIOCGIFADDR, &ifr);
        if(ret < 0)
        {
            IPCFG_DEBUG("get ip addr : ioctl open error !\n");
            close(sockfd);
            return 0;
        }

        //ip_addr = ipcfg_htonl(ifr.ifr_ifru.ifru_addr.sin_addr);
        //sin = (struct sockaddr_in *)$ifr.ifr_addr;
        sin = (struct sockaddr_in   *)&ifr.ifr_ifru.ifru_addr;
        ip_addr = ipcfg_htonl(sin->sin_addr.s_addr);
        close(sockfd);
        return ip_addr;
    }
#endif

#if defined(__STSYSTEM__)
#endif

}

/**********************************
func: IPCFG_SetIpAddr
desp: 设置系统的ip地址
param: if_name 要设置的接口名称
       ip_addr 要设置的ip地址
return: ok     0
        fail  -1
**********************************/
int IPCFG_SetIpAddr(char *if_name, unsigned int ip_addr)
{
#if defined(__WIN32__)
#endif

#if defined(__LINUX__)
    {
        int sockfd;
        int ret;
        struct ifreq ifr;
        struct sockaddr_in   *sin;

        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(sockfd < 0)
        {
            IPCFG_DEBUG("set ip addr: socket open error !\n");
            return -1;
        }

        memset(&ifr, 0, sizeof(ifr));
        strcpy(ifr.ifr_name, if_name);

        //ifr.ifr_ifru.ifru_addr.sin_addr = ipcfg_htonl(ip_addr);
        sin = (struct sockaddr_in   *)&ifr.ifr_ifru.ifru_addr;
        sin->sin_family = AF_INET;
        sin->sin_addr.s_addr = ipcfg_htonl(ip_addr);

        //IPCFG_DownInterface(if_name);
        ret = ioctl(sockfd, SIOCSIFADDR, &ifr);
        if(ret < 0)
        {
            IPCFG_DEBUG("set ip addr: ioctl open error !\n");
            close(sockfd);
            return -1;
        }
        //IPCFG_UpInterface(if_name);
        close(sockfd);
        return 0; //success
    }
#endif

#if defined(__STSYSTEM__)
#endif

}

/**********************************
func: IPCFG_GetSubMask
desp: 获取系统的子网掩码
param: if_name 接口名称
return: 获取的子网掩码
**********************************/
unsigned int IPCFG_GetSubMask(char *if_name)
{
    unsigned int sbmask;
#if defined(__WIN32__)
#endif

#if defined(__LINUX__)
    {
        int sockfd;
        int ret;
        struct ifreq ifr;
        struct sockaddr_in   *sin;

        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(sockfd < 0)
        {
            IPCFG_DEBUG("get sub mask: socket open error!\n");
            return 0;
        }

        memset(&ifr, 0, sizeof(ifr));
        strcpy(ifr.ifr_name, if_name);

        ret = ioctl(sockfd, SIOCGIFNETMASK, &ifr);
        if(ret < 0)
        {
            IPCFG_DEBUG("get sub mask: ioctl open error !\n");
            return 0;
        }

        //sbmask = ipcfg_htonl(ifr.ifr_ifru.ifru_addr.sin_addr);
        sin = (struct sockaddr_in *)&ifr.ifr_ifru.ifru_addr;

        sbmask = ipcfg_htonl(sin->sin_addr.s_addr);

        close(sockfd);
        return sbmask;
    }
#endif

#if defined(__STSYSTEM__)
#endif

}

/**********************************
func: IPCFG_SetSubMask
desp: 设置系统的子网掩码
param: if_name 接口名称
       submask 要设置的子网掩码
return: ok    0
        fail  -1
**********************************/
int IPCFG_SetSubMask(char *if_name, unsigned int submask)
{
#if defined(__WIN32__)
#endif

#if defined(__LINUX__)
    {
        int sockfd;
        int ret;
        struct ifreq ifr;
        struct sockaddr_in *sin;

        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(sockfd < 0)
        {
            IPCFG_DEBUG("set sub mask: socket open error !\n");
            return -1;
        }

        memset(&ifr, 0, sizeof(ifr));
        strcpy(ifr.ifr_name, if_name);

        //ifr.ifr_ifru.ifru_addr.sin_addr = ipcfg_htonl(submask);
        sin = (struct sockaddr_in *)&ifr.ifr_ifru.ifru_addr;
        sin->sin_addr.s_addr = ipcfg_htonl(submask);
        sin->sin_family = AF_INET;

        ret = ioctl(sockfd, SIOCSIFNETMASK, &ifr);
        if(ret < 0)
        {
            IPCFG_DEBUG("set sub mask: ioctl open error !\n");
            close(sockfd);
            return -1;
        }

        close(sockfd);
        return 0;
    }
#endif

#if defined(__STSYSTEM__)
#endif

}

#define _BUFSIZE 1024
struct _route_info
{
    u_int dstAddr;
    u_int srcAddr;
    u_int gateWay;
    char ifName[/*IF_NAMESIZE*/16];
};

int _readNlSock(int sockFd, char *bufPtr, int seqNum, int pId)
{
    struct nlmsghdr *nlHdr;
    int readLen = 0, msgLen = 0;

    do
    {
        readLen = recv(sockFd, bufPtr, _BUFSIZE - msgLen, 0);
        nlHdr = (struct nlmsghdr *)bufPtr;
        if(nlHdr->nlmsg_type == NLMSG_DONE)
            break;

        bufPtr += readLen;
        msgLen += readLen;

        if((nlHdr->nlmsg_flags & NLM_F_MULTI) == 0)
            break;
    }
    while((nlHdr->nlmsg_seq != seqNum) || (nlHdr->nlmsg_pid != pId));
    return msgLen;
}

void _parseRoutes(struct nlmsghdr *nlHdr, struct _route_info *rtInfo,unsigned int *gateway)
{
    struct rtmsg *rtMsg;
    struct rtattr *rtAttr;
    int rtLen;
    //char tempBuf[100];
    struct in_addr dst;
    struct in_addr gate;

    rtMsg = (struct rtmsg *)NLMSG_DATA(nlHdr);
    if((rtMsg->rtm_family != AF_INET) || (rtMsg->rtm_table != RT_TABLE_MAIN))
        return;

    rtAttr = (struct rtattr *)RTM_RTA(rtMsg);
    rtLen = RTM_PAYLOAD(nlHdr);

    for(; RTA_OK(rtAttr,rtLen); rtAttr = RTA_NEXT(rtAttr,rtLen))
    {
        switch(rtAttr->rta_type)
        {
            case RTA_OIF:
                if_indextoname(*(int *)RTA_DATA(rtAttr), rtInfo->ifName);
                break;
            case RTA_GATEWAY:
                rtInfo->gateWay = *(u_int *)RTA_DATA(rtAttr);
                break;
            case RTA_PREFSRC:
                rtInfo->srcAddr = *(u_int *)RTA_DATA(rtAttr);
                break;
            case RTA_DST:
                rtInfo->dstAddr = *(u_int *)RTA_DATA(rtAttr);
                break;
        }
    }

    dst.s_addr = rtInfo->dstAddr;

    if(strstr((char *)inet_ntoa(dst), "0.0.0.0"))
    {
        gate.s_addr = rtInfo->gateWay;
        //sprintf(gateway, "%s",(char *)inet_ntoa(gate));
        *gateway = gate.s_addr;
    }
    //IPCFG_DEBUG("parseRoute:%s\n",gateway);
    return;
}


/**********************************
func: IPCFG_GetGateWay
desp: 获取系统的网关地址
param: gateway 保存获取的网关地址
return: ok    0
        fail  -1
**********************************/
unsigned int IPCFG_GetGateWay()
{
    unsigned int gw;
#if defined(__WIN32__)
#endif

#if defined(__LINUX__)
    {
        struct nlmsghdr *nlMsg;
        struct rtmsg *rtMsg;
        struct _route_info *rtInfo;
        char msgBuf[_BUFSIZE];
        int sock, len, msgSeq = 0;

        if((sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE)) < 0)
        {
            IPCFG_DEBUG("Socket Creation: error \n");
            return 0;
        }

        memset(msgBuf, 0, _BUFSIZE);

        nlMsg = (struct nlmsghdr *)msgBuf;
        rtMsg = (struct rtmsg *)NLMSG_DATA(nlMsg);

        nlMsg->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));
        nlMsg->nlmsg_type = RTM_GETROUTE;

        nlMsg->nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST;
        nlMsg->nlmsg_seq = msgSeq++;
        nlMsg->nlmsg_pid = getpid();

        if(send(sock, nlMsg, nlMsg->nlmsg_len, 0) < 0)
        {
            IPCFG_DEBUG("Write To Socket Failed...\n");
            close(sock);
            return 0;
        }

        if((len = _readNlSock(sock, msgBuf, msgSeq, getpid())) < 0)
        {
            IPCFG_DEBUG("Read From Socket Failed...\n");
            close(sock);
            return 0;
        }
        rtInfo = (struct _route_info *)malloc(sizeof(struct _route_info));
        for(; NLMSG_OK(nlMsg,len); nlMsg = NLMSG_NEXT(nlMsg,len))
        {
            memset(rtInfo, 0, sizeof(struct _route_info));
            _parseRoutes(nlMsg, rtInfo,&gw);
        }
        free(rtInfo);
        close(sock);
        //IPCFG_DEBUG("\nGet gateway is Ok! %s\n",gateway);
        return ipcfg_htonl(gw);
    }
#endif

#if defined(__STSYSTEM__)
#endif

}

/**********************************
func: IPCFG_SetGateWay
desp: 设置系统的网关地址
param: if_name 接口名称
       gw 要设置的网关地址
return: ok    0
        fail  -1
**********************************/
int IPCFG_SetGateWay(char *if_name, unsigned int gw)
{
#if defined(__WIN32__)
#endif

#if defined(__LINUX__)
    {
        int skfd;
        struct rtentry rt;
        int err;

        skfd = socket(/*PF_INET*/AF_INET, SOCK_DGRAM, 0);
        if(skfd < 0)
        {
            skfd = socket(PF_INET, SOCK_DGRAM, 0);
            if(skfd<0)
                return -1;
        }

        /* Delete existing defalt gateway */
        memset(&rt, 0, sizeof(rt));

        rt.rt_dst.sa_family = AF_INET;
        ((struct sockaddr_in *)&rt.rt_dst)->sin_addr.s_addr = 0;

        rt.rt_genmask.sa_family = AF_INET;
        ((struct sockaddr_in *)&rt.rt_genmask)->sin_addr.s_addr = 0;

        rt.rt_flags = RTF_UP;

        err = ioctl(skfd, SIOCDELRT, &rt);

        if((err == 0 || errno == ESRCH) && gw)
        {
            /* Set default gateway */
            memset(&rt, 0, sizeof(rt));

            rt.rt_dst.sa_family = AF_INET;
            ((struct sockaddr_in *)&rt.rt_dst)->sin_addr.s_addr = 0;

            rt.rt_gateway.sa_family = AF_INET;
            ((struct sockaddr_in *)&rt.rt_gateway)->sin_addr.s_addr = ipcfg_htonl(gw);

            rt.rt_genmask.sa_family = AF_INET;
            ((struct sockaddr_in *)&rt.rt_genmask)->sin_addr.s_addr = 0;

            rt.rt_flags = RTF_UP | RTF_GATEWAY;

            err = ioctl(skfd, SIOCADDRT, &rt);
        }

        close(skfd);

        return err;
    }
#endif

#if defined(__STSYSTEM__)
#endif

}

/**********************************
func: IPCFG_Get_BroadAddr
desp: 获取系统的广播地址
param: if_name 接口名称
return: 获取的广播地址
**********************************/
unsigned int IPCFG_GetBroadAddr(char *if_name)
{
    unsigned int bdaddr;
#if defined(__WIN32__)
#endif

#if defined(__LINUX__)
    {
        int sockfd;
        int ret;
        struct ifreq ifr;
        struct sockaddr_in *sin;

        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(sockfd < 0)
        {
            IPCFG_DEBUG("get broadcast addr: socket open error !\n");
            return 0;
        }

        memset(&ifr, 0, sizeof(ifr));
        strcpy(ifr.ifr_name, if_name);

        ret = ioctl(sockfd, SIOCGIFBRDADDR, &ifr);
        if(ret < 0)
        {
            IPCFG_DEBUG("get broadcast addr: ioctl open error !\n");
            close(sockfd);
            return 0;
        }

        //bdaddr = ipcfg_htonl(ifr.ifr_ifru.broad_addr.sin_addr);
        sin = (struct sockaddr_in *)&ifr.ifr_ifru.ifru_broadaddr;
        bdaddr = ipcfg_htonl(sin->sin_addr.s_addr);

        close(sockfd);
        return bdaddr;
    }
#endif

#if defined(__STSYSTEM__)
#endif

}

/**********************************
func: IPCFG_SetBroadAddr
desp: 设置系统的广播地址
param: if_name 接口名称
       brdaddr 要设置的广播地址
return: ok    0
        fail  -1
**********************************/
int IPCFG_SetBroadAddr(char *if_name, unsigned int brdaddr)
{
#if defined(__WIN32__)
#endif

#if defined(__LINUX__)
    {
        int sockfd;
        int ret;
        struct ifreq ifr;
        struct sockaddr_in *sin;

        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(sockfd < 0)
        {
            IPCFG_DEBUG("set broadcast addr: socket open error !\n");
            return -1;
        }

        memset(&ifr, 0, sizeof(ifr));
        strcpy(ifr.ifr_name, if_name);

        //ifr.ifr_ifru.broad_addr.sin_addr = ipcfg_htonl(brdaddr);
        sin = (struct sockaddr_in *)&ifr.ifr_ifru.ifru_broadaddr;
        sin->sin_addr.s_addr = ipcfg_htonl(brdaddr);
        sin->sin_family = AF_INET;

        ret = ioctl(sockfd, SIOCSIFBRDADDR, &ifr);
        if(ret < 0)
        {
            IPCFG_DEBUG("set broadcast addr: ioctl open error !\n");
            close(sockfd);
            return -1;
        }

        close(sockfd);
        return 0;
    }
#endif

#if defined(__STSYSTEM__)
#endif

}

/*
* get dns
*/
unsigned int IPCFG_GetDns(void)
{
    int i1,i2,Result;
    FILE *pFile=NULL;
    char dns_buf[20];
    unsigned int dns_addr;
    char BufferTmp[100];

    //pFile = fopen("/FlashData/resolv.conf","r");
    //pFile = fopen("/etc/config/resolv.conf","r");
    pFile = fopen("/etc/resolv.conf","r");
    //printf("STBDNS_Get begin\n");
    if(pFile == NULL)
    {
        perror("fopen");
        IPCFG_DEBUG("get dns : open file error \n");
        return 0;
    }
    //IPSET_DEBUG(("Get DNS step1 (open file)is OK!\n"));
    i1 = 0;
    i2 = 0;
    memset(BufferTmp,0,sizeof(BufferTmp));
    Result = fread(BufferTmp,sizeof(char),100,pFile);
    if(Result==0||Result<=19) /* nameserver 0.0.0.0 */
    {
        fclose(pFile);
        IPCFG_DEBUG("get dns : read file error \n");
        return 0;
    }

    while(BufferTmp[i1]!=0)
    {
        if(strncmp(BufferTmp+i1,"nameserver",10)==0)
            break;
        i1++;
    }
    i1+=10;
    if(isdigit(BufferTmp[i1])==0)//if it's not a digit
        i1++;
    i2=i1;
    while(BufferTmp[i2]!=' '  ||BufferTmp[i2]!='\r')
    {
        if(isgraph(BufferTmp[i2])==0)//if it's not graph
            break;
        i2++;
    }

    strncpy(dns_buf,BufferTmp+i1,i2-i1);
    dns_buf[i2-i1] = '\0';
    dns_addr = ipcfg_htonl(inet_addr(dns_buf));
    fclose(pFile);
    //IPCFG_DEBUG("Get DNS is OK %lx\n",dns_addr);

    return dns_addr;
}

int IPCFG_GetDnsEx(unsigned int *dnsaddrs)
{
    int i1,i2,Result;
    FILE *pFile=NULL;
    char dns_buf[20];
    unsigned int dns_addr;
    char BufferTmp[100];

    //pFile = fopen("/FlashData/resolv.conf","r");
    pFile = fopen("/etc/resolv.conf","r");
    //printf("STBDNS_Get begin\n");
    if(pFile == NULL)
    {
        perror("fopen");
        IPCFG_DEBUG("get dns : open file error \n");
        return 0;
    }
    //IPSET_DEBUG(("Get DNS step1 (open file)is OK!\n"));
    i1 = 0;
    i2 = 0;
    memset(BufferTmp,0,sizeof(BufferTmp));
    Result = fread(BufferTmp,sizeof(char),100,pFile);
    if(Result==0||Result<=19) /* nameserver 0.0.0.0 */
    {
        fclose(pFile);
        IPCFG_DEBUG("get dns : read file error \n");
        return 0;
    }

    while(BufferTmp[i1]!=0)
    {
        if(strncmp(BufferTmp+i1,"nameserver",10)==0)
            break;
        i1++;
    }
    i1+=10;
    if(isdigit(BufferTmp[i1])==0)//if it's not a digit
        i1++;
    i2=i1;
    while(BufferTmp[i2]!=' '  ||BufferTmp[i2]!='\n')
    {
        if(isgraph(BufferTmp[i2])==0)//if it's not graph
            break;
        i2++;
    }

    strncpy(dns_buf,BufferTmp+i1,i2-i1);
    dns_buf[i2-i1] = '\0';
    dns_addr = ipcfg_htonl(inet_addr(dns_buf));

    dnsaddrs[0] = dns_addr;

    /* get dns 2 */
    i1 = i2;
    while(BufferTmp[i1]!=0)
    {
        if(strncmp(BufferTmp+i1,"nameserver",10)==0)
            break;
        i1++;
    }
    i1+=10;
    if(isdigit(BufferTmp[i1])==0)//if it's not a digit
        i1++;
    i2=i1;
    while(BufferTmp[i2]!=' '  ||BufferTmp[i2]!='\n')
    {
        if(isgraph(BufferTmp[i2])==0)//if it's not graph
            break;
        i2++;
    }

    strncpy(dns_buf,BufferTmp+i1,i2-i1);
    dns_buf[i2-i1] = '\0';
    dns_addr = ipcfg_htonl(inet_addr(dns_buf));

    dnsaddrs[1] = dns_addr;

    fclose(pFile);
    //IPCFG_DEBUG("Get DNS is OK %lx\n",dns_addr);

    return 0;
}

/*
* set dns
*/
int IPCFG_SetDns(unsigned int dnsaddr)
{
    int Result;
    FILE *pFile=NULL;
    char *p;
    char dns_buf[20];
    struct in_addr dns_addr;
    char BufferTmp[100];

    //pFile = fopen("/FlashData/resolv.conf","w+");
    //pFile = fopen("/etc/config/resolv.conf","w+");
    pFile = fopen("/etc/resolv.conf","w+");
    //printf("STBDNS_Get begin\n");
    if(pFile == NULL)
    {
        perror("fopen");
        IPCFG_DEBUG("set dns : open file error \n");
        return -1;
    }
    //IPSET_DEBUG(("Get DNS step1 (open file)is OK!\n"));

    memset(BufferTmp,0,sizeof(BufferTmp));

    /* 202.106.0.20 */
    dns_addr.s_addr = ipcfg_htonl(dnsaddr);
    p = inet_ntoa(dns_addr);
    strcpy(dns_buf,p);
    strcpy(dns_buf+strlen(p)," \r\n");
    /* nameserver  */
    strncpy(BufferTmp, "nameserver ", strlen("nameserver "));
    /* nameserver 202.106.0.20 */
    strcpy((BufferTmp+11), dns_buf);

    Result = fwrite(BufferTmp, sizeof(char), strlen(BufferTmp), pFile);
    if(Result <= 0)
    {
        IPCFG_DEBUG("set dns : write file error \n");
        return -1;
    }

    fclose(pFile);

    return 0;
}

int IPCFG_SetDnsEx(unsigned int dnsaddr1, unsigned int dnsaddr2)
{
    int Result;
    FILE *pFile=NULL;
    char *p;
    char dns_buf[20];
    struct in_addr dns_addr;
    char BufferTmp[100];
    int buf_pos=0;

    //pFile = fopen("/FlashData/resolv.conf","w+");
    //pFile = fopen("/etc/config/resolv.conf","w+");
    pFile = fopen("/etc/resolv.conf","w+");
    //printf("STBDNS_Get begin\n");
    if(pFile == NULL)
    {
        perror("fopen");
        IPCFG_DEBUG("set dns : open file error \n");
        return -1;
    }
    //IPSET_DEBUG(("Get DNS step1 (open file)is OK!\n"));

    memset(BufferTmp,0,sizeof(BufferTmp));

    /* dns addr 1 */
    dns_addr.s_addr = ipcfg_htonl(dnsaddr1);
    p = inet_ntoa(dns_addr);
    strcpy(dns_buf,p);
    strcpy(dns_buf+strlen(p),"\n");
    /* nameserver  */
    buf_pos = strlen("nameserver ");
    strncpy(BufferTmp, "nameserver ", buf_pos);
    /* nameserver 202.106.0.20 */
    strcpy((BufferTmp+11), dns_buf);
    buf_pos += (strlen(p)+1);

    /* dns addr 2 */
    dns_addr.s_addr = ipcfg_htonl(dnsaddr2);
    p = inet_ntoa(dns_addr);
    strcpy(dns_buf,p);
    strcpy(dns_buf+strlen(p),"\n");
    /* nameserver  */
    strncpy((BufferTmp+buf_pos), "nameserver ", strlen("nameserver "));
    /* nameserver 202.106.0.20 */
    strcpy((BufferTmp+buf_pos+11), dns_buf);


    Result = fwrite(BufferTmp, sizeof(char), strlen(BufferTmp), pFile);
    if(Result <= 0)
    {
        IPCFG_DEBUG("set dns : write file error \n");
        return -1;
    }

    fclose(pFile);

    return 0;
}

/**********************************
func:
desp:
param:
return:
**********************************/
void IPCFG_GetIfFlag()
{
#if defined(__WIN32__)
#endif

#if defined(__LINUX__)
#endif

#if defined(__STSYSTEM__)
#endif

}

/**********************************
func:
desp:
param:
return:
**********************************/
void IPCFG_SetIfFlag()
{
#if defined(__WIN32__)
#endif

#if defined(__LINUX__)
#endif

#if defined(__STSYSTEM__)
#endif

}

int IPCFG_GetUPFlag(char *if_name)
{
#if defined(__WIN32__)
#endif

#if defined(__LINUX__)
    {
        int sockfd;
        int ret;
        struct ifreq ifr;
        short flag;

        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(sockfd < 0)
        {
            IPCFG_DEBUG("get up flag: socket open error !\n");
            return -1;
        }

        memset(&ifr, 0, sizeof(ifr));
        strcpy(ifr.ifr_name, if_name);

        flag = IFF_UP; //????
        ret = ioctl(sockfd, SIOCGIFFLAGS, &ifr);
        if(ret < 0)
        {
            IPCFG_DEBUG("get up flag: ioctl open error !\n");
            close(sockfd);
            return -1;
        }

        if(ifr.ifr_ifru.ifru_flags & flag)
        {
            close(sockfd);
            return 1;
        }
        else{
            close(sockfd);
            return 0;
        }

    }
#endif

#if defined(__STSYSTEM__)
#endif

}


/**********************************
func: IPCFG_GetMacAddr
desp: 获取系统的MAC地址
param: if_name 接口名称
       mac_addr 保存获取网卡地址
return: ok    0
        fail  -1
**********************************/
int IPCFG_GetMacAddr(char *if_name, unsigned char *mac_addr)
{
#if defined(__WIN32__)
#endif

#if defined(__LINUX__)
    {
        int sockfd;
        int ret;
        struct ifreq ifr;

        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(sockfd < 0)
        {
            IPCFG_DEBUG("get mac addr: socket open error !\n");
            return -1;
        }

        memset(&ifr, 0, sizeof(ifr));
        strcpy(ifr.ifr_name, if_name);

        ret = ioctl(sockfd, SIOCGIFHWADDR, &ifr);
        if(ret < 0)
        {
            IPCFG_DEBUG("get mac addr: ioctl open error !\n");
            close(sockfd);
            return -1;
        }

        mac_addr[0] = ifr.ifr_ifru.ifru_hwaddr.sa_data[0];
        mac_addr[1] = ifr.ifr_ifru.ifru_hwaddr.sa_data[1];
        mac_addr[2] = ifr.ifr_ifru.ifru_hwaddr.sa_data[2];
        mac_addr[3] = ifr.ifr_ifru.ifru_hwaddr.sa_data[3];
        mac_addr[4] = ifr.ifr_ifru.ifru_hwaddr.sa_data[4];
        mac_addr[5] = ifr.ifr_ifru.ifru_hwaddr.sa_data[5];

        close(sockfd);
        return 0;
    }
#endif

#if defined(__STSYSTEM__)
#endif

}

/**********************************
func: IPCFG_SetMacAddr
desp: 设置系统的网卡地址
param: if_name 接口名称
       mac_addr 要设置的MAC地址
return: ok    0
        fail  -1
**********************************/
int IPCFG_SetMacAddr(char *if_name, char *mac_addr)
{
#if defined(__WIN32__)
#endif

#if defined(__LINUX__)
    {
        int sockfd;
        int ret;
        struct ifreq ifr;

        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(sockfd < 0)
        {
            IPCFG_DEBUG(" set mac addr: socket open error !\n");
            return -1;
        }

        memset(&ifr, 0, sizeof(ifr));
        strcpy(ifr.ifr_name, if_name);

        ifr.ifr_ifru.ifru_hwaddr.sa_family = ARPHRD_ETHER;
        memcpy(ifr.ifr_ifru.ifru_hwaddr.sa_data, mac_addr, 6);

        IPCFG_DownInterface(if_name);
        ret = ioctl(sockfd, SIOCSIFHWADDR, &ifr);
        if(ret < 0)
        {
            IPCFG_DEBUG("set mac addr: ioctl open error !\n");
            close(sockfd);
            return -1;
        }
        IPCFG_UpInterface(if_name);

        close(sockfd);
        sys_delay(3000);
        return 0;
    }
#endif

#if defined(__STSYSTEM__)
#endif

}

/**********************************
func: IPCFG_UpInterface
desp: 激活网络接口
param: if_name 要激活的接口名称
return: ok    0
        fail  -1
**********************************/
int IPCFG_DownInterface(char *if_name)
{
#if defined(__WIN32__)
#endif

    //IPCFG_GetIfFlag();
    //IPCFG_SetIfFlag();

#if defined(__LINUX__)
    {
        int sockfd;
        int ret;
        struct ifreq ifr;
        short flag;

        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(sockfd < 0)
        {
            IPCFG_DEBUG("sys down if: socket open error !\n");
            return -1;
        }

        memset(&ifr, 0, sizeof(ifr));
        strcpy(ifr.ifr_name, if_name);

        flag = ~IFF_UP; //????
        ret = ioctl(sockfd, SIOCGIFFLAGS, &ifr);
        if(ret < 0)
        {
            IPCFG_DEBUG("sys get down if: ioctl open error !\n");
            close(sockfd);
            return -1;
        }
        //clear flag
        ifr.ifr_ifru.ifru_flags &= flag;
        IPCFG_DEBUG("ip cfg down interface : falg [%lx] \n", ifr.ifr_ifru.ifru_flags);

        ret = ioctl(sockfd, SIOCSIFFLAGS, &ifr);
        if(ret < 0)
        {
            IPCFG_DEBUG("sys set down if: ioctl open error");
            close(sockfd);
            return -1;
        }

        close(sockfd);
        return 0;
    }
#endif

#if defined(__STSYSTEM__)
#endif

}

/**********************************
func: IPCFG_DownInterface
desp: 关闭网络接口
param: if_name 要关闭的接口名称
return: ok    0
        fail  -1
**********************************/
int IPCFG_UpInterface(char *if_name)
{
#if defined(__WIN32__)
#endif

#if defined(__LINUX__)
    {
        int sockfd;
        int ret;
        struct ifreq ifr;
        short flag;

        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(sockfd < 0)
        {
            IPCFG_DEBUG("sys up if: socket open error !\n");
            return -1;
        }

        memset(&ifr, 0, sizeof(ifr));
        strcpy(ifr.ifr_name, if_name);

        // flag = IFF_UP;  //????
        //flag |= IFF_UP | IFF_RUNNING;
        ret = ioctl(sockfd, SIOCGIFFLAGS, &ifr);
        if(ret < 0)
        {
            IPCFG_DEBUG("sys get up if: ioctl open error !\n");
            close(sockfd);
            return -1;
        }

        //set flag
        ifr.ifr_ifru.ifru_flags |= (IFF_UP | IFF_RUNNING);//flag;

        ret = ioctl(sockfd, SIOCSIFFLAGS, &ifr);
        if(ret < 0)
        {
            IPCFG_DEBUG("sys set up if: ioctl open error !\n");
            close(sockfd);
            return -1;
        }

        close(sockfd);
        return 0;
    }
#endif

#if defined(__STSYSTEM__)
#endif

}

static void _ethtool_cmd_speed_set(struct ethtool_cmd *ep,
                                   unsigned short speed)
{

    ep->speed = (unsigned short int)speed;
    //ep->speed_hi = (unsigned short int)(speed >> 16);
}

static unsigned short _ethtool_cmd_speed(struct ethtool_cmd *ep)
{
    return ep->speed;
}

char IPCFG_GetNICMode(char *if_name)
{
    int skfd;
    char mode;
    struct ifreq ifr;
    struct ethtool_cmd ecmd;

    ecmd.cmd = ETHTOOL_GSET;

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, if_name, sizeof(ifr.ifr_name) - 1);
    ifr.ifr_data = (char *) &ecmd;

    if((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return -1;

    if(ioctl(skfd, SIOCETHTOOL, &ifr) == -1)
    {
        close(skfd);
        return -1;
    }

    close(skfd);

    mode = 0;

    switch(ecmd.duplex)
    {
        case 0x00:
            //fprintf(stdout, "Half\n");
            mode &= ~0xfd;
            if(ecmd.autoneg == 0x01)
            {
                /* auto enable */
                mode |= 0x04;
            }
            else
            {
                /* auto disable */
                mode &= ~0xfb;
            }
            if(100 == _ethtool_cmd_speed(&ecmd))
            {
                mode |= 0x01;
            }
            else
            {
                mode &= ~0xfe;
            }
            break;
        case 0x01:
            //fprintf(stdout, "Full\n");
            mode |= 0x02;
            if(ecmd.autoneg == 0x01)
            {
                /* auto enable */
                mode |= 0x04;
            }
            else
            {
                /* auto disable */
                mode &= ~0xfb;
            }
            if(100 == _ethtool_cmd_speed(&ecmd))
            {
                mode |= 0x01;
            }
            else
            {
                mode &= ~0xfe;
            }
            break;
        default:
            //fprintf(stdout, "Unknown! (%i)\n", ep->duplex);
            mode = -2; /* unkown */
            break;
    };

    return mode;
}

int IPCFG_SetNICMode(char *if_name, unsigned char mode)
{
    int skfd;
    unsigned short speed;
    struct ifreq ifr;
    struct ethtool_cmd ecmd;

    ecmd.cmd = ETHTOOL_GSET;

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, if_name, sizeof(ifr.ifr_name) - 1);
    ifr.ifr_data = (char *) &ecmd;

    if((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return -1;

    if(ioctl(skfd, SIOCETHTOOL, &ifr) == -1)
    {
        close(skfd);
        return -1;
    }

    /* set ecmd */
    if(mode & 0x04)
    {
        ecmd.autoneg = 0x01;
    }
    else
    {
        ecmd.autoneg = 0x00;
    }

    if(mode & 0x02)
    {
        ecmd.duplex = 0x01;
    }
    else
    {
        ecmd.duplex = 0x00;
    }

    if(mode & 0x01)
    {
        /* 100Mb */
        speed = 100;
        _ethtool_cmd_speed_set(&ecmd, speed);
    }
    else
    {
        /* 10Mb */
        speed = 10;
        _ethtool_cmd_speed_set(&ecmd, speed);
    }

    ecmd.cmd = ETHTOOL_SSET;
    ifr.ifr_data = (char *)&ecmd;
    if(ioctl(skfd, SIOCETHTOOL, &ifr) == -1)
    {
        close(skfd);
        return -1;
    }

    close(skfd);
    return 0;
}

/* If scanf supports size qualifiers for %n conversions, then we can
 * use a modified fmt that simply stores the position in the fields
 * having no associated fields in the proc string.  Of course, we need
 * to zero them again when we're done.  But that is smaller than the
 * old approach of multiple scanf occurrences with large numbers of
 * args. */

/* static const char * const ss_fmt[] = { */
/*  "%lln%llu%lu%lu%lu%lu%ln%ln%lln%llu%lu%lu%lu%lu%lu", */
/*  "%llu%llu%lu%lu%lu%lu%ln%ln%llu%llu%lu%lu%lu%lu%lu", */
/*  "%llu%llu%lu%lu%lu%lu%lu%lu%llu%llu%lu%lu%lu%lu%lu%lu" */
/* }; */

/* Lie about the size of the int pointed to for %n. */

#define _INT_MAX_   sizeof(int)
#define _LONG_MAX_      sizeof(long)

#if 1//(_INT_MAX_ == _LONG_MAX_)
static const char * const _ss_fmt[] =
{
    "%n%llu%u%u%u%u%n%n%n%llu%u%u%u%u%u",
    "%llu%llu%u%u%u%u%n%n%llu%llu%u%u%u%u%u",
    "%llu%llu%u%u%u%u%u%u%llu%llu%u%u%u%u%u%u"
};
#else
static const char * const _ss_fmt[] =
{
    "%n%llu%lu%lu%lu%lu%n%n%n%llu%lu%lu%lu%lu%lu",
    "%llu%llu%lu%lu%lu%lu%n%n%llu%llu%lu%lu%lu%lu%lu",
    "%llu%llu%lu%lu%lu%lu%lu%lu%llu%llu%lu%lu%lu%lu%lu%lu"
};
#endif

static int _procnetdev_version(char *buf)
{
    if(strstr(buf, "compressed"))
        return 2;
    if(strstr(buf, "bytes"))
        return 1;
    return 0;
}

static char *_get_if_name(char *name, char *pp)
{
    char *p;
    p = pp;
#define _IFNAMESIZE_        16
    /* Extract <name> from nul-terminated p where p matches
       <name>: after leading whitespace.
       If match is not made, set name empty and return unchanged p */
    int namestart=0, nameend=0;
    while(isspace(p[namestart]))
        namestart++;
    nameend=namestart;
    while(p[nameend] && p[nameend]!=':' && !isspace(p[nameend]))
        nameend++;
    if(p[nameend]==':')
    {
        if((nameend-namestart)<_IFNAMESIZE_)
        {
            memcpy(name,&p[namestart],nameend-namestart);
            name[nameend-namestart]='\0';
            p=&p[nameend];
        }
        else
        {
            /* Interface name too large */
            name[0]='\0';
        }
    }
    else
    {
        /* trailing ':' not found - return empty */
        name[0]='\0';
    }

    p = p+1;
    return p;
}

static void _get_dev_fields(char *bp, net_device_stats_t *stats, int procnetdev_vsn)
{
    memset(stats, 0, sizeof(struct __net_device_stats));
    sscanf(bp, _ss_fmt[procnetdev_vsn],
           &stats->rx_bytes, /* missing for 0 */
           &stats->rx_packets,
           &stats->rx_errors,
           &stats->rx_dropped,
           &stats->rx_fifo_errors,
           &stats->rx_frame_errors,
           &stats->rx_compressed, /* missing for <= 1 */
           &stats->rx_multicast, /* missing for <= 1 */
           &stats->tx_bytes, /* missing for 0 */
           &stats->tx_packets,
           &stats->tx_errors,
           &stats->tx_dropped,
           &stats->tx_fifo_errors,
           &stats->collisions,
           &stats->tx_carrier_errors,
           &stats->tx_compressed /* missing for <= 1 */
          );

    if(procnetdev_vsn <= 1)
    {
        if(procnetdev_vsn == 0)
        {
            stats->rx_bytes = 0;
            stats->tx_bytes = 0;
        }
        stats->rx_multicast = 0;
        stats->rx_compressed = 0;
        stats->tx_compressed = 0;
    }
}

int IPCFG_GetDevStat(char *if_name, net_device_stats_t *dev_stat)
{
    FILE *fh;
    char buf[512];
    int procnetdev_vsn;

    if(if_name == NULL)
    {
        IPCFG_DEBUG("get dev state : if_name is null \n");
        return -1;
    }

    fh = fopen("/proc/net/dev", "r");
    if(fh == NULL)
    {
        IPCFG_DEBUG("get dev state : open file error \n");
        return -1;
    }

    fgets(buf, sizeof(buf), fh);    /* eat line */
    fgets(buf, sizeof(buf), fh);

    procnetdev_vsn = _procnetdev_version(buf);

    while(fgets(buf, sizeof(buf), fh))
    {
        char *s, name[128];
        //IPCFG_DEBUG("while get dev state : bus %s  %d \n", buf, procnetdev_vsn);
        s = _get_if_name(name, buf);
        //IPCFG_DEBUG("while get dev state : name is %s s is %s \n", name, s);
        _get_dev_fields(s, dev_stat, procnetdev_vsn);
        if(!strcmp(if_name, name))
            break;
    }

    fclose(fh);
    return 0;
}

// if_name like "ath0", "eth0". Notice: call this function
// need root privilege.
// return value:
// -1 -- error , details can check errno
// 1 -- interface link up
// 0 -- interface link down.

int IPCFG_GetLinkStatus(char * if_name)
{
    int skfd;
    struct ifreq ifr;
    struct ethtool_value edata;

    edata.cmd = ETHTOOL_GLINK;
    edata.data = 0;

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, if_name, sizeof(ifr.ifr_name) - 1);
    ifr.ifr_data = (char *) &edata;

    if((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return -1;

    if(ioctl(skfd, SIOCETHTOOL, &ifr) == -1)
    {
        close(skfd);
        return -1;
    }

    close(skfd);
    return edata.data;
}

static int DHCP_Start = 0;
void IPCFG_InitDhcp()
{
    dhcp_init();
    register_dhcpok_callback(NULL);
    DHCP_Start = 0;
}

/* for Wireless Extension */
#if 0
extern unsigned char DRV_ReadIPAUTO_WE(void);
extern unsigned int DRV_ReadIPAddr_WE(void);
extern unsigned int DRV_ReadIPMask_WE(void);
extern unsigned int DRV_ReadIPGW_WE(void);
extern unsigned int DRV_ReadIPDNS_WE(void);
#endif
void IPCFG_ResetDhcp(char *if_name, char *our_opt60, int our_len, char *you_opt60, int you_len)
{
    int ret;
    unsigned char ip_auto = 0;
    unsigned int ip_addr, ip_mask, ip_gw, ip_dns;

    if(strcmp(if_name, "eth0") == 0 || strcmp(if_name, "eth1") == 0)
    {
       ip_auto = DRV_ReadIPAUTO();
	// ip_auto=0;	
        ip_addr = DRV_ReadIPAddr();
        ip_mask = DRV_ReadIPMask();
        ip_gw = DRV_ReadIPGW();
        ip_dns = DRV_ReadIPDNS();
    }
    else
    {
#if 0
        ip_auto = DRV_ReadIPAUTO_WE();
        ip_addr = DRV_ReadIPAddr_WE();
        ip_mask = DRV_ReadIPMask_WE();
        ip_gw = DRV_ReadIPGW_WE();
        ip_dns = DRV_ReadIPDNS_WE();
#endif
    }
    if(ip_auto)
    {
        if(DHCP_Start == 1)
        {
            dhcp_stop();
            sys_delay(500);
            /* ip clear*/
            IPCFG_SetIpAddr(if_name, 0x0);
            dhcp_start(if_name, our_opt60, our_len, you_opt60, you_len);
            printf("IPCFG_ResetDhcp IP auto,DHCP_Start = %d\n",DHCP_Start);
        }
        else
        {
            /* ip clear*/
            //IPCFG_SetIpAddr("eth0", 0xc0a804e3);
            IPCFG_SetIpAddr(if_name, 0x0);
            dhcp_start(if_name, our_opt60, our_len, you_opt60, you_len);
            printf("IPCFG_ResetDhcp IP auto,DHCP_Start = %d\n",DHCP_Start);
            DHCP_Start = 1;
        }

    }
    else
    {
        if(DHCP_Start == 1)
        {
            dhcp_stop();
            sys_delay(500);
            /* ip set /get ip from eeprom and set into system */
            IPCFG_SetIpAddr(if_name, ip_addr);
            IPCFG_SetSubMask(if_name, ip_mask);
            IPCFG_SetGateWay(if_name, ip_gw);
            IPCFG_SetDns(ip_dns);
            printf("IPCFG_ResetDhcp IP manual,DHCP_Start = %d\n",DHCP_Start);
            DHCP_Start = 0;
        }
        else
        {
            /* ip set */
            sys_delay(500);
            IPCFG_SetIpAddr(if_name, ip_addr);
            IPCFG_SetSubMask(if_name, ip_mask);
            IPCFG_SetGateWay(if_name, ip_gw);
            IPCFG_SetDns(ip_dns);
            printf("IPCFG_ResetDhcp IP manual,DHCP_Start = %d\n",DHCP_Start);
        }
    }

}

int IPCFG_GetDhcpStat(void)
{
    if(DHCP_Start == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int IPCFG_StopDhcp(void)
{
    if(DHCP_Start == 1)
    {
        dhcp_stop();
        sys_delay(500);
        DHCP_Start = 0;
        return 1;
    }
    else
    {
        //not start
        return 1;
    }
}

unsigned int IPCFG_GetHostByName(char *cname)
{
    struct hostent *p_hostent;
    char **pptr;
    struct in_addr ipaddr;

    p_hostent = gethostbyname(cname);
    if(p_hostent == NULL)
    {
        IPCFG_DEBUG("get host by name error -- return null \n");
        return 0;
    }
    else
    {
        //added by lzz 20091106
        pptr = p_hostent->h_addr_list;
        ipaddr = *(struct in_addr *)*pptr;
        return ipcfg_htonl(ipaddr.s_addr);
    }
}

DRV_EthLinkCallback Eth_Link_CallBack = NULL;

void IPCFG_RegEthLinkCallback(DRV_EthLinkCallback Callback_f)
{
    Eth_Link_CallBack = Callback_f;
}

/* 检查插拔网线操作，并调用注册的回调函数 */
static int eth_link_status = 0;
void link_state_check(void *arg)
{

    while(1)
    {

        if(eth_link_status != IPCFG_GetLinkStatus(eth_name))
        {
            eth_link_status = IPCFG_GetLinkStatus(eth_name);
            if(eth_link_status > 0)
            {
                IPCFG_DEBUG("[link_state_check] Link Up\n");
                //dm9000_reuse(); /* error */
                //set_PHY_Auto();/* error */
                if(Eth_Link_CallBack != NULL)
                {
                    Eth_Link_CallBack(TRUE);
                }

                //link_ok = 1;
            }
            else if(eth_link_status == 0)
            {
                IPCFG_DEBUG("[link_state_check] Link Down\n");
                if(Eth_Link_CallBack != NULL)
                {
                    Eth_Link_CallBack(FALSE);
                }

                //link_ok = 0;
            }
            else
            {
                /* error */
                IPCFG_DEBUG("[link_state_check]Link Error \n");
            }
        }
        sleep(1); /* delay 1 s */
    }
}
#if 1
/*----------------------------------------------------------------------------------------*
Function        :HI_S32 HI_ETH_Open(Eth_Port_E ePort)
Description     :switch to powerdown mode
Calls           :
Data Accessed   :
Data Updated    :
Input           :NA
Output          :NA
Return          :NA
Others          :NA
*----------------------------------------------------------------------------------------*/
int drv_Eth_Open(Eth_Port_E ePort)
{
    struct ifreq ifr;
    int sockfd;

    if(ETH_PORT_MAX <= ePort)
    {
        printf("invalid parameter:%d\n", ePort);
        return 0x80460004;
    }

    g_s32ETHFd[ePort] = 1;

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("socket create fail!\n");
        return -1;
    }

    strcpy(ifr.ifr_name,eth_name_array[ePort]);
    if((ioctl(sockfd,SIOCGIFFLAGS,(caddr_t)&ifr))==-1)
    {
        perror("SIOCGIFFLAGS fail :");
        close(sockfd);
        return -1;
    }

    ifr.ifr_flags |= IFF_UP;
    if((ioctl(sockfd,SIOCSIFFLAGS,(caddr_t)&ifr))==-1)
    {
        perror("SIOCSIFFLAGS fail :");
        close(sockfd);
        return -1;
    }

    close(sockfd);
    return 0;
}
#endif

#define MULIT_CONFIG_BASE  (0x10203000)
//static BOOL mIsFirstCall;
static HI_U32 g_au32SampleGpioRegValue=0 ;

//待硬件到位后再做严格测试，目前暂不进行相应测试?wlei add 20120712
//经测试，返回值为0代表上线，返回值为1代表没上线。wlei check 20120716
int Get_CM_Online_Status()
{

    DRV_ErrCode ErrCode = DRV_NO_ERROR;

    HI_SYS_ReadRegister(MULIT_CONFIG_BASE + 0x148, &g_au32SampleGpioRegValue); /*store old pin function define */
    g_au32SampleGpioRegValue &= 0xFFFFFFFc;
    HI_SYS_WriteRegister(MULIT_CONFIG_BASE + 0x148, g_au32SampleGpioRegValue);

    HI_BOOL status_value;

    ErrCode = HI_UNF_GPIO_Open();
    if(HI_SUCCESS != ErrCode)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ErrCode);
        return ErrCode;
    }

    ErrCode = HI_UNF_GPIO_SetDirBit(11*8+2, HI_TRUE);   /*set GPIO11_3 direction is output, very important wlei*/

    ErrCode=HI_UNF_GPIO_ReadBit(11*8+2,&status_value); //read online status value
    if(HI_SUCCESS != ErrCode)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ErrCode);
        HI_UNF_GPIO_Close();
        return ErrCode;
    }

    if(HI_SUCCESS != ErrCode)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ErrCode);
        HI_UNF_GPIO_Close();
        return ErrCode;
    }

    return status_value;

}

/*设置使用的网口: Eth=0 使用eth0；Eth=1 使用eth1*/
void IPCFG_SetEth(unsigned char Eth)
{
	if(0 == Eth)
	{
		eth_name = eth_name_array[0];
	}
	else
	{
		eth_name = eth_name_array[1];
	}

	return;
}

void IPCFG_Init()
{
    unsigned char mac_addr[6];
    unsigned char mac_addr2[6];
    Eth_Port_E eth = ETH_PORT_UP;
    DRV_ReadMACAddr(mac_addr);
    printf("[IP] EEPROM MAC Addr is %x--%x--%x--%x--%x--%x \n", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3],mac_addr[4],mac_addr[5]);
    IPCFG_GetMacAddr(eth_name, mac_addr2);
    printf("[IP] SYSTEM MAC Addr is %x--%x--%x--%x--%x--%x \n", mac_addr2[0], mac_addr2[1], mac_addr2[2], mac_addr2[3],mac_addr2[4],mac_addr2[5]);
    IPCFG_SetMacAddr(eth_name, (char *)mac_addr);
    sys_new_thread(link_state_check, NULL, 1, "link_check");
    IPCFG_InitDhcp();
    drv_Eth_Open(eth);
    return;
}
//********************************************************

void DRV_GetMAC(unsigned char *AddrMAC)
{
    IPCFG_GetMacAddr(eth_name,AddrMAC);
}

unsigned int DRV_GetIPAddr_InUse(void)
{
    return IPCFG_GetIpAddr(eth_name);
}

unsigned int DRV_GetMask_InUse(void)
{
    return IPCFG_GetSubMask(eth_name);
}

unsigned int DRV_GetGW_InUse(void)
{
    return IPCFG_GetGateWay();
}

unsigned int DRV_GetDNS_InUse(void)
{
    return  IPCFG_GetDns();
}

unsigned int DRV_GetDNSEx_InUse()
{
    unsigned int dnsaddrs[2]= {0};

    IPCFG_GetDnsEx(dnsaddrs);

    return dnsaddrs[1];
}

void DRV_IP_RESET(void)
{
	IPCFG_ResetDhcp(eth_name, NULL, 0, NULL, 0);
	//Err_Print("\n\n\n\n\n[WARRING] Debug Mode, DRV_IP_RESET Do Not Support!!!\n\n\n\n");
}


