/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */
 
#ifndef __HLP_COMMUNICATE_H__
#define __HLP_COMMUNICATE_H__


#include <hlp_config.h>
#include <hlp_core.h>


void hlp_commu_tcp_server_start();
void hlp_commu_tcp_server_stop();
void hlp_commu_tcp_client_start();
void hlp_commu_tcp_client_exit();
void  hlp_commu_client_exit();
int hlp_commu_init();

#endif /* __HLP_COMMUNICATE_H__ */

