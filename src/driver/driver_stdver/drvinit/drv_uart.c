#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/types.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <signal.h>
#include "drv_uart.h"

int DRV_OpenSerial(const char *dev_name)
{
	int fd;
	fd = open(dev_name, O_RDWR);
	if (-1 == fd){ 
		perror("");
		exit(1);
	}

	return fd;
}

int DRV_CloseSerial(int fd)
{
    int ret;
    
    ret = close(fd);
   
    return ret;
}

int DRV_SetSpeed(int fd)
{
	int ret = 0;
	struct  termios opt;

	tcgetattr(fd, &opt);
	tcflush(fd, TCIOFLUSH);
	cfsetispeed(&opt,B115200);
	cfsetospeed(&opt,B115200);

	ret = tcsetattr(fd,TCSANOW,&opt);
	if(ret <0) {
		perror("");
	}
	tcflush(fd,TCIOFLUSH);

	return ret;
}

int DRV_EraseIn(int fd)
{
	int num;
	num=tcflush(fd, TCIFLUSH);
	if (-1 == num){ 
		printf("Erase  input memory error!");
	}

}

int DRV_EraseOut(int fd)
{
	int num;
	num=tcflush(fd, TCOFLUSH);
	if (-1 == num){ 
		printf("Erase  output memory error!");
	}

}

int DRV_EraseIO(int fd)
{
	int num;
	num=tcflush(fd, TCIOFLUSH);
	if (-1 == num){ 
		printf("Erase  memory error!");
	}
}

int DRV_SetSerialRowmode(int fd)
{
	int ret = 0;
	struct termios options; 

	tcgetattr(fd, &options);

	tcflush(fd, TCIOFLUSH);
	// termios �ṹ�п���ģʽ��־�ֶ�c_cflag 
	options.c_cflag |= CREAD ;// ������ա�
	options.c_cflag &=~CSIZE;//�������λ����
	options.c_cflag |= CS8 ; //����8λ����λ��־CS8
	options.c_cflag &= ~PARENB;//ͣ����żУ��	
	options.c_cflag &= ~CSTOPB;//ʹ��1λֹͣλ
	options.c_cflag &= ~ CRTSCTS ;//��Ӳ��������λ
	

// termios �ṹ�б���ģʽ��־�ֶ�c_lflag 
	options.c_lflag |= ECHO;	// ���������ַ���
	options.c_lflag &= ~ICANON ;// �����淶ģʽ����ģʽ����
	options.c_lflag |= ECHOE;// ��������ICANON����ERASE/WERASE ������ǰһ�ַ�/���ʡ�
	options.c_lflag |= ISIG ;// ���յ��ַ�INTR��QUIT��SUSP ��DSUSP��������Ӧ���źš�

// termios �ṹ�����ģʽ�ֶ�c_oflag 
	options.c_oflag |= OPOST;// ִ���������
	options.c_oflag |= ONLCR; // �����ʱ�����з�NL ӳ��ɻس�-���з�CR-NL��
	options.c_oflag |=OCRNL; // �����ʱ���س���CR ӳ��ɻ��з�NL��

// termios �ṹ����ģʽ�ֶ�c_iflag 
	options.c_iflag &= ~ICRNL;// ������ʱ���س���CR ӳ��ɻ��з�NL��
	options.c_iflag &= ~INLCR;// ����ʱ�����з�NL ӳ��ɻس���CR��
	options.c_iflag &= ~IGNCR;// ���Իس���CR��
	options.c_iflag &= ~IXON ;// ����ʼ/ֹͣ��XON/XOFF��������ơ�
	options.c_iflag &=  ~IXOFF ;// ����ʼ/ֹͣ��XON/XOFF��������ơ�


	tcflush(fd,TCIFLUSH); 
	options.c_cc[VTIME] = 50;
	options.c_cc[VMIN] = 1;

	ret = tcsetattr(fd,TCSANOW,&options);
	if (ret < 0) { 
		perror(""); 
	} 

	return ret;
}

int DRV_SerialRead(int fd,char *buf,int len)
{
    int num;
    
    num = read(fd, buf,len);
    if(-1 == num)
		{
        printf("DRV_SerialRead error!\n");
    }

    return num;

}

int DRV_SerialWrite(int fd,char *buf,int len)
{
    int num;
    
    num = write(fd, buf,len);
    if(len != num)
	{
        printf("DRV_SerialWrite error!\n");
    }
    
    return num;
}
    
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
int DRV_SetTimeout(int fd, unsigned char timeoutMS, unsigned int read_min)
    {
	int ret = 0;
	struct  termios options;

	tcgetattr(fd, &options);
	
	tcflush(fd,TCIFLUSH); 
	options.c_cc[VTIME] = timeoutMS;
	options.c_cc[VMIN] = read_min;

	ret = tcsetattr(fd,TCSANOW,&options);
	if(ret <0) {
		perror("");
    }

	tcflush(fd,TCIOFLUSH);

	return ret;
}

#define MAX_BUFF_SIZE (20)
int drv_uart_test()
{

	int fd = -1;
	int wr_pos = 0;
	int i;

	fd = DRV_OpenSerial("/dev/ttyAMA0");
	if(fd <0 )
		printf("open tty1 error!\n");
		
	DRV_SetSpeed(fd);
	DRV_SetSerialRowmode(fd);
	DRV_SetTimeout(fd,30 ,20);

    char buf[MAX_BUFF_SIZE];
	while(1)
    {
	    	DRV_SerialRead(fd, buf,20); 
	    	DRV_SerialWrite(fd,buf,20);
		tcflush(fd, TCOFLUSH);	
		
		printf("&&&&&&&&&&&&&&&&\n");
		printf("+++++++++++++++");
		
		fflush(stdout);		
		tcflush(fd, TCOFLUSH);
    }
	
	
	DRV_CloseSerial(fd);
	return 0;
}

