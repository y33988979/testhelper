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
    USBAPP_BMP_TYPE=0,	//BMP图片
    USBAPP_JPG_TYPE=1,	//JPEG图片
    USBAPP_GIF_TYPE,	//GIF图片
    USBAPP_TEXT_TYPE=5,	//TXT文本
    USBAPP_VIDEO_TYPE=6,//video文件
    USBAPP_TS_TYPE=8,	//TS文件
    USBAPP_MP3_TYPE=12,	//MP3文件
    USBAPP_PCM_TYPE=13,	//PCM文件
    USBAPP_MPG_TYPE=14,	//MPG文件
    USBAPP_VIDEO_CONFIG_TYPE =15,
    USBAPP_OTHER_TYPE,
    USBAPP_TYPE_COUNT
} Filetype_t;

typedef enum
{
    USBAPP_FILE_MODE    =0,	    //所有文件模式
    USBAPP_PIC_MODE     =1,	    //图片模式
    USBAPP_TXT_MODE     =2,	    //文本模式
    USBAPP_AUDIO_MODE   =3,	    //音频模式
    USBAPP_VIDEO_MODE   =4,	    //视频模式(无TS)
    USBAPP_PVR_MODE     =5,     //PVR模式
    USBAPP_VIDEOALL_MODE=6,	    //所有视频模式(有TS)
    USBAPP_OTHER_MODE,
    USBAPP_MODE_COUNT
} Modetype_t;

typedef struct DiskInfo
{
    unsigned char	DiskIndex;			//对应应用上层磁盘索引号
    unsigned char   DiskDevNo;			//对应底层磁盘devinfo数组下标
    unsigned char 	DiskName[USB_DISK_NAME_MAX_LENGTH];
	unsigned int  	DiskSize;			//磁盘总大小
	unsigned int 	DiskFreeSize;		//磁盘剩余大小
} DiskInfo_t;

typedef struct PartInfo
{
    unsigned char 	PartIndex;			//对应应用上层分区索引号
    unsigned char  	PartNo;				//对应底层分区数组下标
	unsigned char 	PartName[USB_PART_NAME_MAX_LENGTH];
	char			PartIcon;			//分区对应盘符,如C、D、E等
    int				PartFsType;			//分区文件系统类型
    unsigned int 	PartSize;			//分区总大小
    unsigned int 	PartFreeSize;		//分区剩余大小
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
  Description	  : 获取Usb应用消息队列
  Input 		  :
  Outut 		  :
  Calls 		  :
  Called by 	  :
  Global Variables:
  Others		  :
*****************************************************************************/
DRVOS_MsgQueue_t *Usbapp_GetMsgQueue(void);

#endif
