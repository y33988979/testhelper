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
#include "dhcp_socket.h"
#include "ip_cfg.h"
#include "dhcp_client.h"

SOCKET dhcp_socket_register(void)
{
#if defined(__WIN32__)
{
    //版本信息确认
	WORD  wVersionRequested;
	WSADATA  wsaData;
	int err;
    SOCKET socketfd;
    SOCKADDR_IN  addrSrv;
	
	wVersionRequested=MAKEWORD(1,1);
	err=WSAStartup(wVersionRequested,&wsaData);
	if(err!=0){
        DHCP_DEBUG("windows socket start up error \n");
		return DHCP_SOCK_ERR;
	}
	if(LOBYTE(wsaData.wVersion )!=1 ||
		HIBYTE(wsaData.wVersion )!=1 )
	{
		WSACleanup();
        DHCP_DEBUG("windows socket version error \n");
		return DHCP_SOCK_ERR;
	}
	
	//主体
	socketfd = socket(AF_INET,SOCK_DGRAM,0);
    if(socketfd < 0){
        DHCP_DEBUG("[dhcp_client]dhcp_client windows socket error! \n");
        WSACleanup();
        return DHCP_SOCK_ERR;
    }
    
    dhcp_desp.sockfd = socketfd;
    
    //地址信息设置
	addrSrv.sin_addr .S_un .S_addr =ipcfg_htonl(INADDR_ANY);
	addrSrv.sin_family =AF_INET;
	addrSrv.sin_port =ipcfg_htons(DHCP_CLIENT_PORT);
	//绑定到特定端口
	err = bind(sockSrv,(SOCKADDR *)&addrSrv,sizeof(SOCKADDR));
    if(err < 0){
        closesocket(socketfd);
        DHCP_DEBUG("[dhcp_client]dhcp_client windows socket bind error! \n");
        WSACleanup();
        return DHCP_SOCK_ERR;
    }
}
#endif

#if defined(__LINUX__)
{
    int ret;
    int len;
    int socketfd;
    struct sockaddr_in local_addr;
    int flag;

    //IPCFG_SetIpAddr("eth0", 0xc0a80564);
    //STBIp_Set("192.168.4.178");
    //STBIp_Set("0.0.0.0");
    //memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET; //address family
    //local_addr.sin_addr.s_addr = ipcfg_htonl(0xc0a804b2); // ip addr
    local_addr.sin_addr.s_addr = INADDR_ANY;//ipcfg_htonl(0x00000000); // ip addr
    local_addr.sin_port = ipcfg_htons(DHCP_CLIENT_PORT); //port number
    
    //len = sizeof(remote_addr);
    
    socketfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(socketfd < 0){
        DHCP_DEBUG("[dhcp_client]dhcp_client socket error! \n");
        return DHCP_SOCK_ERR;
    }

    /* Set the REUSEADDR option so that we don't fail to start if
	   we're being restarted. */
    flag = 1;
    if (setsockopt (socketfd, SOL_SOCKET, SO_REUSEADDR,
                    (char *)&flag, sizeof(flag)) < 0){
        DHCP_DEBUG("Can't set SO_REUSEADDR option on dhcp socket: %m \n");
    }

    /* Set the BROADCAST option so that we can broadcast DHCP responses. */
    if (setsockopt (socketfd, SOL_SOCKET, SO_BROADCAST,
                    (char *)&flag, sizeof(flag)) < 0){
        DHCP_DEBUG("Can't set SO_BROADCAST option on dhcp socket: %m \n");
    }
    
    ret = bind(socketfd,(struct sockaddr*)&local_addr,sizeof(local_addr));
    if(ret < 0){
        close(socketfd);
        DHCP_DEBUG("[dhcp_client]dhcp_client socket bind error! \n");
        return DHCP_SOCK_ERR;
    }

    return socketfd;
}
#endif

    return 0;
}

/*
   socket register ex--
*/
SOCKET dhcp_socket_register_ex(u32_t ip, int port, char *inf)
{
	struct ifreq interface;
	int fd;
	struct sockaddr_in addr;
	int n = 1;

	DHCP_DEBUG("Opening listen socket on 0x%08x:%d %s", ip, port, inf);
	if ((fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		DHCP_DEBUG("socket call failed: %m");
		return -1;
	}
    
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = ipcfg_htons(port);
	addr.sin_addr.s_addr = ip;

	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *) &n, sizeof(n)) == -1) {
		close(fd);
		return -1;
	}
	if (setsockopt(fd, SOL_SOCKET, SO_BROADCAST, (char *) &n, sizeof(n)) == -1) {
		close(fd);
		return -1;
	}

	strncpy(interface.ifr_name, inf, IFNAMSIZ);
	if (setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE,(char *)&interface, sizeof(interface)) < 0) {
		close(fd);
		return -1;
	}

	if (bind(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr)) == -1) {
		close(fd);
		return -1;
	}

	return fd;
}

int dhcp_socket_send(SOCKET sock, char *sendbuf, int sendsize, int sendflag, 
                            struct sockaddr *remote_addr, int socksize)
{
    int len;
    len = sendto(sock, sendbuf, sendsize, sendflag, remote_addr, socksize);
    if(len < 0)
    {
        DHCP_DEBUG(" dhcp_socket: send packet error !\n");
    }
    return len;
}

int dhcp_socket_recv(SOCKET sock, char *recvbuf, int recvsize, int recvflag)
{
    int len;
    
    if((len = recv(sock, recvbuf, recvsize, recvflag)) < 0){
        DHCP_DEBUG(" dhcp_socket: recv fail \n");
    }
    return len;
}

int dhcp_socket_select(SOCKET sock, fd_set *rfds, fd_set *wfds, fd_set *efds, struct timeval *tv)
{
    int ret;
    ret = select(sock, rfds, wfds, efds, tv);
    return ret;
}

