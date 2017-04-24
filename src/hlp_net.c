/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */

#include <hlp_config.h>
#include <hlp_core.h>
#include <hlp_ring.h>
#include <hlp_net.h>
#include <hlp_log.h>
 
#define  HLP_NET_PRINT_SERVER_PORT     7078
#define  HLP_NET_PRINT_FIFO_MAX        128


static hlp_ring_t *g_ringbuffer = NULL;
static Server_info_t g_Server_info;
static int bServerInit = 0;
int gTcpPortNum = HLP_NET_PRINT_SERVER_PORT;
static int gSockFd = -1;

void GetPkt(hlp_buf_t *buffer)		
{
	ring_pkt_get(g_ringbuffer, buffer);
}

void PutPkt(hlp_buf_t *buffer)
{
	ring_pkt_put(g_ringbuffer, buffer);
}

int GetPktCnt()
{
	return g_ringbuffer->count;
}

int hlp_net_print_set_port(int portNum)
{
	if(portNum < 0 || portNum > 65535)
	{
		printf("setTcpPort error:  portNum=%d\n", portNum);
		return 0;
	}
	gTcpPortNum = portNum;
	printf("setTcpPort: %d\n", gTcpPortNum);
	return 0;
}

void sigroutine(int signo)
{
	printf("Ctrl+c 1....\n");
	close(gSockFd);
	printf("Ctrl+c 2....\n");
	free(g_ringbuffer);
	printf("Ctrl+c 3....\n");
	exit(-1);
}

/* 创建socket */	
static int hlp_net_print_sock_init()
{
	int  fd = 0;
	int  opt = 0;
	struct sockaddr_in    server_addr;
	
	/* 忽略SIGPIPE信号，否则会退出整个应用 */	
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, sigroutine);
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family=AF_INET; 
    server_addr.sin_port=htons(gTcpPortNum); 
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	g_Server_info.addr = server_addr;

	fd = socket(AF_INET, SOCK_STREAM, 0);	
	if (fd < 0)	
	{		
		perror("hlp_net_print_sock_init socket:");
		LOG_WRITE_ERR("hlp_net_print_sock_init socket: %s\n", strerror(errno));
		return -1;	
	}
	
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		perror("hlp_net_print_sock_init setsockopt SO_REUSEADDR error:");
		LOG_WRITE_ERR("hlp_net_print_sock_init setsockopt SO_REUSEADDR socket: %s\n", strerror(errno));
		return -1;
	}
	printf("hlp_net_print_sock_init setsockopt SO_REUSEADDR \n");
	LOG_WRITE_ERR("hlp_net_print_sock_init setsockopt SO_REUSEADDR \n");

	/* 绑定*/	
	if (bind(fd, (struct sockaddr *)&(server_addr), sizeof(struct sockaddr)) < 0)	
	{		
		perror("hlp_net_print_sock_init bind");
		LOG_WRITE_ERR("hlp_net_print_sock_init setsockopt bind: %s\n", strerror(errno));
		exit(-1);	
	}	
	/* 监听*/	
	if (listen(fd, 2) < 0)	
	{		
		perror("hlp_net_print_sock_init listen");
		LOG_WRITE_ERR("hlp_net_print_sock_init listen : %s\n", strerror(errno));
		return -1;	
	}

	gSockFd = fd;
	return fd;
	
}

#if 0
#define PKT_STATUS_HEAD    100
#define PKT_STATUS_DATA    101
#define PKT_STATUS_END     102

int recv_process(Connect_info_t *c, char *rBuffer, int len)
{
	int dataLen;

	switch(c->recv_status)
	{
		case PKT_STATUS_HEAD:
		dataLen = *((short*)rBuffer);
		dataLen = SWAP16(dataLen);
		c->recv_data_len = dataLen;
		c->recv_total_len = dataLen+2;
		printf("__recv hlp_ring_thead, pktTotalLen=%d, recvLen=%d.\n", dataLen, len );
		memset(c->recv_buf, 0 , TCP_BUF_SIZE);
		memcpy(c->recv_buf, rBuffer, len);
		c->recv_data_size = len-2;
		if(dataLen <= len-2)
		{
			c->recv_status = PKT_STATUS_END;
			return PKT_STATUS_END;
		}
		else
		{
			c->recv_status = PKT_STATUS_DATA;
			return PKT_STATUS_DATA;
		}	
		break;
		case PKT_STATUS_DATA:
			memcpy(c->recv_buf+c->recv_data_size+2, rBuffer, len);
			c->recv_data_size += len;
			if(c->recv_data_size == c->recv_data_len)
			{
				printf("recv done!!!!!!\n");
				c->recv_status = PKT_STATUS_END;
				return PKT_STATUS_END;
			}
			return PKT_STATUS_DATA;
		break;
		default:
			printf("c->pkt_status error!!!!\n");
			return -1;
			break;
	}
}
#endif

void nprintf(char *Msg)
{
	hlp_buf_t packet;
	packet.len = strlen(Msg);
	memcpy(packet.data, Msg, packet.len);
	PutPkt(&packet);
	//printf("sadfsaf\n");
}

void hlp_net_print_client_task(void *args)
{
	hlp_buf_t  packet;
	Connect_info_t *c;

	c = (Connect_info_t *)args;

	g_Server_info.client_count++;
	while(1)
	{
		if(GetPktCnt() > 0)
		{
			memset((void*)&packet, 0, sizeof(hlp_ring_t));
			GetPkt(&packet);
			if(write(c->sockfd, packet.data, packet.len) < 0)
			{
				close(c->sockfd);
				perror("hlp_net_print_client_task write:");
				LOG_WRITE_ERR("hlp_net_print_client_task write : %s\n", strerror(errno));
				break;
			}

		}
		
		hlp_taskdelay_ms(100);
	}
	printf("client task exit..\n");
	LOG_WRITE_ERR("client task exit..\n");
}

void hlp_net_print_task()
{
	int   ret;
	int   sockfd = -1;
	int   new_sockfd = -1;
	struct sockaddr_in    client_addr;
	int   sin_size = sizeof(struct sockaddr_in);

	Connect_info_t  *new_connect;

	//pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	//pthread_setcanceltype(PTHREAD_CANCEL_ASYCHRONOUS, NULL);
	//sleep(2);
	sockfd = hlp_net_print_sock_init();
	if(sockfd < 0)
		return ;

	g_Server_info.sockfd = sockfd;
	g_Server_info.client_count = 0;
	printf("server [%s:%d] start success\r\n", "0.0.0.0", HLP_NET_PRINT_SERVER_PORT);
	LOG_WRITE_ERR("server [%s:%d] start success\r\n", "0.0.0.0", HLP_NET_PRINT_SERVER_PORT);

	while(1)
	{
		//pthread_testcancel();
		printf("Wait new connecting...\n");
		if ((new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, (socklen_t*)&sin_size)) == -1)
		{
			perror("accept");
			return;		
		}
		//pthread_testcancel();
		printf("New connenct incoming..! fd=%d\n", new_sockfd);
		new_connect = (Connect_info_t*)malloc(sizeof(Connect_info_t));
		if(new_connect == NULL)
		{
			printf("malloc(Connect_info_t) error: no enough memory!!\n");
			return ;
		}
		memset(new_connect, 0 , sizeof(Connect_info_t));
		//new_connect->recv_status = PKT_STATUS_HEAD;
		new_connect->sockfd= new_sockfd;
		new_connect->addr = client_addr;
		ret = pthread_create(&new_connect->task, NULL, (void * (*)(void*)) hlp_net_print_client_task, (void*)new_connect);
		if(ret < 0)
		{
			perror("pthread_create error:");
			continue;
		}
		printf("Start new tcpClientTask success! [0x%x]\n", (int)&new_connect->task);
	}
}

int hlp_net_print_init()
{
	int ret;
	if(bServerInit == 0)
	{
		g_ringbuffer = hlp_ring_create(HLP_NET_PRINT_FIFO_MAX);
		if(g_ringbuffer == NULL)
			return -1;
		bServerInit = 1;
	}
	
	ret = pthread_create(&g_Server_info.task, NULL ,(void * (*)(void*))hlp_net_print_task, NULL);
    if (ret < 0)
    {
        printf("hlp_net_print_Init: pthread_create hlp_net_print_task is fail!\n");
		LOG_WRITE_ERR("hlp_net_print_Init: pthread_create hlp_net_print_task is fail!\n");
        return -1;
    }

	return 0;
}

