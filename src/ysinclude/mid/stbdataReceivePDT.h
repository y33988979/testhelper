/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : stbdataReceivePDT.h

    Description : 九州开机画面功能

    Others:      

    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef _YSstbdataReceivePDT_
#define _YSstbdataReceivePDT_
/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "driver.h"
#include "dvb.h"
#include "midware.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#define PDT_TABLE_ID 					0XCA
#define BGP_TABLE_ID 					0XCB
#define ADP_TABLE_ID 					0XC9

#define PDT_PID 						0X1BA0
#define SECTION_WAIT_TIMEOUT_PDT_MS     4000

/*开发人员根据FLASH的BLOCK大小及具体规范要求确定图片的最大数据大小!!!!*/
#define MAX_PIC_DATA_LENGTH 			(1024 *128 - 8)
#define MAX_PIC621_DATA_LENGTH 			(1024 *256-8)

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
typedef struct PICFile
{
    unsigned int uiSourceFileLength;
    int uiTimeDelay;
    unsigned short usMaxDataBlockNumber;
    unsigned short usDataBlockReceivedNumber;
    unsigned char *pBlockReceived;
    unsigned char *pCompressFileData;
    BOOL bReceiveComplete;
    BOOL bFirstReceived;
    unsigned char ucVersionNo;
}PICFile_t;

typedef enum PDTMode
{
	PDT_DEFAULT_MODE,
	PDT_NORMAL_RESERVE_MODE,
	PDT_TIMEOUT_DELAY_MODE
}PDTMode_e;

/*----------------------------------------------------------------------------
 *	Global functions
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :   xyan
  Function Name   : StartReceivePDT
  Description     : 开始更新PDT 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void StartReceivePDT(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :   xyan
  Function Name   : FinishReceivePDT
  Description     : 结束更新PDT 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void FinishReceivePDT(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :   xyan
  Function Name   : GetPDTReceiveStatus
  Description     : 获取更新PDT 数据状态
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL GetPDTReceiveStatus(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :   xyan
  Function Name   : GetPDTMode
  Description     : 获取图片的打包模式
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 普通存储or 超时等待
*****************************************************************************/
char GetPDTMode(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :   xyan
  Function Name   : Free_PDTCompressFileData
  Description     : 释放256KB 开机画面空间
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void Free_PDTCompressFileData(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :   xyan
  Function Name   : RequestPDTWaitting
  Description     : 等待请求收取PDT数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void RequestPDTWaitting(unsigned short StartRequestStatus);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :   xyan
  Function Name   : ShowBootPictureForJZAd
  Description     : 显示九州开机广告图片
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void ShowBootPictureForJZAd(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :   xyan
  Function Name   : FinishBootPicShowForJZAd      
  Description     : 完成九州Delay模式广告显示
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void FinishBootPicShowForJZAd(void);

#endif

