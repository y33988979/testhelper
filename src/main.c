/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */

#include <hlp_config.h>
#include <hlp_core.h>
#include <hlp_uart.h>
#include <hlp_net.h>
#include <hlp_gpio.h>
#include <hlp_panel.h>
#include <hlp_burnin.h>
#include <hlp_log.h>
#include <hlp_drv.h>
#include <hlp_file_manager.h>
#include <hlp_switch_onoff.h>
#include <hlp_communicate.h>


#define DRV_CHECK(Fun)  { hlp_s32_t err = Fun; \
                          if(err != HLP_OK) \
                          { \
							  printf("ERROR\n"); \
                              return err; \
                          } }

static int g_UartFd = -1;

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

void PrintBuildTime(void)
{
	printf("Buile Time: %s %s\n", __DATE__, __TIME__);
}

int main()
{
	PrintBuildTime();
	/* log日志 */
	hlp_log_init();
	hlp_time_ms();
	DRV_CHECK( DRV_InitBasic() );
	DRV_CHECK( DRV_UsbInit() );
	usb_callback_register();
	hlp_gpio_init();
	
#if 0 /* 串口收发任务，网口打印任务，web&cgi任务*/
	g_UartFd = UartInit();
	printf("uart init success!\n");
	//hlp_net_print_init();
	//printf("tcpServerInit(net print) success!\n");
	
	hlp_uart_recv_start(g_UartFd);
	hlp_uart_send_start(g_UartFd);
	//hlp_ipc_task_start();
#endif

	/* 网口通信任务 */
	hlp_commu_init();

	/* 前面板任务 */
	hlp_panel_task_start();

	/* 自动开关机测试任务 */
	hlp_switch_onoff_task_init();

	/* 拷机任务 */
	//hlp_burnin_init();
	//hlp_gpio_test();

	//dd();
	while(1)
	{
		sleep(30);
		printf("[%d][testhelper] program is running...\n", hlp_time_ms());
		//break;
	}
	
	return 0;
}

