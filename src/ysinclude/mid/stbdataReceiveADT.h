/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : stbdataReceiveADT.h

    Description : 九州广告功能

    Others:      

    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef _YSstbdataReceiveADT_
#define _YSstbdataReceiveADT_
/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "driver.h"
#include "dvb.h"
#include "midware.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#define ADT_TABLE_ID 				0xc0  	//192
#define ADT_PID 					0x1ba1	//7073
#define SECTION_WAIT_TIMEOUT_ADT_MS	2000   

#define DEFAULT_AD_TYPE 			0xff
#define INVALID_ADID 				0xffff
#define DEFAULT_DISPLAYMODE 		0xff
#define DEFAULT_COLOR 				0xff
#define AD_CONTENT_DESC 			128
#define AD_DISPLAYMODE_DESC			129
#define AD_BACKGROUND_DESC			130
#define AD_LINKAGE_DESC				131
#define AD_PICTURE_DESC             132
#define AD_EPG_DESC                 133
#define AD_CONTROL_DESC             134
#define AD_MAINMENUAD_DESC      	135
#define AD_FUN_SET_DESC      		137 /*广告功能设置，用于自定义广告*/
#define AD_DATA_LENGTH				30*1024

#define BANNER_AD_STATUS_PIC 		0x0001
#define BANNER_AD_STATUS_TXT 		0x0002
#define BANNER_AD_STATUS_DEFAULT 	0x0

/*----------------------------------------------------------------------------
 *	Enum & Struct
 *----------------------------------------------------------------------------*/
enum
{
	ADTYPE_NONE,
	ADTYPE_BANNER,
	ADTYPE_STRIP,
	ADTYPE_BLOCK,
	ADTYPE_EPGAD,
	ADTYPE_CONTROL,
	ADTYPE_MAINMENU
};

enum
{
	ADDISPLAY_STATIC,
	ADDISPLAY_TRANSFORM,
	ADDISPLAY_PULLOUT,
	ADDISPLAY_ROLLOVER
};

enum
{
	ADSHOW_INTERVAL,
	ADSHOW_CONTINUAL
};

enum
{
	AD_NOT_IN_USE,
	AD_IN_USE
};

enum
{
	ADREGION_UP,
	ADREGION_DOWN
};

typedef struct AdDataSection
{
	unsigned char 	adSectionData[1024];	
	unsigned short 	usDataLength;
	BOOL bInUsed;				
}AdDataSection_t;

typedef struct STBDATAAdContentInfo
{
	unsigned char 	ucDisplayMode;	
	unsigned char 	ucRowNumber;		
	unsigned char 	ucFontColor;
	unsigned short 	usDisplayTime;		
	unsigned char 	*pAdText;
}STBDATAAdContentInfo_t;

typedef struct STBDATAAdDisplayModeInfo
{
	unsigned char 	ucDisplayMode;
	unsigned short 	usDisplayTime;
	unsigned short 	usDisplayInterval;
}STBDATAAdDisplayModeInfo_t;

typedef struct STBDATAAdBackgroudInfo
{
	unsigned char 	ucBackgroundType;
	unsigned char 	ucDisplayRegion;
	unsigned short	x1;
	unsigned short 	x2;
	unsigned short 	y1;
	unsigned short 	y2;
	unsigned char 	ucFrameSize;
	unsigned char 	ucFrameColor;
	unsigned char 	ucFrameTrans;
	unsigned char 	ucBackgroundColor;
	unsigned char 	ucBackTrans;
}STBDATAAdBackgroudInfo_t;

typedef struct STBDATAAdLinkageInfo
{
	unsigned short 	usAD_TSID;
	unsigned short 	usAD_NetWorkID;
	unsigned short 	usAD_ServiceID;
	unsigned char 	ucStatus; 			//所关联的频道的状态
	signed short 	ssServicelNO;   	/* 该广告对应的service序号*/
}STBDATAAdLinkageInfo_t;

typedef struct STBDATAAdPictureInfo
{
	unsigned short 	ucPic_ID;
	unsigned char 	ucPic_Type;
	unsigned short 	usPic_Width;
	unsigned short 	usPic_Heigth;
	unsigned int 	ucPic_Size;
	unsigned char 	ucPalette_Type;
	unsigned char 	ucIsCompress;
	unsigned int 	ucCompress_length;
}STBDATAAdPictureInfo_t;

typedef struct STBDATAAdEpgInfo
{
	unsigned short 	usAD_TSID;
	unsigned short 	usAD_NetWorkID;
	unsigned short 	usAD_ServiceNO;
}STBDATAAdEpgInfo_t;

typedef struct STBDATAAdUnitInfo
{
	unsigned char 	ucStatus;			//广告状态
	unsigned short 	usAdID;
	unsigned char 	ucAdType; 			/*Banner or Strip or Block*/
	unsigned char	ucAdContentCount;	//广告内容链表长度
	list_t 	AdContentList;				//广告内容链表，一行为一个节点
	list_t 	AdLinkageList;				//关联的频道链表

	STBDATAAdDisplayModeInfo_t *pAdDisplayModeInfo;
	STBDATAAdBackgroudInfo_t *pAdBackgroundInfo;
	STBDATAAdPictureInfo_t *pAdPictureInfo;
 	STBDATAAdEpgInfo_t *pAdEpgInfo;
}STBDATAAdUnitInfo_t;

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
void StartReceiveAdData(void);

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
void FinishReceiveAdData(void);

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
BOOL GetAdDataReceiveStatus(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : UpdateAdvertiseData
  Description     : 接收九州广告数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void UpdateAdvertiseData(unsigned short StartRequestStatus);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetAdTypeOfServiceNo
  Description     : 获取某service 上的DTV 广告数据类型
  Input           : ssServiceNo
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_GetAdTypeOfServiceNo(short ssServiceNo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetAdUnitofChannelNo
  Description     : 通过Service 序号和广告类型获得该频道上的广告数据
  Input           : ssServicelNo, ucAdType
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAAdUnitInfo_t *STBDATA_GetAdUnitofServiceNo(short ssServicelNo,unsigned char ucType);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetAdContentofRowNumber
  Description     : 获取AdContentList 中某一行信息
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAAdContentInfo_t *STBDATA_GetAdContentofRowNumber(unsigned char ucRowNumber,list_t *pContentList);


#endif

