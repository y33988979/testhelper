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
������ȴ�ʱ��ͷ��ط�ʽ add by yjzhou 2013-9-17
timeoutMS: ��ʱʱ��
read_min:  ��ȡ�ַ�����

read_min > 0��timeoutMS > 0  ����һ���ֽں�������ʱ����
�䳬ʱʱ��ΪtimeoutMS, read()���ص�����Ϊ���ٶ���read_min���ַ���ʱ������

read_min > 0, timeoutMS == 0, ֻҪ�������ݵ��ֽ������ڵ���read_min����read()���أ�
���򣬽������������ȴ���

read_min == 0, timeoutMS > 0    ֻҪ�������ݣ���read()���أ�
����ʱ������ȴδ�������ݣ���read()����0

read_min == 0, timeoutMS == 0 �������ݣ���read()��ȡָ�����������ݺ󷵻أ�
��û�����ݣ���read()����0��

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

