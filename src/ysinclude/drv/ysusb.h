#ifndef _YSUSB_H_
#define _YSUSB_H_

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <linux/types.h>
#include <linux/netlink.h>
#include <sys/mount.h>
#include <errno.h>
#include <pthread.h>
#include <dirent.h>
#include <signal.h>
#include <stddefs.h>
#include <ysvfs.h>

typedef struct dev_info_s
{
	unsigned int state;   //�豸�Ƿ����
    char devname[16];//�豸��
	char logicname[16][32];//�߼��豸��
	char mountdir[16][32];//���ص�
	char mountedlogname[16][32];//�ѹ����ϵ��߼��豸��
	unsigned int logicdev_num;//�߼��豸����
	unsigned int mountnum;//�ѹ��ظ���

}dev_info;
typedef struct usb_notify_s
{
	unsigned int state;   //add;0 remove;
	unsigned int devnum;
	unsigned int mountnum;
	dev_info devinfo[16];
	
}usb_notify;

extern usb_notify usbdevice;
void *usbhotplug(void);


int aweigh_checkpanfu(char Panfu);
char aweigh_getpanfu();
void	vfsmount();
BOOL drv_UsbCheckString(char *key,int keylen);
void checkdevice(void);
void *usbhotplug(void);
int DRV_UsbInit(void);

void unregister_app_plugout_callback();
void register_app_plugout_callback(callback cb);
void unregister_app_plugin_callback();
void register_app_plugin_callback(callback cb);
void unregister_app_mountok_callback();
void register_app_mountok_callback(callback cb);
void unregister_app_mountfailed_callback();
void register_app_mountfailed_callback(callback cb);

#ifdef __cplusplus
}
#endif
#endif
