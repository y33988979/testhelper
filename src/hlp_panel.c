/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */
 
#include <hlp_panel.h>
#include <hlp_string.h>
#include <hlp_gpio.h>
#include <hlp_uart.h>
#include <hlp_burnin.h>
#include <hlp_switch_onoff.h>
#include <hlp_file_manager.h>
#include <hlp_switch_onoff.h>
#include <hlp_drv.h>
#include <hlp_log.h>
//#include "yspanel.h"
//#include "ysusb.h"


static char *hlp_panel_main_menu_str[4] = { "func", "stat", " set", "NULL"};
static char *hlp_panel_menu_item_str[3][4] = 
	{ 
		{"COPY", " CLS", "NULL", "NULL"},
		{"STAT", " TM1", " TM2", " TM3"},
		{"NULL", "NULL", "NULL", "NULL"}
	};
//static hlp_u8_t  hlp_panel_item_num[] = {2, 4, 0};
static char *key_string[7] = {"DOWN", "LEFT", "UP", "EXIT", "RIGHT", "", "OK"};
static hlp_u8_t  hlp_panel_key = HLP_KEY_NULL;
static hlp_u8_t  panel_last_key[4] = {HLP_KEY_NULL};
static hlp_u8_t  panel_last_index=0;
hlp_u32_t  panel_last_key_time = 0;
hlp_panel_menu_t      *g_main_menu = NULL;
extern hlp_bool_t  burnin_start;
extern hlp_switch_onoff_task_t g_switch_onoff_task;
extern hlp_bool_t  g_switch_onoff_start;

void panel_led_show_string(char *string) 
{
	DRV_LED_UnDisplayTime();
	DRV_LED_DisplayChar(string[0], string[1], string[2], string[3]);
}

void panel_led_show_number(hlp_u16_t number) 
{
	hlp_u8_t  string[16] = {0};

	if(number > 9999)
		number = 9999;
	sprintf(string ,"%04d", number);
	DRV_LED_UnDisplayTime();
	DRV_LED_DisplayChar(string[0], string[1], string[2], string[3]);
}

static void panel_led_update()
{
	hlp_u8_t  status = -1;
	status = hlp_get_led_status();
	hlp_greenled_off();
	hlp_redled_off();
	printf("panel_led_update !!\n");
	LOG_WRITE_ERR("panel_led_update !!\n");
}

static void panel_menu_update(hlp_panel_menu_t *menu)
{
	if(menu->current_item->show_type != ITEM_SHOW_NUMBER)
	{
		printf("menu->current_item->parent->name=%s.\n", menu->current_item->parent->name);
		LOG_WRITE_ERR("menu->current_item->parent->name=%s.\n", menu->current_item->parent->name);
		DRV_LED_UnDisplayTime();
		panel_led_show_string((char*)menu->current_item->name);
	}
	if(menu->current_item->enter_callback)
		menu->current_item->enter_callback(menu, 0);
}

static void panel_show_menu_str()
{
	panel_led_show_string("----");
}

void panel_enter_main_menu()
{
	hlp_panel_menu_t *menu = g_main_menu;
	menu->current_item = menu->main_item;
	menu->current_index = 0;
	menu->current_lever = 0;
}

void panel_show_main_menu()
{
	hlp_panel_menu_t *menu = g_main_menu;
	menu->current_item = menu->main_item;
	menu->current_index = 0;
	menu->current_lever = 0;
	panel_led_show_string("----");
}
void panel_menu_key_handle(hlp_panel_menu_t *menu, hlp_u8_t key)
{
	hlp_panel_menu_item_t *item = menu->current_item;
	printf("item->name=%s OK callback..\n", item->name);
	LOG_WRITE_ERR("item->name=%s OK callback..\n", item->name);
}

void panel_menu_mount_check_handle()
{
	hlp_bool_t mount_ok;

	mount_ok = hlp_mount_ok();
	if(mount_ok)
	{
		hlp_redled_off();
		hlp_greenled_on();
	}
	else
	{
		hlp_greenled_off();
		hlp_redled_on();
	}
	printf("mount_check mount is %s\n", mount_ok?"OK":"Failed");
	LOG_WRITE_ERR("mount_check mount is %s\n", mount_ok?"OK":"Failed");
}

void panel_menu_copy_handle(hlp_panel_menu_t *menu, hlp_u8_t key)
{
	hlp_s8_t  ret = 0;
	hlp_panel_menu_item_t *item = menu->current_item;
	
	printf("item->name=%s callback..\n", item->name);
	LOG_WRITE_ERR("item->name=%s callback..\n", item->name);
	if(hlp_mount_ok())
	{
		ret = system("cp /root/HI3716/logs -rf /media/volume01/");
		if(drv_SystemIsOK(ret)== 1)	hlp_operation_success();
		else			hlp_operation_failed();
	}
	else
	{
		hlp_operation_failed();
		printf("usb device is umount!!\n");
		LOG_WRITE_ERR("usb device is umount!!\n");
	}
}

void panel_menu_cls_handle(hlp_panel_menu_t *menu, hlp_u8_t key)
{
	hlp_s8_t  ret = 0;
	hlp_panel_menu_item_t *item = menu->current_item;

	printf("item->name=%s callback..\n", item->name);
	LOG_WRITE_ERR("item->name=%s callback..\n", item->name);

	ret = system("rm /root/HI3716/logs/* -rf ");
	if(drv_SystemIsOK(ret)== 1)	hlp_operation_success();
	else			hlp_operation_failed();
}

void panel_menu_burn_handle(hlp_panel_menu_t *menu, hlp_u8_t key)
{
	if(g_switch_onoff_start == HLP_FALSE)
	{
		hlp_switch_onoff_task_start();
	}
	else
	{
		printf("the switch_onoff task is running!\n");
		LOG_WRITE_ERR("the switch_onoff task is running!\n");
	}
}

void panel_menu_stop_handle(hlp_panel_menu_t *menu, hlp_u8_t key)
{
	hlp_switch_onoff_task_stop();
	hlp_power_switch_off();
}

void panel_menu_burn_handle1(hlp_panel_menu_t *menu, hlp_u8_t key)
{
	hlp_u16_t hour = 0;
	hlp_u16_t min = 0;
	hlp_panel_menu_item_t *item = menu->current_item;

	if(burnin_start == HLP_TRUE)
		return;
	printf("item->name=%s callback..\n", item->name);
	LOG_WRITE_ERR("item->name=%s callback..\n", item->name);
	hlp_burnin_print_config();
	hlp_power_switch_on();
	hlp_greenled_off();
	hlp_buzzer_ring_ms(200);
	hlp_burnin_task_start();
	panel_show_main_menu();
	min = hlp_get_burnin_remainder_time_min();
	hour = min / 60;
	min = min % 60;
	DRV_LED_DisplayTime(hour, min);
}

void panel_menu_stop_handle1(hlp_panel_menu_t *menu, hlp_u8_t key)
{
	if(burnin_start == HLP_FALSE)
		return;
	hlp_burnin_task_stop();
}

void panel_menu_load_handle(hlp_panel_menu_t *menu, hlp_u8_t key)
{
	hlp_s8_t  ret = 0;
	hlp_panel_menu_item_t *item = menu->current_item;

	printf("item->name=%s callback..\n", item->name);
	LOG_WRITE_ERR("item->name=%s callback..\n", item->name);
	if(hlp_mount_ok())
	{
		ret = system("cp /media/volume01/burnin.conf -rf /root/HI3716/conf");
		hlp_burnin_load_config();
		if(drv_SystemIsOK(ret)== 1)	hlp_operation_success();
		else			hlp_operation_failed();
	}
	else
	{
		hlp_operation_failed();
		printf("usb device is umount!!\n");
		LOG_WRITE_ERR("usb device is umount!!\n");
	}
}

void panel_menu_stat_handle(hlp_panel_menu_t *menu, hlp_u8_t key)
{
	hlp_panel_menu_item_t *item = menu->current_item;

	printf("item->name=%s callback..\n", item->name);
	LOG_WRITE_ERR("item->name=%s callback..\n", item->name);
	extern hlp_bool_t g_uart_heartbeat_recv;
	printf("g_uart_heartbeat_recv=%d\n", g_uart_heartbeat_recv);
	LOG_WRITE_ERR("g_uart_heartbeat_recv=%d\n", g_uart_heartbeat_recv);
	hlp_strcpy(item->chead->name, g_uart_heartbeat_recv?"Succ":"fail");
}

hlp_s32_t panel_menu_time1_handle()
{
	hlp_s32_t ret = 0;
	hlp_u16_t hour = 0;
	hlp_u16_t min = 0;

	min = hlp_get_burnin_total_time();
	DRV_LED_DisplayTime(hour, min);
	return ret;
}
hlp_s32_t panel_menu_time2_handle()
{
	hlp_s32_t ret = 0;
	hlp_u16_t hour = 0;
	hlp_u16_t min = 0;
	
	min = hlp_get_burnin_current_count();
	DRV_LED_DisplayTime(hour, min);
	return ret;
}
hlp_s32_t panel_menu_time3_handle()
{
	hlp_s32_t ret = 0;
	hlp_u16_t hour = 0;
	hlp_u16_t min = 0;
	
	min = hlp_get_burnin_total_count();
	DRV_LED_DisplayTime(hour, min);
	return ret;
}

hlp_s16_t hlp_panel_menu_item_reset(hlp_panel_menu_item_t *item)
{	
	hlp_strcpy(item->name, "NULL");
	item->lever = 0;
	item->index = 0;
	item->children_num = 0;
	item->type = 0;
	item->parent = NULL;
	item->prev = NULL;
	item->next = NULL;
	item->chead = NULL;
	item->ctail = NULL;
	item->key_callback = NULL;
	item->enter_callback = NULL;
	return HLP_OK;
}

hlp_s16_t hlp_panel_menu_category_init(hlp_panel_menu_category_t *category, hlp_u8_t idx, hlp_u8_t item_cnt)
{
	hlp_u8_t  i;
	if(category == NULL)
	{
		printf("[ERROR]: category == NULL\n");
		LOG_WRITE_ERR("[ERROR]: category == NULL\n");
		return HLP_ERROR;
	}
	category->index = idx;
	category->item_cnt = item_cnt;
	idx = (idx==0) ? (item_cnt-1) : (idx-1);
	category->prev = &category[idx];
	idx = (idx==item_cnt-1) ? (0) : (idx+1);
	category->next = &category[idx];
	strcpy((char*)category->name, (char*)hlp_panel_main_menu_str[idx]);

	for(i=0; i<item_cnt; i++)
	{
		hlp_panel_menu_item_reset(&category->items[i]);
		hlp_strcpy(category->items[i].name, hlp_panel_menu_item_str[idx][i]);
		//category->items[i].parent = category;
	}
	return HLP_OK;
}

hlp_s16_t hlp_panel_menu_init(hlp_panel_menu_t *menu)
{
#if 0
	hlp_u8_t  i;
	// 1.1
	hlp_panel_menu_category_init(&menu->categorys[0], 0, 2);
	// 1.2
	hlp_panel_menu_category_init(&menu->categorys[1], 1, 4);
	// 1.3
	hlp_panel_menu_category_init(&menu->categorys[2], 2, 0);
	for(i=0 ; i<HLP_MENU_CATEGORY_NUM; i++)
		menu->categorys[i].parent = menu;
#endif
	return HLP_OK;
}

hlp_panel_menu_item_t* hlp_panel_item_creat(char *name)
{
	hlp_panel_menu_item_t      *item = NULL;
	item = (hlp_panel_menu_item_t*)malloc(sizeof(hlp_panel_menu_item_t));
	if(item == NULL)
	{
		printf("hlp_panel_item_creat malloc failed, item_name=%s!\n", name);
		LOG_WRITE_ERR("hlp_panel_item_creat malloc failed, item_name=%s!\n", name);
		return NULL;
	}
	hlp_memset(item, 0, sizeof(sizeof(hlp_panel_menu_item_t)));
	hlp_memcpy(item->name, name, HLP_MENU_ITEM_NAME_LEN_MAX);
	
	return item;
}

hlp_panel_menu_item_t* hlp_panel_items_creat(hlp_u8_t num)
{
	hlp_panel_menu_item_t      *items = NULL;
	items = (hlp_panel_menu_item_t*)malloc(num*sizeof(hlp_panel_menu_item_t));
	if(items == NULL)
	{
		printf("hlp_panel_menu_item_t malloc failed, item_num=%d!\n", num);
		LOG_WRITE_ERR("hlp_panel_menu_item_t malloc failed, item_num=%d!\n", num);
		return NULL;
	}
	memset(items, 0, sizeof(num*sizeof(hlp_panel_menu_item_t)));
	
	return items;
}

void hlp_panel_menu_add_item(hlp_panel_menu_item_t *item, hlp_panel_menu_item_t *parent)
{
	if(item != NULL)
	{
		/* sort items */
		if(parent != NULL)
		{
			if(parent->children_num == 0)
			{
				parent->chead = item;
				parent->ctail = item;
				item->prev = item;
				item->next = item;
			}
			else
			{
				item->prev = parent->ctail;
				item->next = parent->chead;
				parent->chead->prev = item;
				parent->ctail->next = item;
				parent->ctail = item;
			}
			item->parent = parent;
			item->lever = parent->lever+1;
			item->index = parent->children_num;
			parent->children_num++;
		}
	}
}

hlp_panel_menu_t* hlp_panel_menu_creat()
{
	hlp_panel_menu_t           *menu = NULL;
	hlp_panel_menu_item_t      *main_item = NULL;
	hlp_panel_menu_item_t      *func_item = NULL;
	hlp_panel_menu_item_t      *stat_item = NULL;
	hlp_panel_menu_item_t      *burn_item = NULL;
	hlp_panel_menu_item_t      *item[5] = {NULL};

	menu = (hlp_panel_menu_t*)malloc(sizeof(hlp_panel_menu_t));
	if(menu == NULL)
	{
		printf("hlp_panel_menu_t malloc failed, no enough memory!\n");
		LOG_WRITE_ERR("hlp_panel_menu_t malloc failed, no enough memory!\n");
		return NULL;
	}
	
	/* main menu */
	g_main_menu = menu;
	main_item = hlp_panel_item_creat("----");
	main_item->type = HLP_PANEL_MENU_MAIN;
	menu->main_item = main_item;
	menu->current_item = main_item;

	/* Lever2 menu */
	item[0] = hlp_panel_item_creat("tttt");
	//item[1] = hlp_panel_item_creat("Func");
	//item[2] = hlp_panel_item_creat("SSSS");
	//item[3] = hlp_panel_item_creat("NULL");
	
	hlp_panel_menu_add_item(item[0], main_item);
	//hlp_panel_menu_add_item(item[1], main_item);
	//hlp_panel_menu_add_item(item[2], main_item);
	//hlp_panel_menu_add_item(item[3], main_item);
	burn_item = item[0];
	//func_item = item[1];
	//stat_item = item[2];

	/* Lever3 menu [burn]*/
	item[0] = hlp_panel_item_creat("burn");
	item[1] = hlp_panel_item_creat("StoP");
	//item[2] = hlp_panel_item_creat("load");
	hlp_panel_menu_add_item(item[0], burn_item);
	hlp_panel_menu_add_item(item[1], burn_item);
	//hlp_panel_menu_add_item(item[2], burn_item);
	item[0]->key_callback = (hlp_panel_menu_key_callback)panel_menu_burn_handle;
	item[1]->key_callback = (hlp_panel_menu_key_callback)panel_menu_stop_handle;
	//item[2]->key_callback = (hlp_panel_menu_key_callback)panel_menu_load_handle;
	#if 0
	/* Lever3 menu [Func]*/
	item[0] = hlp_panel_item_creat("copy");
	item[1] = hlp_panel_item_creat(" clS");
	item[2] = hlp_panel_item_creat("NULL");

	item[0]->key_callback = (hlp_panel_menu_key_callback)panel_menu_copy_handle;
	item[1]->key_callback = (hlp_panel_menu_key_callback)panel_menu_cls_handle;
	item[0]->enter_callback = (hlp_panel_menu_enter_callback)panel_menu_mount_check_handle;
	item[1]->enter_callback = (hlp_panel_menu_enter_callback)panel_menu_mount_check_handle;
	hlp_panel_menu_add_item(item[0], func_item);
	hlp_panel_menu_add_item(item[1], func_item);
	hlp_panel_menu_add_item(item[2], func_item);
	

	/* Lever3 menu [Stat]*/
	item[0] = hlp_panel_item_creat("  SS");
	item[1] = hlp_panel_item_creat("  t1");
	item[2] = hlp_panel_item_creat("  t2");
	item[3] = hlp_panel_item_creat("  t3");

	item[0]->key_callback = (hlp_panel_menu_key_callback)panel_menu_stat_handle;
	item[1]->key_callback = (hlp_panel_menu_key_callback)panel_menu_time1_handle;
	item[2]->key_callback = (hlp_panel_menu_key_callback)panel_menu_time2_handle;
	item[3]->key_callback = (hlp_panel_menu_key_callback)panel_menu_time3_handle;
	hlp_panel_menu_add_item(item[0], stat_item);
	hlp_panel_menu_add_item(item[1], stat_item);
	hlp_panel_menu_add_item(item[2], stat_item);
	hlp_panel_menu_add_item(item[3], stat_item);
	
	/* Lever4 menu [stat info , t1, t2, t3] */
	item[4] = hlp_panel_item_creat("erro");
	//item[4]->show_type = ITEM_SHOW_NUMBER;
	hlp_panel_menu_add_item(item[4], item[0]);
	item[4] = hlp_panel_item_creat("0000");
	item[4]->show_type = ITEM_SHOW_NUMBER;
	hlp_panel_menu_add_item(item[4], item[1]);
	item[4] = hlp_panel_item_creat("0000");
	item[4]->show_type = ITEM_SHOW_NUMBER;
	hlp_panel_menu_add_item(item[4], item[2]);
	item[4] = hlp_panel_item_creat("0000");
	item[4]->show_type = ITEM_SHOW_NUMBER;
	hlp_panel_menu_add_item(item[4], item[3]);
	#endif
	return menu;
}

static void hlp_exit_test()
{
	hlp_u8_t index = panel_last_index;
	
	if(panel_last_key[index++%4] == HLP_PANEL_KEY_LEFT
		&&panel_last_key[index++%4] == HLP_PANEL_KEY_DOWN
		&&panel_last_key[index++%4] == HLP_PANEL_KEY_RIGHT
		&&panel_last_key[index++%4] == HLP_PANEL_KEY_UP)
		exit(-1);
}
void hlp_panel_set_key(hlp_u8_t value)
{
	hlp_panel_key = value;
}

static void hlp_panel_callback(hlp_u8_t Key)
{

	if(hlp_panel_key != HLP_KEY_NULL)
	{
		printf("the Key[%d] is processing, miss key=%d\n", hlp_panel_key, Key);
		LOG_WRITE_ERR("the Key[%d] is processing, miss key=%d\n", hlp_panel_key, Key);
	}
	else
	{
		if(Key>=81 && Key<=87)
		{
			printf("Key %s is pressed!\n", key_string[Key-81]);
			LOG_WRITE_ERR("Key %s is pressed!\n", key_string[Key-81]);
		}
		if(g_switch_onoff_task.is_ringing)
		{
			g_switch_onoff_task.is_ringing = HLP_FALSE;
			hlp_buzzer_ringoff();
		}
		hlp_panel_key = Key;
		panel_last_key[panel_last_index++ % 4] = Key;
		hlp_exit_test();
		panel_last_key_time = hlp_time_ms();
	}	
}

hlp_u8_t hlp_panel_key_get(hlp_u8_t *key)
{
	*key = hlp_panel_key;
	return hlp_panel_key;
}

hlp_u8_t hlp_panel_key_process(hlp_panel_menu_t *menu, hlp_u8_t key)
{
	
	hlp_panel_menu_item_t *current_item = menu->current_item;
	//hlp_u8_t    idx = current_item->index;
	hlp_u8_t    children_num = 0;

	if(current_item->parent == NULL && key!=HLP_PANEL_KEY_SELECT)
	{
		printf("This is main menu!\n");
		LOG_WRITE_ERR("This is main menu!\n");
		hlp_panel_key = HLP_KEY_NULL;
		return HLP_OK;
	}
	panel_led_update();
	children_num = current_item->parent->children_num;
	
	printf("000menu->prev_item->name=%s\n", menu->current_item->name);
	LOG_WRITE_ERR("000menu->prev_item->name=%s\n", menu->current_item->name);

	switch(key)
	{
		case HLP_PANEL_KEY_UP:
			break;
		case HLP_PANEL_KEY_DOWN:
			break;
		case HLP_PANEL_KEY_LEFT:
			menu->current_item = current_item->prev;
			menu->current_index = menu->current_item->index;
			break;
		case HLP_PANEL_KEY_RIGHT:
			menu->current_item = current_item->next;
			menu->current_index = menu->current_item->index;
			break;
		case HLP_PANEL_KEY_SELECT:
			if(menu->current_item->key_callback)
				menu->current_item->key_callback(menu, key);
			if(current_item->children_num != 0)
			{
				menu->current_item = current_item->chead;
				menu->current_index = menu->current_item->index;
				menu->current_lever++;
			}
			break;
		case HLP_PANEL_KEY_EXIT:

			menu->current_item = current_item->parent;
			menu->current_index = menu->current_item->index;
			menu->current_lever--;

			break;
		default:
			printf("unkown key = %d\n", key);
			LOG_WRITE_ERR("unkown key = %d\n", key);
			break;
	}
	printf("111menu->current_item->name=%s\n", menu->current_item->name);
	LOG_WRITE_ERR("111menu->current_item->name=%s\n", menu->current_item->name);

	panel_menu_update(menu);
	
	hlp_panel_key = HLP_KEY_NULL;
	return HLP_OK;
}

void* hlp_panel_task(void *args)
{
	hlp_u8_t   panel_key = HLP_KEY_NULL;
	hlp_panel_menu_t *menu = NULL;

	menu = (hlp_panel_menu_t *)args;
	while(1)
	{
		hlp_panel_key_get(&panel_key);
		if(panel_key != HLP_KEY_NULL)
		{
			hlp_panel_key_process(menu, panel_key);
		}
		hlp_taskdelay_ms(300);
		//printf("hlp_panel_task sleep..\n"); 
	}
}

hlp_s16_t hlp_panel_init()
{
	hlp_s16_t  ret = 0;

	DRV_PanelInit(0);
	printf("DRV_PanelInit success!\n");
	LOG_WRITE_ERR("DRV_PanelInit success!\n");

	DRV_PanelEnableCallback((int)hlp_panel_callback);
	//DRV_PanelEnableCallback(hlp_panel_key_process);
	printf("DRV_PanelEnableCallback success!\n");
	LOG_WRITE_ERR("DRV_PanelEnableCallback success!\n");

	panel_show_menu_str();
	return ret;
}

int hlp_panel_task_start()
{
	int        ret;
	pthread_t  tid;
	hlp_panel_menu_t *menu = NULL;
	
	ret = hlp_panel_init();
	if(ret < 0)
		return -1;

	menu = hlp_panel_menu_creat();
	if(menu == NULL)
		return -1;
	printf("menu creat success! handle=0x%08x\n", (int)menu);
	LOG_WRITE_ERR("menu creat success! handle=0x%08x\n", (int)menu);
	
	ret = pthread_create(&tid, NULL, (void * (*)(void*)) hlp_panel_task, (void*)menu);
	if(ret < 0)
	{
		perror("pthread_create hlp_uart_recv_start error:");
		LOG_WRITE_ERR("pthread_create hlp_uart_recv_start error: %s", strerror(errno));
		return -1;
	}
	printf("Start new hlp_panel_test_start success! [0x%x]\n", (int)&tid);
	LOG_WRITE_ERR("Start new hlp_panel_test_start success! [0x%x]\n", (int)&tid);
	sleep(2);
	return ret;
}

