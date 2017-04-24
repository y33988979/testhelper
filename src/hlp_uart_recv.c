/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */
 
#include <hlp_uart.h>
#include <hlp_string.h>
#include <hlp_net.h>
#include <hlp_log.h>
#include <hlp_file.h>
#include <hlp_ring.h>
#include <hlp_burnin.h>
#include <hlp_common.h>
#include <hlp_uart_command.h>

#define HLP_LOG_PATH   "/root/HI3716/logs/"

static hlp_bool_t  bRun = HLP_TRUE;
hlp_bool_t  g_uart_need_parse = HLP_FALSE;
hlp_bool_t  g_uart_heartbeat_recv = HLP_TRUE;
hlp_uart_recv_staus_e   g_uart_recv_status = UART_RECV_STOP;

hlp_uart_recv_staus_e hlp_uart_recv_status_get()
{
	return g_uart_recv_status;
}

void hlp_uart_recv_status_set(hlp_uart_recv_staus_e status)
{
	g_uart_recv_status = status;
}

hlp_s32_t hlp_uart_recv_parse(hlp_u8_t *data, hlp_u32_t len)
{
	hlp_u8_t  *ptr = NULL;
	hlp_u8_t   status_str[16] = {0x40, 0x23, 0x11, 0x00, 0x00};
	hlp_u32_t  count;

	count = len;
	ptr = data;
	//while(ptr != (data-16))
	{
		if(hlp_strncmp(ptr, status_str, 5) == 0)
		{
			g_uart_heartbeat_recv = HLP_TRUE;
			printf("hlp_uart_recv_parse recv [heartbeat!]\n");
			LOG_WRITE_ERR("hlp_uart_recv_parse recv [heartbeat!]\n");
			return len;
		}
	}
	//g_uart_heartbeat_recv = HLP_TRUE; /* 测试添加，实际与被测机通信时要去掉 */
	
	return -1;
}

void* hlp_uart_recv_task(void *args)
{
	int  i, len, count;
	hlp_u8_t rBuffer[512] = {0};
	int uart_fd = *(int *)args;
	hlp_buf_t wBuffer;
	hlp_file_t wFile;
	hlp_u8_t  dateString[64] = {0};
	hlp_uart_recv_staus_e recv_status;

	i = 0; 
	len = 0;
	count = 0;
	while(bRun)
	{
		recv_status = hlp_uart_recv_status_get();
		switch(recv_status)
		{
			case UART_RECV_START:
				hlp_get_current_datetime_string(dateString);
				strcpy((char*)wFile.name, HLP_LOG_PATH);
				strcat((char*)wFile.name, (char*)dateString);
				if(hlp_get_burnin_isCapture_log() == 0)
				{
					printf("Capture_log ==========%d\n", hlp_get_burnin_isCapture_log());
					LOG_WRITE_ERR("Capture_log ==========%d\n", hlp_get_burnin_isCapture_log());
				}
				wFile.fd = hlp_file_open(wFile.name, O_CREAT|O_WRONLY, 664);
				if(wFile.fd < 0)
				{
					perror("hlp_open_file:");
					LOG_WRITE_ERR("hlp_open_file: %s", strerror(errno));
					hlp_uart_recv_status_set(UART_RECV_ERROR);
				}
				hlp_uart_recv_status_set(UART_RECV_RUNNING);
				printf("[testhelper]: [uart_recv]: create file \"%s\" fd=%d, success.!\n", wFile.name, wFile.fd);
				LOG_WRITE_ERR("[testhelper]: [uart_recv]: create file \"%s\" fd=%d, success.!\n", wFile.name, wFile.fd);
				continue;
				break;
			case UART_RECV_RUNNING:
				len = DRV_SerialRead(uart_fd, (char *)rBuffer, sizeof(rBuffer)-1);
				if(len > 0)
				{
					if(g_uart_need_parse)
						hlp_uart_recv_parse(rBuffer, len);
					memcpy(wBuffer.data, rBuffer, len);
					wBuffer.len = len;
					//PutPkt(&wBuffer);
					rBuffer[len] = '\0';
					hlp_file_write(&wFile, wBuffer.data, wBuffer.len);
					//nprintf(rBuffer);
					//printf("len=====0x%x\n", len);
				}
				else
				{
					if(len < 0)
					{
						perror("DRV_SerialRead error:");
					}
					hlp_taskdelay_ms(2);
				}
				continue;
				break;
			case UART_RECV_SUSPEND:
				
				printf("[testhelper]: [uart_recv]: suspend!!\n");
				LOG_WRITE_ERR("[testhelper]: [uart_recv]: suspend!!\n");
				hlp_uart_recv_status_set(UART_RECV_SLEEP);
				break;
			case UART_RECV_STOP:
				//printf("uart_recv stop!!\n");
				printf("[testhelper]: [uart_recv]: close file fd=%d. !\n", wFile.fd);
				LOG_WRITE_ERR("[testhelper]: [uart_recv]: close file fd=%d. !\n", wFile.fd);
				hlp_file_close(wFile.fd);
				hlp_uart_recv_status_set(UART_RECV_SLEEP);
				break;
			case UART_RECV_SLEEP:
				hlp_taskdelay_ms(100);
				break;
			case UART_RECV_ERROR:
				printf("uart_recv error!!\n");
				LOG_WRITE_ERR("uart_recv error!!\n");
				hlp_uart_recv_status_set(UART_RECV_SLEEP);
				break;
		}
	}
	
	return 0;
}

int hlp_uart_recv_start(int fd)
{
	int        ret;
	pthread_t  tid;

	hlp_uart_recv_status_set(UART_RECV_SLEEP);
	ret = pthread_create(&tid, NULL, (void * (*)(void*)) hlp_uart_recv_task, (void*)&fd);
	if(ret < 0)
	{
		perror("pthread_create hlp_uart_recv_start error:");
		LOG_WRITE_ERR("pthread_create hlp_uart_recv_start error: %s", strerror(errno));
		return -1;
	}
	printf("Start hlp_uart_recv_start success! [0x%x]\n", (int)&tid);
	LOG_WRITE_ERR("Start hlp_uart_recv_start success! [0x%x]\n", (int)&tid);
	sleep(1);
	
	return ret;
}


