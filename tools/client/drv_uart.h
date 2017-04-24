/**
 \file
 \brief common head file or uart
 \author ychen
 \date 2015.11.16
 */
/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */
 
#ifndef __HLP_UART_H__
#define __HLP_UART_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>


int DRV_OpenSerial(const char *dev_name);

int DRV_CloseSerial(int fd);

int DRV_ResetSerial(int fd);

int DRV_SetTimeout(int fd, unsigned char timeout_hundredMS,unsigned read_min);

int DRV_SetSpeed(int fd);

int DRV_SetSerialRowmode(int fd);

int DRV_SerialRead(int fd,char *buf,int len);

int DRV_SerialWrite(int fd,char *buf,int len);

int drv_uart_test();

extern int hlp_uart_recv_start(int fd);
extern int hlp_uart_send_start(int fd);

#endif /* __HLP_UART_H__ */

