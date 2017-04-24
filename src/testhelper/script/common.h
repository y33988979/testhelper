/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */
#ifndef __COMMON_H__
#define __COMMON_H__

#include <hlp_core.h>
#include <hlp_config.h>

/* ��ȡweb�ύ��GET/POST���� */
hlp_u8_t* getcgidata(FILE* fp, hlp_u8_t* requestmethod);
/* ����web�ύ��GET/POST���� */
hlp_u8_t input_parse(hlp_u8_t *input);
/* ��ȡweb�ύ��GET/POST���ݸ��� */
hlp_u8_t get_input_argc();
/* ��ȡweb�ύ��GET/POST���ݲ����� */
hlp_u8_t** get_input_argv();
/* ��ȡ������web�ύ��GET/POST���� */
hlp_u8_t cgidata_process();

#endif /* __COMMON_H__ */

