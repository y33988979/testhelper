/*----------------------------------------------------------------------------
 *  Include
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 在此包含其他模块的头文件，自己的头文件放在函数定义之前
*----------------------------------------------------------------------------*/

#include <string.h>
#include "drv_basic.h"
#include "drv_radio.h"
#include "drv_os.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include "ysoslist.h"
 
//#define SWAP16(x) ((short)((((short)(x)&(short)0x00ffU)<< 8) |(((short)(x)&(short)0xff00U)>> 8)))
#define SWAP16(x) ((short)(((short)((x&0xff)<< 8)) | ((short)((x&0xff00)>> 8))))
#define SWAP32(x) (((x&0xff)<<24)|((x&0xff00)<<8)|((x&0xff0000)>>8)|((x&0xff000000)>>24))

#define  SERVER_PORT     7078
#define  TCP_BUF_SIZE    128
#define  FIFO_MAX        64

typedef struct _Pkt
{
	int sockfd;
	int dataLen;
	char data[128];
}Pkt;

struct ring_buffer
{
	uint32_t      size;       //大小
	uint32_t      write;       //入口位置
	uint32_t      read;        //出口位置
	uint32_t      count;      //数据包个数
	Pkt           pkts[FIFO_MAX];     //缓冲区
	pthread_mutex_t *f_lock;    //互斥锁
};

typedef struct _Server_info_t
{
	int   sockfd;
	struct sockaddr_in addr;
	DRVOS_Task_t *task;
	/* 服务器的客户端数量 */
	int client_count;
}Server_info_t;

typedef struct _Connect_info_t
{
	int   sockfd;
	struct sockaddr_in addr;
	DRVOS_Task_t *task;

	int recv_status;
	/* 需要发送的数据的长度*/
	int send_len;
	/* 接收到的数据长度 */
	int recv_total_len;
	int recv_data_len;
	int recv_data_size;
	/* 发送数据缓存*/
	unsigned char send_buf[TCP_BUF_SIZE];
	/* 接收数据缓存 */
	unsigned char recv_buf[TCP_BUF_SIZE];
	/* */
	unsigned char temp_buf[TCP_BUF_SIZE];
}Connect_info_t;

static struct ring_buffer *g_ringbuffer = NULL;
static pthread_mutex_t g_pktLock = PTHREAD_MUTEX_INITIALIZER;
static list_t g_TcpClientList;
static Server_info_t g_Server_info;
static int bServerInit = 0;
int gTcpPortNum = SERVER_PORT;

/*----------------------------------------------------------------------------
 *  Debug
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 *  External Function Prototype
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 引用的外部函数原型
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 *  External variable
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *  Struct
 *----------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------
 *  Private Constant & Enum & Macro
 *----------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------
 *  Global variable
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *  Private Module Variable
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 必须定义为static变量，防止外部引用
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *  Private Function Prototype
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 必须定义为static变量，防止外部引用
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
* 在此包含自己的头文件，以确保头文件中所有类型都已声明
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 *  Function
 *----------------------------------------------------------------------------*/

#define is_power_of_2(x) ((x) != 0 && (((x) & ((x) - 1)) == 0))

//初始化缓冲区
struct ring_buffer* ring_buffer_init(uint32_t size, pthread_mutex_t *f_lock)
{
	//assert(buffer);
	struct ring_buffer *ring_buf = NULL;
	if (!is_power_of_2(size))
	{
		fprintf(stderr,"size must be power of 2.\n");
		return ring_buf;
	}
	ring_buf = (struct ring_buffer *)malloc(sizeof(struct ring_buffer));
	if (!ring_buf)
	{
		fprintf(stderr,"Failed to malloc memory,errno:%u,reason:%s",
		errno, strerror(errno));
		return ring_buf;
	}
	memset(ring_buf, 0, sizeof(struct ring_buffer));
	ring_buf->size = size;
	ring_buf->count = 0;
	ring_buf->write = 0;
	ring_buf->read = 0;
	ring_buf->f_lock = f_lock;
	return ring_buf;
}

//释放缓冲区
void ring_buffer_free(struct ring_buffer *ring_buf)
{
	if (ring_buf)
	{
		free(ring_buf);
		ring_buf = NULL;
	}
}

//缓冲区的长度
uint32_t ring_get_count(struct ring_buffer *ring_buf)
{
	return ring_buf->count;
}

//从缓冲区中取数据
uint32_t ring_pkt_get(struct ring_buffer *ring_buf, Pkt *pkt)
{
	//assert(pkt);
	if(ring_buf->count == 0)
	{
		printf("pkt fifo is empty!!\n");
		return -1;
	}
	memcpy((void*)pkt, (void*)&ring_buf->pkts[ring_buf->read], sizeof(Pkt));
	ring_buf->read = (ring_buf->read + 1) % FIFO_MAX;
	ring_buf->count--;
	return 0;
	
}
//向缓冲区中存放数据
uint32_t ring_pkt_put(struct ring_buffer *ring_buf, Pkt *pkt)
{
	//assert(pkt);
	if(ring_buf->count == ring_buf->size-1)
	{
		printf("pkt fifo is full!!\n");
		return -1;
	}
	memcpy((void*)&ring_buf->pkts[ring_buf->write], (void*)pkt, sizeof(Pkt));
	ring_buf->write = (ring_buf->write + 1) % FIFO_MAX;
	ring_buf->count++;
	return 0;
}

void GetPkt(Pkt *pkt)		
{
	ring_pkt_get(g_ringbuffer, pkt);
}

void PutPkt(Pkt *pkt)
{
	ring_pkt_put(g_ringbuffer, pkt);
}

int GetPktCnt()
{
	return g_ringbuffer->count;
}

int setTcpPort(int portNum)
{
	if(portNum < 0 || portNum > 65535)
	{
		printf("setTcpPort error:  portNum=%d\n", portNum);
		return 0;
	}
	gTcpPortNum = portNum;
	printf("setTcpPort: %d\n", gTcpPortNum);
}

/* 创建socket */	
static int sock_init()
{
	int  fd = 0;
	int  opt = 1;
	struct sockaddr_in    server_addr;
	
	/* 忽略SIGPIPE信号，否则会退出整个应用 */	
	signal(SIGPIPE, SIG_IGN);
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family=AF_INET; 
    server_addr.sin_port=htons(gTcpPortNum); 
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	g_Server_info.addr = server_addr;

	fd = socket(AF_INET, SOCK_STREAM, 0);	
	if (fd < 0)	
	{		
		perror("socket");
		return -1;	
	}
	/* 绑定*/	
	if (bind(fd, (struct sockaddr *)&(server_addr), sizeof(struct sockaddr)) < 0)	
	{		
		perror("bind");		
		return -1;	
	}	
	/* 监听*/	
	if (listen(fd, 10) < 0)	
	{		
		perror("listen");
		return -1;	
	}

	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		perror("setsockopt SO_REUSEADDR");
		return -1;
	}

	return fd;
	
}

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
		printf("__recv pkt head, pktTotalLen=%d, recvLen=%d.\n", dataLen, len );
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

void tcpClientTask(void *args)
{
	int len = 0, i;
	int pkt_status = 0;
	char  rBuffer[64];
	Pkt   packet;
	Connect_info_t *c;

	c = (Connect_info_t *)args;

	ListAddEnd(&g_TcpClientList, c);
	g_Server_info.client_count++;
	while(1)
	{
		if(GetPktCnt() > 0)
		{
			memset((void*)&packet, 0, sizeof(Pkt));
			GetPkt(&packet);
			if(write(c->sockfd, packet.data, packet.dataLen) < 0)
				perror("write:");
		}
		usleep(20);
	}
	DRV_OSFree(0xFFFFFFFF, c->task);
	c->task = NULL;
	ListDeleteItem(&g_TcpClientList, c);
	printf("client task exit..\n");
}

void tcpServerTask()
{
	int   sockfd = -1;
	int   new_sockfd = -1;
	struct sockaddr_in    client_addr;
	int   sin_size = sizeof(struct sockaddr_in);

	Connect_info_t  *new_connect;
	DRVOS_Task_t *pTcpClientTask = NULL;

	//pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	//pthread_setcanceltype(PTHREAD_CANCEL_ASYCHRONOUS, NULL);
	sleep(8);
	sockfd = sock_init();
	if(sockfd < 0)
		return ;

	g_Server_info.sockfd = sockfd;
	g_Server_info.client_count = 0;
	printf("server [%s:%d] start success\r\n", "0.0.0.0", SERVER_PORT);

	while(1)
	{
		//pthread_testcancel();
		printf("Wait new connecting...\n");
		if ((new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size)) == -1)
		{
			perror("accept");
			return;		
		}
		//pthread_testcancel();
		printf("New connenct incoming..! fd=%d\n", new_sockfd);
		new_connect = (Connect_info_t*)DRV_OSMalloc(SystemPartition, sizeof(Connect_info_t));
		if(new_connect == NULL)
		{
			printf("malloc(Connect_info_t) error: no enough memory!!\n");
			return ;
		}
		memset(new_connect, 0 , sizeof(Connect_info_t));
		new_connect->recv_status = PKT_STATUS_HEAD;
		new_connect->sockfd= new_sockfd;
		new_connect->addr = client_addr;
		pTcpClientTask = DRV_OS_TaskCreate("tcpServerTask", tcpClientTask, 3, 32 * 1024, (void*)new_connect, 0);
		new_connect->task = pTcpClientTask;
		printf("Start new tcpClientTask success! [0x%x]\n", pTcpClientTask);
	}
}


int tcpServerInit()
{
	if(bServerInit == 0)
	{
		ListInit(&g_TcpClientList, 0, NULL, NULL, NULL, SystemPartition);
		g_ringbuffer = ring_buffer_init(FIFO_MAX, &g_pktLock);
		if(g_ringbuffer == NULL)
			return -1;
		bServerInit = 1;
	}
	
	g_Server_info.task = DRV_OS_TaskCreate("tcpServerTask", tcpServerTask, 3, 32 * 1024, NULL, 0);
    if (g_Server_info.task == NULL)
    {
        Err_Print("[DRV]tcpServerInit tcpServerTask is fail!\n");
        return -1;
    }

	return 0;
}

int task_exit(DRVOS_Task_t *task)
{
	int ret, cnt = 5;
	while(cnt--)
	{
		ret = DRV_OS_TaskWait(task, DRVOS_WAIT_FINITY, 500); 
		if(ret == 0)
			break;
	    DRV_OS_TaskKill(task, 0, 0);
		DRV_OS_TaskDelay_MS(100);
	}
	DRV_OSTaskDelete(task);
	return cnt;
}

int tcpServerExit()
{
	int ret;
	ret = task_exit(g_Server_info.task);
	memset(&g_Server_info, 0, sizeof(Server_info_t));
	return ret;
}

int tcpClientExit()
{
	int count = 0, cnt=5;
	Connect_info_t *connect;

	count = ListGetCount(&g_TcpClientList);
	printf("Client count=%d, start clean!!\n", count);
	connect = ListMoveHead(&g_TcpClientList);
	while(connect != NULL)
	{
		if(task_exit(connect->task) >= 0)
		{
			connect->task = NULL;
			ListDeleteItem(&g_TcpClientList, connect);
		}
		connect = ListMoveNext(&g_TcpClientList);
	}
	count = ListGetCount(&g_TcpClientList);
	printf("Client count=%d, clean done!!\n", count);
	
	return count;
}

int tcpServerReInit()
{
	tcpClientExit();
	tcpServerExit();
	tcpServerInit();
}

