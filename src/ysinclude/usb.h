/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : usb.h

    Description :

    Others:

    History:   1. xyan creat 2011-9-15


*******************************************************************************/
#ifndef	_YSUSB_
#define	_YSUSB_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "driver.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#define USB_DISK_NAME_MAX_LENGTH			60
#define USB_PART_NAME_MAX_LENGTH			60
#define USB_MAX_MOUNT_NUMBER				16
#define USB_MAX_PART_MOUNT_NUMBER			16
#define USBAPP_MAX_PATH_LENGTH   			512
#define USBAPP_MAX_DISK_PATH_LENGTH			40

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
typedef enum
{
    USBAPP_BMP_TYPE=0,	//BMPͼƬ
    USBAPP_JPG_TYPE=1,	//JPEGͼƬ
    USBAPP_GIF_TYPE,	//GIFͼƬ
    USBAPP_TEXT_TYPE=5,	//TXT�ı�
    USBAPP_VIDEO_TYPE=6,//video�ļ�
    USBAPP_TS_TYPE=8,	//TS�ļ�
    USBAPP_MP3_TYPE=12,	//MP3�ļ�
    USBAPP_PCM_TYPE=13,	//PCM�ļ�
    USBAPP_MPG_TYPE=14,	//MPG�ļ�
    USBAPP_VIDEO_CONFIG_TYPE =15,
    USBAPP_OTHER_TYPE,
    USBAPP_TYPE_COUNT
} Filetype_t;

typedef enum
{
    USBAPP_FILE_MODE    =0,	    //�����ļ�ģʽ
    USBAPP_PIC_MODE     =1,	    //ͼƬģʽ
    USBAPP_TXT_MODE     =2,	    //�ı�ģʽ
    USBAPP_AUDIO_MODE   =3,	    //��Ƶģʽ
    USBAPP_VIDEO_MODE   =4,	    //��Ƶģʽ(��TS)
    USBAPP_PVR_MODE     =5,     //PVRģʽ
    USBAPP_VIDEOALL_MODE=6,	    //������Ƶģʽ(��TS)
    USBAPP_OTHER_MODE,
    USBAPP_MODE_COUNT
} Modetype_t;

typedef struct DiskInfo
{
    unsigned char	DiskIndex;			//��ӦӦ���ϲ����������
    unsigned char   DiskDevNo;			//��Ӧ�ײ����devinfo�����±�
    unsigned char 	DiskName[USB_DISK_NAME_MAX_LENGTH];
	unsigned int  	DiskSize;			//�����ܴ�С
	unsigned int 	DiskFreeSize;		//����ʣ���С
} DiskInfo_t;

typedef struct PartInfo
{
    unsigned char 	PartIndex;			//��ӦӦ���ϲ����������
    unsigned char  	PartNo;				//��Ӧ�ײ���������±�
	unsigned char 	PartName[USB_PART_NAME_MAX_LENGTH];
	char			PartIcon;			//������Ӧ�̷�,��C��D��E��
    int				PartFsType;			//�����ļ�ϵͳ����
    unsigned int 	PartSize;			//�����ܴ�С
    unsigned int 	PartFreeSize;		//����ʣ���С
} PartInfo_t;

/*----------------------------------------------------------------------------
 *	External Function Prototype
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Global functions
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time 	  : 2009-04-24
  Author		  :    xyan
  Function Name   : Usbapp_GetMsgQueue
  Description	  : ��ȡUsbӦ����Ϣ����
  Input 		  :
  Outut 		  :
  Calls 		  :
  Called by 	  :
  Global Variables:
  Others		  :
*****************************************************************************/
DRVOS_MsgQueue_t *Usbapp_GetMsgQueue(void);

#endif
