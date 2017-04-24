/**
 \file
 \brief common head file or uart
 \author wrwei
 \date 2012.05.22
 */

#ifndef __DRV_UART_H__
#define __DRV_UART_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>


#ifdef __cplusplus
extern "C" {
#endif

int DRV_OpenSerial(const char *dev_name);

int DRV_CloseSerial(int fd);

int DRV_ResetSerial(int fd);

/************************************************************************************
设置最长等待时间和返回方式 add by yjzhou 2013-9-17
timeoutMS: 超时时间
read_min:  读取字符长度

read_min > 0，timeoutMS > 0  读到一个字节后，启动定时器，
其超时时间为timeoutMS, read()返回的条件为至少读到read_min个字符或定时器超期

read_min > 0, timeoutMS == 0, 只要读到数据的字节数大于等于read_min，则read()返回；
否则，将无限期阻塞等待。

read_min == 0, timeoutMS > 0    只要读到数据，则read()返回；
若定时器超期却未读到数据，则read()返回0

read_min == 0, timeoutMS == 0 若有数据，则read()读取指定数量的数据后返回；
若没有数据，则read()返回0；

**************************************************************************************/
int DRV_SetTimeout(int fd, unsigned char timeoutMS, unsigned int read_min);

int DRV_SetSpeed(int fd);

int DRV_SetSerialRowmode(int fd);

int DRV_SerialRead(int fd,char *buf,int len);

int DRV_SerialWrite(int fd,char *buf,int len);

int drv_uart_test();

#ifdef __cplusplus
}
#endif
#endif /* __SAMPLE_AUDIO_PUB_H__ */

