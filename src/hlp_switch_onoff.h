/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */
 
#ifndef __HLP_SWITCH_TEST_H__
#define __HLP_SWITCH_TEST_H__


#include <hlp_config.h>
#include <hlp_core.h>


typedef struct hlp_switch_onoff_conf
{
	hlp_u16_t               total_time; /* second */
	hlp_u16_t               interval_time;
	hlp_u16_t               total_count;
	hlp_u16_t               capture_log;
	hlp_u16_t               conf_items_num;
}hlp_switch_onoff_conf_t;

typedef struct hlp_switch_onoff_task
{
	hlp_u32_t               current_time;
	hlp_u32_t               start_time;
	hlp_u32_t               end_time;
	hlp_u32_t               interval_time;
	hlp_u16_t               current_count;
	hlp_u16_t               total_count;
	hlp_u16_t               connect_wait_count;
	hlp_u16_t               connect_wait_max_count;
	hlp_u16_t               heartbeat_drop_max;
	hlp_u16_t               android_boot_time;
	hlp_bool_t              is_ringing;

}hlp_switch_onoff_task_t;

void hlp_switch_onoff_task_start();
void hlp_switch_onoff_task_stop();
hlp_s32_t hlp_switch_onoff_task_init();

#endif /* __HLP_SWITCH_TEST_H__ */

