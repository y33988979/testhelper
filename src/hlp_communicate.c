/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */
 

#include <hlp_net.h>
#include <hlp_log.h>
#include <hlp_gpio.h>
#include <hlp_common.h>
#include <hlp_string.h>
#include <hlp_communicate.h>

#define HLP_COMMU_SEVER_IPADDR    "192.168.207.1"

static int gCommuSocketFd = -1;
static int gCommuTcpPortNum = 55555;
static Server_info_t gCommuServerInfo;

static int tcp_connect_count = 0;
hlp_bool_t bRecvHeartbeat = HLP_FALSE;
hlp_bool_t bTcpConnected = HLP_FALSE;
hlp_bool_t bSwitchtaskIsDetect = HLP_FALSE;
hlp_bool_t bTcpClientIsExit = HLP_FALSE;
hlp_bool_t bTcpClientExitDone = HLP_FALSE;

hlp_bool_t  bCommuTcpServerStart = HLP_FALSE;

void hlp_commu_tcp_server_start()
{
	bCommuTcpServerStart = HLP_TRUE;
}

void hlp_commu_tcp_server_stop()
{
	bCommuTcpServerStart = HLP_FALSE;
}

void hlp_commu_tcp_client_start()
{
	bTcpClientIsExit = HLP_FALSE;
}

void hlp_commu_tcp_client_exit()
{
	bTcpClientIsExit = HLP_TRUE;
}

void  hlp_commu_client_exit()
{
	if(gCommuServerInfo.client_count > 0)
	{
		printf("client_count=%d\n", gCommuServerInfo.client_count);
		LOG_WRITE_ERR("client_count=%d\n", gCommuServerInfo.client_count);
		hlp_commu_tcp_client_exit();
	}
}

void switch_onoff_wait_tcpclient_exit(hlp_u32_t timeout)
{
	int i = 0;

	/* timeout = 500  (ms) , value>=100 */
	timeout = timeout/100;
	for(i=0; i<timeout; i++)
	{
		if(bTcpClientExitDone == HLP_TRUE)
		{
			break;
		}
		else
		{
			hlp_taskdelay_ms(100);
		}
	}
}

int hlp_commu_sock_init()
{
	int  fd = 0;
	int  opt = 0;
	struct sockaddr_in    server_addr;
	
	/* 忽略SIGPIPE信号，否则会退出整个应用 */	
	signal(SIGPIPE, SIG_IGN);
	//signal(SIGINT, sigroutine);
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family=AF_INET; 
    server_addr.sin_port=htons(gCommuTcpPortNum); 
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	gCommuServerInfo.addr = server_addr;

	fd = socket(AF_INET, SOCK_STREAM, 0);	
	if (fd < 0)	
	{		
		perror("socket");
		return -1;	
	}
	
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_BROADCAST, &opt, sizeof(opt)) < 0)
	{
		perror("setsockopt SO_REUSEADDR error:");
		return -1;
	}
	printf("setsockopt SO_REUSEADDR \n");
	LOG_WRITE_ERR("hlp_uart_send_command STB_HeartBeat!!!\n");

	/* 绑定*/	
	if (bind(fd, (struct sockaddr *)&(server_addr), sizeof(struct sockaddr)) < 0)	
	{		
		perror("bind");
		LOG_WRITE_ERR("bind error: %s", strerror(errno));
		exit(-1);	
	}	
	/* 监听*/	
	if (listen(fd, 2) < 0)	
	{		
		perror("listen");
		LOG_WRITE_ERR("listen error: %s", strerror(errno));
		return -1;	
	}

	gCommuSocketFd = fd;
	printf("server [%s:%d] sock init success\r\n", inet_ntoa(server_addr.sin_addr), gCommuTcpPortNum);
	LOG_WRITE_ERR("server [%s:%d] sock init success\r\n", inet_ntoa(server_addr.sin_addr), gCommuTcpPortNum);
	return fd;
}

void hlp_commu_client_task(void *args)
{
	hlp_s16_t       nwrite, nread,i;
	hlp_u8_t        wBuffer[16] = {0x40, 0x23, 0x11, 0x00, 0x00, 0x00};
	//hlp_u8_t        wBuffer[16] = { 0x23, 0x11,0x40, 0x00, 0x00, 0x00};
	hlp_u8_t        rBuffer[16] = {0};
	hlp_u16_t       wcount = 0, rcount = 0;
	hlp_s16_t       ret = -1;
	Connect_info_t *c;
	struct timeval  tv_out;
	tv_out.tv_sec = 2;//等待2秒
	tv_out.tv_usec = 0;

	gCommuServerInfo.client_count++;
	c = (Connect_info_t *)args;

	pthread_detach(pthread_self());
	
	printf("client start! sockfd=%d.\n", c->sockfd);
	LOG_WRITE_ERR("client start! sockfd=%d.\n", c->sockfd);
	
	#if 1 /* 设置recv send 超时时间*/
	ret = setsockopt(c->sockfd,SOL_SOCKET,SO_SNDTIMEO,(char*)&tv_out,sizeof(tv_out));
	if(ret < 0)
	{
		perror("setsockopt SO_SNDTIMEO");
		LOG_WRITE_ERR("setsockopt SO_SNDTIMEO: %s", strerror(errno));
		//return -1;
	}
	ret = setsockopt(c->sockfd,SOL_SOCKET,SO_RCVTIMEO,(char*)&tv_out,sizeof(tv_out));
	if(ret < 0)
	{
		perror("setsockopt SO_RCVTIMEO");
		LOG_WRITE_ERR("setsockopt SO_RCVTIMEO: %s", strerror(errno));
		//return -1;
	}
	#endif

	//printf("Tcp client: setsockopt SO_SNDTIMEO SO_RCVTIMEO !\n");
	//LOG_WRITE_ERR("Tcp client: setsockopt SO_SNDTIMEO SO_RCVTIMEO !\n");

	bRecvHeartbeat = HLP_FALSE;
	bTcpClientIsExit = HLP_FALSE;
	bTcpClientExitDone = HLP_FALSE;
	bTcpConnected = HLP_TRUE;
	
	while(1)
	{
		/* 如果开关机任务已经检测到心跳，关闭socket，退出client线程 */
		if(bTcpClientIsExit == HLP_TRUE)
		{
			break;
		}
		
		hlp_memset(rBuffer, 0, sizeof(rBuffer));
		//printf("write before!!!\n");
		//LOG_WRITE_ERR("write before!!!\n");
		nwrite = write(c->sockfd, wBuffer, 6);
		if(nwrite < 0)
		{
			perror("write error:");
			LOG_WRITE_ERR("write error: %s", strerror(errno));
			break;
			//exit(1);			
		}
		else
		{
			printf("send heartbeat!!ret=%d, cnt=%d\n", nwrite, wcount++);
			LOG_WRITE_ERR("send heartbeat!!ret=%d, cnt=%d\n", nwrite, wcount);
		}
		nread = read(c->sockfd, rBuffer, 10);
		if(nwrite < 0)
		{
			perror("read error:");
			LOG_WRITE_ERR("read error: %s", strerror(errno));
			break;
			//exit(1);
		}
		else
		{
			printf("recv heartbeat!!ret=%d, cnt=%d\n", nread, rcount++);
			LOG_WRITE_ERR("recv heartbeat!!ret=%d, cnt=%d\n", nread, rcount);
			bRecvHeartbeat = HLP_TRUE;
			#if 0
			for(i=0; i<nread; i++)
			{
				printf("0x%02x ", rBuffer[i]);
				LOG_WRITE_ERR("0x%02x ", rBuffer[i]);
			}		
			printf("\n");
			LOG_WRITE_ERR("\n");
			#endif			
		}
		sleep(3);
	}

	printf("client exit..fd=%d, tcp_cnt=%d\n", c->sockfd, tcp_connect_count++);
	LOG_WRITE_ERR("client exit..fd=%d, tcp_cnt=%d\n", c->sockfd, tcp_connect_count);
	close(c->sockfd);
	free(c);
	//bTcpClientExitDone = HLP_TRUE;
	bTcpConnected = HLP_FALSE;
	bRecvHeartbeat = HLP_FALSE;
	bTcpClientIsExit = HLP_FALSE;
	bTcpClientExitDone = HLP_FALSE;
	gCommuServerInfo.client_count--;
}


void hlp_commu_task()
{
	int   ret;
	int   sockfd = -1;
	int   new_sockfd = -1;
	struct sockaddr_in    client_addr;
	int   sin_size = sizeof(struct sockaddr_in);
	pthread_attr_t      attr;
	Connect_info_t  *new_connect;

	//pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	//pthread_setcanceltype(PTHREAD_CANCEL_ASYCHRONOUS, NULL);
	//sleep(2);
	sockfd = hlp_commu_sock_init();
	if(sockfd < 0)
		return ;
	printf("socket[commu] init success! fd=%d\n", sockfd);
	LOG_WRITE_ERR("socket[commu] init success! fd=%d\n", sockfd);
	gCommuServerInfo.sockfd = sockfd;
	gCommuServerInfo.client_count = 0;
//hlp_commu_tcp_server_start();//hahahahhahahaahah
//hlp_power_switch_on();
	while(1)
	{
		/* 开关机任务没有启动时，或已经创建了连接，不接受新连接 */
		if(bCommuTcpServerStart == HLP_FALSE)
		{
			hlp_taskdelay_ms(500);
			continue;
		}
		
		/* 接受新连接 */
		//pthread_testcancel();
		printf("Wait new connecting...\n");
		LOG_WRITE_ERR("Wait new connecting...\n");
		if ((new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, (socklen_t*)&sin_size)) == -1)
		{
			perror("accept");
			return;		
		}
		//pthread_testcancel();
		printf("New connenct incoming..! fd=%d\n", new_sockfd);
		LOG_WRITE_ERR("New connenct incoming..! fd=%d\n", new_sockfd);
		new_connect = (Connect_info_t*)malloc(sizeof(Connect_info_t));
		if(new_connect == NULL)
		{
			printf("malloc(Connect_info_t) error: no enough memory!!\n");
			LOG_WRITE_ERR("malloc(Connect_info_t) error: no enough memory!!\n");
			return ;
		}
		memset(new_connect, 0 , sizeof(Connect_info_t));
		//new_connect->recv_status = PKT_STATUS_HEAD;
		new_connect->sockfd= new_sockfd;
		new_connect->addr = client_addr;
		pthread_attr_setstacksize(&attr, 2*1024);
		ret = pthread_create(&new_connect->task, &attr, (void * (*)(void*)) hlp_commu_client_task, (void*)new_connect);
		if(ret != 0)
		{
			perror("pthread_create error:");
			LOG_WRITE_ERR("pthread_create error: %s", strerror(errno));
			continue;
		}
		printf("Start new tcpClientTask success! [0x%x]\n", (int)&new_connect->task);
		LOG_WRITE_ERR("Start new tcpClientTask success! [0x%x]\n", (int)&new_connect->task);
		hlp_taskdelay_ms(200);
		if(gCommuServerInfo.client_count == 1)
		{
			hlp_commu_tcp_server_stop();	//hhahahahahahaha
		}
	}
}

int hlp_commu_init()
{
	int         ret;
	pthread_t   tid;

	//ret = system("ifconfig eth0 192.168.207.1");
	//if(ret < 0)
		//printf("ifconfig error!\n");
	
	ret = pthread_create(&tid, NULL ,(void * (*)(void*))hlp_commu_task, NULL);
    if (ret < 0)
    {
    	perror("pthread_create:");
        printf("pthread_create hlp_commu_task is fail!\n");
		LOG_WRITE_ERR("pthread_create hlp_commu_task is fail! %s\n", strerror(errno));
        return -1;
    }

	return 0;
}


