/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : stbdataReceiveADTForKonka.h

    Description : 康佳广告功能

    Others:      

    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef _YSstbdataReceiveADTForKonka_
#define _YSstbdataReceiveADTForKonka_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "driver.h"
#include "dvb.h"
#include "midware.h"

/*--------------------------------------------------------------------
 * Define & Enum 
 *--------------------------------------------------------------------*/
#define KONKAPIC_LENGTH     				(1024 * 200)
#define KONKAPIC_BUFFER     				(1024 * 500)
#define KONKAPIC_MAX_NUM    				20

#define KONKA_PIC_TABLE 					0xFE
#define KONKA_PIC_PID   					0x1771
#define SECTION_WAIT_TIMEOUT_KONKA_PIC  	3000

#define KONKA_AD_BLOCK_SIZE					65528	//64K的Block
#define KONKA_STARTPIC_FIRST_SAVE_BLOCK 	4       //开机画面开始存储的Block号
#define KONKA_STARTPIC_USE_BLOCK_NUMBER		4		//开机画面最大使用Block数目

//康佳广告E2P存储地址,如果需要使用康佳广告,请开发人员根据实际进行调整
#define NVM_START_PIC_LENGTH_ADDR  			(NVM_APP_INFO_BASE + 600)
#define NVM_START_PIC_LENGTH_SIZE  				4
#define NVM_START_PIC_TYPE_ADDR  			(NVM_APP_INFO_BASE + 604)
#define NVM_START_PIC_TYPE_SIZE  				1
#define NVM_START_PIC_EXIST_ADDR  			(NVM_APP_INFO_BASE + 605)
#define NVM_START_PIC_EXIST_SIZE  				1
#define NVM_START_PIC_VERSION_ADDR  		(NVM_APP_INFO_BASE + 606)
#define NVM_START_PIC_VERSION_SIZE   			1

/*----------------------------------------------------------------------------
 *	Enum & Struct
 *----------------------------------------------------------------------------*/
enum
{
    KONKA_BMP,
    KONKA_JPG,
    KONKA_GIF,  
    KONKA_COUNT
};

enum
{
    KONKA_STARTPIC,
    KONKA_PF,
    KONKA_HOMEPAGE  
};

typedef struct KonkaPicData
{
	unsigned char bInUse;
	unsigned short *pServiceid;
	unsigned short ServiceCount;   //此图片关联service的个数
	unsigned short ServiceidReceived;
	signed char PicPosition;
	signed char Picid;
	unsigned short ServiceNo;
	unsigned char *picdata;
	unsigned char *pSegReceived; //标志pic的segment是否受到,长度同segnumber
	unsigned short SegReceivedNumber;

	unsigned char PicInfoReceived;
	unsigned int Originlength;
	unsigned int CRC32;
	unsigned short SegLength;
	unsigned short SegNumber;
	unsigned char  bCompressed;
	unsigned int CompressedLength;
	unsigned short Height;
	unsigned short Width;
	signed char Pictype;
}KonkaPicDatatmp_t;

/*----------------------------------------------------------------------------
 * Global Fuction Declare
 *---------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : UpdateKonka_GeHomepagePic
  Description     : 得到康佳主菜单广告图片
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
KonkaPicDatatmp_t * UpdateKonka_GeHomepagePic(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : UpdateKonka_GetPicOfChannelNO
  Description     : 根据频道号得到所关联的广告图片
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
KonkaPicDatatmp_t *UpdateKonka_GetPicOfChannelNO(unsigned short ChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : UpdateKonka
  Description     : 接收康佳广告
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void UpdateKonka(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : ShowStartPicForKonka
  Description     : 显示康佳开机画面
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 此函数的OSD显示为5516上的,请集成者根据版本情况更换接口  
*****************************************************************************/ 
void ShowStartPicForKonka(void);

#endif
