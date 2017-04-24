/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */
#ifndef __COMMON_H__
#define __COMMON_H__

#include <hlp_core.h>
#include <hlp_config.h>

/* 获取web提交的GET/POST数据 */
hlp_u8_t* getcgidata(FILE* fp, hlp_u8_t* requestmethod);
/* 解析web提交的GET/POST数据 */
hlp_u8_t input_parse(hlp_u8_t *input);
/* 获取web提交的GET/POST数据个数 */
hlp_u8_t get_input_argc();
/* 获取web提交的GET/POST数据参数表 */
hlp_u8_t** get_input_argv();
/* 获取并解析web提交的GET/POST数据 */
hlp_u8_t cgidata_process();

#endif /* __COMMON_H__ */

