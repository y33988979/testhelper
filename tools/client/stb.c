#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "drv_uart.h"

static int g_UartFd = -1;

void* hlp_uart_recv_task(void *args)
{
	int  len;
	char commandString[512];
	char buffer[128];
	int uart_fd = *(int *)args;

	while(1)
	{
		memset(commandString, 0, sizeof(commandString));
		memset(buffer, 0, sizeof(buffer));
		len = DRV_SerialRead(g_UartFd, commandString, sizeof(commandString));
		if(len <= 0)
		{
			printf("%s %d DRV_SerialRead fail. ret=%d\n", __func__, __LINE__, len);
			usleep(1000);
			continue;
		}
		commandString[len] = '\0';
		printf("commandString=%s\n", commandString);
		sprintf(buffer, "echo \"%s\" >> uart.log", commandString);
		system(buffer);
		if(0 == strncmp(commandString, "reboot", 6))
		{
			reboot(0);
		}
	}
}

int hlp_uart_recv_start(int fd)
{
	int        ret;
	pthread_t  tid;
	
	ret = pthread_create(&tid, NULL, (void * (*)(void*)) hlp_uart_recv_task, (void*)&fd);
	if(ret < 0)
	{
		perror("pthread_create hlp_uart_recv_start error:");
		return -1;
	}
	
	sleep(1);
	printf("Start new hlp_uart_recv_start success! [0x%x]\n", (int)&tid);
	return ret;
}

static int UartInit()
{
	int fd;
	fd = DRV_OpenSerial("/dev/ttyAMA0");
	if(fd <0)
	{
		printf("StbParameterSet open tty1 error!\n");
		return -1;
	}
	DRV_SetSpeed(fd);
	DRV_SetSerialRowmode(fd);
	DRV_SetTimeout(fd, 2, 0);
	
	return fd;
}

int main()
{	
	int ret, len = 0;
	int count = 0x30;
	char send_buffer[128] = "stb log message..[%d]";

	g_UartFd = UartInit();
	hlp_uart_recv_start(g_UartFd);
	while(1)
	{	
		memset(send_buffer, 0, sizeof(send_buffer));
		sprintf(send_buffer, "stb log message..[%d]\n", count++);
		len = strlen(send_buffer);
		ret = DRV_SerialWrite(g_UartFd, send_buffer, len);
		if(ret < 0)
		{	
			printf("%s %d DRV_SerialWrite fail. ret=%d\n", __func__, __LINE__, ret);
			break;
		}
		usleep(2000);
	}
	close(g_UartFd);
}


int main1(int argc, char **argv)
{
	int  ret;
	char commandString[512];

	while(1)
	{
		if(fgets(commandString, sizeof(commandString), stdin) == NULL)
		{
			printf("%s %d fgets fail.\n", __func__, __LINE__);
			usleep(1000);
			continue;
		}
		if(0 == strncmp(commandString, "reboot", 6))
		{
			reboot(0);
		}
	}
	
	return 0;
	
}

