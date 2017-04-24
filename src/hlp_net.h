/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */
 
#ifndef __HLP_NET_H__ 
#define __HLP_NET_H__

#include <hlp_config.h>
#include <hlp_core.h>

#define  HLP_TCP_BUF_SIZE    64

typedef struct _Server_info_t
{
	int   sockfd;
	struct sockaddr_in addr;
	pthread_t  task;
	/* �������Ŀͻ������� */
	int client_count;
}Server_info_t;

typedef struct _Connect_info_t
{
	int   sockfd;
	struct sockaddr_in addr;
	pthread_t  task;

	int recv_status;
	/* ��Ҫ���͵����ݵĳ���*/
	int send_len;
	/* ���յ������ݳ��� */
	int recv_total_len;
	int recv_data_len;
	int recv_data_size;
	/* �������ݻ���*/
	unsigned char send_buf[HLP_TCP_BUF_SIZE];
	/* �������ݻ��� */
	unsigned char recv_buf[HLP_TCP_BUF_SIZE];
	/* */
	unsigned char temp_buf[HLP_TCP_BUF_SIZE];
}Connect_info_t;

int hlp_net_print_init(); 

/* printf use ethernet instead serial */
void nprintf(char *Msg);
 
#endif /* __HLP_NET_H__ */
 
 