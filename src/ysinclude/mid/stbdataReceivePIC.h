/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : stbdataReceivePIC.h

    Description : 九州广告图片接收

    Others:      

    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef _YSstbdataReceivePIC_
#define _YSstbdataReceivePIC_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "driver.h"
#include "dvb.h"
#include "midware.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#define PIC_DATA_LENGTH     				600*1024
#define SECTION_WAIT_TIMEOUT_PIC_MS   		4000

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
typedef struct PicDataSection
{
	unsigned char	picSectionData[1024];	
	unsigned short	usDataLength;
	BOOL			bInUsed;	
	BOOL         	IsReceiveComplete;
}PicDataSection_t;

typedef struct  STBDATAPICContentInfo
{
	unsigned short 	usPicID;
	unsigned char 	ucRowNumber;
	unsigned char 	*pPicData;
	unsigned int 	ucPicLength;
	unsigned int 	ucPicCompressLength;
}STBDATAPICContentInfo_t;

typedef struct  STBDATAPICTempInfo
{
	unsigned short	ucPicTempID;
	char			scMAXPICSectionNumber;
	BOOL   			IsReceiveOne;
	PicDataSection_t *pPICSection;
	BOOL   			IsReceiveComplete;
}STBDATAPICTempInfo_t;

/*----------------------------------------------------------------------------
 *	Global functions
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : StartReceiveAdData
  Description     : 开始更新电视广告数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void StartReceivePicData(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : FinishReceiveAdData
  Description     : 结束更新电视广告数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void FinishReceivePicData(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : GetAdDataReceiveStatus
  Description     : 获取更新电视广告数据状态
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL GetPicDataReceiveStatus(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : UpdatePictureData
  Description     : 接收九州广告图片数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void UpdatePictureData(unsigned short StartRequestStatus);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetPicContentofRowNumber
  Description     : 获取PicContentList 中某一行信息
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAPICContentInfo_t * STBDATA_GetPicUnitofPictureID(unsigned short usPictureID);

#endif


