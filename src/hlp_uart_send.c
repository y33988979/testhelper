/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */
 
#include <hlp_config.h>
#include <hlp_core.h>
#include <hlp_ring.h>
#include <hlp_net.h>
#include <hlp_uart.h>
#include <hlp_log.h>
#include <hlp_uart_command.h>

sem_t uart_data_sem;
extern hlp_buf_t web_data;
hlp_u32_t uart_last_send_time = 0;

hlp_u8_t cmd_heartbeat_str[] = {0x40, 0x23, 0x11, 0, 0};


void hlp_uart_send_command(CommandType Cmd)
{
	int i = 0;
	hlp_u16_t checksum = 0;
	hlp_u8_t *p = NULL;
	
	UartCommand  command;
	command.pkt_head = 0x4023; /* @# */
	command.Cmd_type = STB_HeartBeat;
	command.dataLen = 0;
	command.checksum = 0;
	p = (hlp_u8_t *)&command;
	for(i=0; i<command.dataLen; i++)
	{
		checksum = checksum ^ *p++;
	}
	command.checksum = checksum;
	sem_post(&uart_data_sem);
}


int hlp_web_data_process(hlp_buf_t *pWebData)
{
	UartCommand *pCmd = NULL;

	pCmd = (UartCommand *)pWebData->data;

	//hlp_uart_send_command(pCmd);
	return 0;
}

void* hlp_uart_send_task(void *args)
{
	int   len;
	int uart_fd = *(int *)args;
	//char rBuffer[128] = {0};
	//hlp_buf_t  pWebData = {0};
	
	while(1)
	{
		sem_wait(&uart_data_sem);

		len = DRV_SerialWrite(uart_fd, (char *)cmd_heartbeat_str, 5);
		if(len < 0)
		{
			printf("[hlp_uart_send_task]: DRV_SerialWrite error!!\n");
			LOG_WRITE_ERR("[hlp_uart_send_task]: DRV_SerialWrite error!!\n");
		}
		uart_last_send_time = hlp_time_ms();
		//pCmd = (UartCommand*)hlp_get_web_data();
		//DRV_SerialWrite(uart_fd, (char *)web_data.data, web_data.len);
	}

	return 0;
}

int hlp_uart_send_start(int fd)
{
	int        ret;
	pthread_t  tid;

	ret = sem_init(&uart_data_sem, 0, 0);
	if(ret < 0)
	{
		perror("hlp_uart_send_start sem_init: ");
		LOG_WRITE_ERR("hlp_uart_send_start sem_init: %s", strerror(errno));
		return HLP_ERROR;
	}

	ret = pthread_create(&tid, NULL, (void * (*)(void*)) hlp_uart_send_task, (void*)&fd);
	if(ret < 0)
	{
		perror("pthread_create hlp_uart_send_task error:");
		LOG_WRITE_ERR("pthread_create hlp_uart_send_task error: %s", strerror(errno));
		return HLP_ERROR;
	}
	
	printf("Start new hlp_uart_send_start success! [0x%x]\n", (int)&tid);
	LOG_WRITE_ERR("Start new hlp_uart_send_start success! [0x%x]\n", (int)&tid);
	sleep(1);
	
	return HLP_OK;
}

