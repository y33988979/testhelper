/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */

 
#include <hlp_gpio.h>
#include <hlp_panel.h>
#include <hlp_log.h>
#include <hlp_common.h>
#include <hlp_switch_onoff.h>
#include <hlp_communicate.h>

hlp_bool_t  g_switch_onoff_start = HLP_FALSE;
hlp_bool_t  g_first_start_flag = HLP_FALSE;
hlp_switch_onoff_task_t g_switch_onoff_task;

extern hlp_u32_t  panel_last_key_time;
extern hlp_bool_t bRecvHeartbeat;
extern hlp_bool_t bTcpConnected;
extern hlp_bool_t bSwitchtaskIsDetect;
extern hlp_bool_t bTcpClientIsExit;
extern hlp_bool_t bTcpClientExitDone;


void switch_onoff_conf_init()
{
	bTcpConnected = HLP_FALSE;
	bRecvHeartbeat = HLP_FALSE;
	bSwitchtaskIsDetect = HLP_FALSE;
	bTcpClientExitDone = HLP_FALSE;
	g_switch_onoff_task.current_time = 0;
	g_switch_onoff_task.start_time = 0;
	g_switch_onoff_task.end_time = 0;
	g_switch_onoff_task.current_count = 0;
	g_switch_onoff_task.interval_time = 10;//10
	g_switch_onoff_task.total_count = 9999;
	g_switch_onoff_task.connect_wait_count = 0;
	g_switch_onoff_task.connect_wait_max_count = 60;
	g_switch_onoff_task.heartbeat_drop_max = 3;
	g_switch_onoff_task.android_boot_time = 40;//40
	g_switch_onoff_task.is_ringing = HLP_FALSE;
}

void switch_onoff_count_show()
{
	panel_led_show_number(g_switch_onoff_task.current_count);	
	//printf("adsfasdfs!@@@@@@@@@@@#######\n");
}

void hlp_switch_reboot()
{
	hlp_power_switch_off();
	sleep(g_switch_onoff_task.interval_time);
	hlp_power_switch_on();
	hlp_commu_tcp_server_start();
	sleep(g_switch_onoff_task.android_boot_time);
}

void* hlp_switch_onoff_task(void *args)
{
	hlp_u32_t   current_time = 0;
	hlp_switch_onoff_task_t      *task = &g_switch_onoff_task;
	
	while(1)
	{
		/* ����ֹͣʱ��˯�� */
		if(g_switch_onoff_start == HLP_FALSE)
		{
			sleep(1);
			continue;
		}

		current_time = hlp_time_ms();
		
		/* ������������ʱ���״���ʱһ��ʱ�� */
		if(g_first_start_flag==HLP_TRUE && g_switch_onoff_start==HLP_TRUE)
		{
			switch_onoff_count_show();
			sleep(g_switch_onoff_task.android_boot_time);
			g_first_start_flag=HLP_FALSE;
		}

		/* ��ʼ������� */
		if(bTcpConnected)
		{
			if(bRecvHeartbeat == HLP_TRUE)
			{
				if(task->total_count == task->current_count)
				{
					printf("[testhelper]: poweronoff test done! count=[%d]!\n", task->current_count);
					LOG_WRITE_ERR("[testhelper]: poweronoff test done! count=[%d]!\n", task->current_count);
					bTcpClientIsExit = HLP_TRUE;
					hlp_switch_onoff_task_stop();
					continue;
				}
				/* �յ����������¿��ػ� */
				task->current_count++;
				task->connect_wait_count = 0;
				panel_enter_main_menu();
				switch_onoff_count_show();
				bTcpClientIsExit = HLP_TRUE;
				printf("[testhelper]: poweronoff success!! count=[%d]!\n", task->current_count);
				LOG_WRITE_ERR("[testhelper]: poweronoff success!! count=[%d]!\n", task->current_count);
				hlp_switch_reboot();
				continue;
			}
		}

		/* �涨ʱ����δ�յ��Զ����������� */
		task->connect_wait_count++;
		if(task->connect_wait_count == task->connect_wait_max_count)
		{
			printf("[testhelper]: the box is offline, task stop, current_count=%d !!!\n", task->current_count);
			LOG_WRITE_ERR("[testhelper]: the box is offline, task stop, current_count=%d !!!\n", task->current_count);
			//panel_led_show_string("Erro");
			bTcpClientIsExit = HLP_TRUE;
			hlp_switch_onoff_task_stop();
			hlp_redled_on();
			task->is_ringing = HLP_TRUE;
			task->connect_wait_count = 0;
			hlp_buzzer_ring_ms(1000*60);//���޸ĳɳ���/////
			continue;
		}
		printf("no heartbeat comming count[%d]\n", task->connect_wait_count);
		LOG_WRITE_ERR("no heartbeat comming count[%d]\n", task->connect_wait_count);

		/* 4����δ������������ʾ����ʣ��ʱ�� */
		if(current_time - panel_last_key_time >= 5*1000)
		{	
			panel_enter_main_menu();
			switch_onoff_count_show();
		}
		
		hlp_taskdelay_ms(400);
	}
}

void hlp_switch_onoff_task_start()
{
	if(g_switch_onoff_start == HLP_TRUE)
		return;
	g_switch_onoff_task.start_time = hlp_time_ms();
	g_switch_onoff_task.current_time = 0;
	g_switch_onoff_task.end_time = 0;
	switch_onoff_conf_init();
	switch_onoff_count_show();
	hlp_power_switch_off();
	hlp_buzzer_ring_ms(200);
	hlp_panel_set_key(HLP_KEY_NULL);
	hlp_redled_off();
	panel_enter_main_menu();
	sleep(2);
	hlp_power_switch_on();
	printf("-----poweron-----!!!\n");
	LOG_WRITE_ERR("hlp_switch_onoff_task_start -----poweron-----!!!\n");
	hlp_commu_tcp_server_start();
	g_first_start_flag = HLP_TRUE;
	g_switch_onoff_start = HLP_TRUE;
}

void hlp_switch_onoff_task_stop()
{
	if(g_switch_onoff_start == HLP_FALSE)
		return;
	g_switch_onoff_start = HLP_FALSE;
	bTcpConnected = HLP_FALSE;
	bRecvHeartbeat = HLP_FALSE;
	hlp_commu_tcp_server_stop();
	hlp_buzzer_ringoff();
	panel_enter_main_menu();
	g_switch_onoff_task.end_time = hlp_time_ms();
}

hlp_s32_t hlp_switch_onoff_task_init()
{
	int        ret;
	pthread_t  tid;

	g_switch_onoff_start = HLP_FALSE;

	switch_onoff_conf_init();

	ret = pthread_create(&tid, NULL, (void * (*)(void*)) hlp_switch_onoff_task, (void*)NULL);
	if(ret < 0)
	{
		perror("pthread_create hlp_switch_onoff_task error:");
		LOG_WRITE_ERR("pthread_create hlp_switch_onoff_task error");
		return HLP_ERROR;
	}
	
	sleep(1);
	printf("Start hlp_switch_onoff_task success! [0x%x]\n", (int)&tid);
	LOG_WRITE_ERR("Start hlp_switch_onoff_task success! [0x%x]\n", (int)&tid);
	return HLP_OK;
}



int hlp_switch_onoff_test()
{
	return 0;
}





