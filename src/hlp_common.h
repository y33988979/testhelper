/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */
 
#ifndef __HLP_COMMON_H__
#define __HLP_COMMON_H__


#include <hlp_config.h>
#include <hlp_core.h>

hlp_s32_t hlp_get_dir_file_num(hlp_u8_t *path);
hlp_bool_t hlp_file_isexist(hlp_u8_t *path, hlp_u8_t *filename);
void hlp_get_current_time_string(hlp_u8_t *timeString);
void hlp_get_current_date_string(hlp_u8_t *dateString);
void hlp_get_current_datetime_string(hlp_u8_t *dateString);
void hlp_taskdelay_ms(hlp_u32_t Delay);


#endif /* __HLP_COMMON_H__ */

