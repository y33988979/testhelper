/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : stbdataReceiveADTForSkyWorth.h

    Description : 创维广告功能

    Others:      

    History:     1. xyan creat 2004-3-29
                 2. xyan Modify 2011-11-15 

*******************************************************************************/
#ifndef _YSstbdataReceiveADTForSkyWorth_
#define _YSstbdataReceiveADTForSkyWorth_
/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "driver.h"
#include "dvb.h"
#include "midware.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#define STBDATA_AD_MAX_NAME_LENGTH               	20
#define STBDATA_FUC_MAX_NAME_LENGTH             	20
#define STBDATA_BOOTLOGO_MAX_AD_NUMBER       		2
#define STBDATA_MAINMENU_MAX_AD_NUMBER       		2
#define STBDATA_MAINMENU_MAX_FUC_NUMBER     		15
#define STBDATA_STRIPE_MAX_CHANNEL_NUMBER    		300

#define AD_DATA_LENGTH_FOR_SKYWORTH                 (1024 * 800)
#define AD_DATA_CHANGE_MEMORY_FOR_SKYWORTH          (1024*1024)
#define MAX_PICAD_NUMBER                        	50
#define AD_USE_MAX_FILTER_NUMBER                	15

#define STBDATA_ADHEAD_DEFAULT_TABLE_ID          	0xfe
#define STBDATA_ADPICDATA_DEFAULT_TABLE_ID     		0xfd
#define AD_DATA_DEFAULT_PID                        	8002//8000         
#define AD_BAT_PID									8003//8001
#define AD_BAT_LINKAGE    							0x4a//0xA0
#define AD_BAT_PRIVATEDATA_SPECIFIER_DESC			0xF0
#define SECTION_WAIT_TIMEOUT_AD_PIC                 3000

#define SAVE_AD_BLOCK_SIZE							131064		//128K Block
#define SAVE_AD_MAX_BLOCK_NUMBER					15			//存广告数据最多可以Block的编号

#define SAVE_BOOTLOGO_AD_FIRST_BLOCK_NO				0			//从Block0开始存BootLogo(0,1,2)
#define SAVE_BOOTLOGO_AD_USE_BLOCK_NUMBER			3			//共使用3块Block来存BootLogo
#define SAVE_MAINMENU_AD_FIRST_BLOCK_NO				3			//从Block3开始存主菜单广告(3,4)
#define SAVE_MAINMENU_AD_USE_BLOCK_NUMBER			2			//共使用2块Block来存主菜单广告
#define SAVE_AD_INFO_BLOCK_NO						5			//使用Block5存储广告更新数据和广告信息
#define SAVE_STRIP_AD_FIRST_BLOCK_NO				6			//从Block5开始存banner条广告

#define BOOT_LOGO_AD_DEFAULT_SHOW_TIME				3			//开机画面默认显示时间为3秒

/*多Filter快速接收广告模式(默认采用单Filter慢速接收模式)*/
#define Multi_Filter_Receive_Ad_Mode
#define Sky_Worth_Ad_Save_Flash_Mode

/*将广告数据从频道数据中挪开,存入Block3中,避免由于保存广告数据断电会丢失频道数据,
开发人员根据实际数据的大小调整*/
#define STBDATA_AD_FLASH_SIZE          	  			(1024*50)
#define NVM_AD_MEM_INDEX_OFFSET         	 		0x0000

/*用于保存所有广告数据信息*/
//24 for AdUpdateInfo(0x0000-0x0018)
#define NVM_AD_MEM_UPDATEINFO_BASE        			(NVM_AD_MEM_INDEX_OFFSET + 0x0000)
#define NVM_AD_MEM_UPDATEINFO_END         			(NVM_AD_MEM_INDEX_OFFSET + 0x0018)

//34000 for NJAdPicInfo 680*50=34000(0x0018-0x84e8)
#define NVM_ADPICINFO_MEM__BASE                		(NVM_AD_MEM_INDEX_OFFSET + 0x0018)
#define NVM_ADPICINFO_MEM__END                 		(NVM_AD_MEM_INDEX_OFFSET + 0x84e8)

/*创维广告E2P存储地址,如使用创维广告,开发人员根据实际情况进行调整*/
#define NVM_AD_BOOTLOGO_EXIST_ADDR  				(NVM_APP_INFO_BASE + 600)
#define NVM_AD_BOOTLOGO_EXIST_SIZE   					1

#define NVM_AD_MAINMENU_EXIST_ADDR  				(NVM_APP_INFO_BASE + 601)
#define NVM_AD_MAINMENU_EXIST_SIZE   					1

#define NVM_BAT_VERSION_BASE_ADDR       			(NVM_APP_INFO_BASE + 602)
#define NVM_BAT_VERSION_BASE_SIZE              			1

#define NVM_BOOTLOGO_PIC_LENGTH_ADDR   				(NVM_APP_INFO_BASE + 603)
#define NVM_BOOTLOGO_PIC_LENGTH_SIZE    				4

#define NVM_BOOTLOGO_PIC_TYPE_ADDR      			(NVM_APP_INFO_BASE + 607)
#define NVM_BOOTLOGO_PIC_TYPE_SIZE              		1

#define NVM_AD_STRIPE_EXIST_ADDR  					(NVM_APP_INFO_BASE + 608)
#define NVM_AD_STRIPE_EXIST_SIZE   						1

#define NVM_BOOTLOGO_TIME_ADDR       				(NVM_APP_INFO_BASE + 609)
#define NVM_BOOTLOGO_TIME_SIZE                			2

/*----------------------------------------------------------------------------
 *	Enum & Struct
 *----------------------------------------------------------------------------*/
enum
{
	INVALID_MODE,
	BOOT_RECEIVE_MODE,
	UPDATE_RECEIVE_MODE
};

enum
{
	INVALID_NAME=0xff,
	AREA_NAME=1,
	MANUFACTURER_NAME=2,
	RCVTYPE_NAME=3,
	PORTION_NAME=4
};

enum
{
	ADTYPE_INVALID,
	ADTYPE_GIF,
	ADTYPE_BMP,
	ADTYPE_JPG,
	ADTYPE_PNG,
	ADTYPE_MPG
};

enum
{
	AD_INVALID,
	AD_BOOTLOGO,  	//开机广告
	AD_MAINMENU, 	//主菜单广告
	AD_SUBMENU,   	//子菜单广告
	AD_STRIPE,     	//baner 条广告
	AD_RCVCONFIG,	//终端接收信息
	AD_EPG			//视频广告
};

enum
{
	FUC_NONE,
	FUC_TV,
	FUC_INTETACTIVE,
	FUC_AUDIOBROADCAST,
	FUC_FAVORITE,
	FUC_INFO,
	FUC_VOD,
	FUC_MAIL,
	FUC_CONFIG
};

typedef struct AdInfo1KSection
{
	unsigned char		AdInfo1KData[1024];	
	unsigned short		usDataLength;
	BOOL				bInUsed;	
}AdInfo1KSection_t;

typedef struct AdInfo4KSection
{
	unsigned char		AdInfo4KData[4096];	
	unsigned short		usDataLength;
	BOOL				bInUsed;				
}AdInfo4KSection_t;

typedef struct STBDATAAdIndex
{
    BOOL                bStatus;
    unsigned short      usExtendedTableID;
    BOOL                bFirstReceived;
    unsigned char       ucSectionNumber;
    AdInfo4KSection_t   *SectionData;
}STBDATAAdIndex_t;

typedef struct STBDATAAdPicInfo
{
	unsigned char       ucInUse;
	unsigned char 		ucAdName[STBDATA_AD_MAX_NAME_LENGTH];
	unsigned short 		usAdID;
	unsigned char  		ucAdType;
	unsigned short 		usAdPosX;
	unsigned short 		usAdPosY;
	unsigned short 		usAdHeight;
	unsigned short 		usAdWidth;
	unsigned short 		usAdDuration;
	unsigned short      usAdVersion;

	unsigned int      	uiAdOriginalSize;
	unsigned short      usAdSegLength;
	unsigned short      usAdSegNum;
	unsigned char       ucAdCompressedFlag;
	unsigned int       	uiAdCompressedSize;

	unsigned int       	uiAdDataReceived;

	unsigned char*     	ucAdData;
	
	unsigned char       ucSaveBlockNo;
	unsigned int        usBlockAddr;

	unsigned char       ucAdLinkageType;

	unsigned int       	uiFucationName;

	/*如果是banner 条广告*/
	unsigned short      ucAdLinkageChannelNo[STBDATA_STRIPE_MAX_CHANNEL_NUMBER];	
    //unsigned short    usLinkageNetWorkID;
    unsigned short      usLinkageTsID;
    unsigned short      usLinkageServiceID;
}STBDATAAdPicInfo_t;

typedef struct STBDATAAdXMLInfo
{
	unsigned char   	ucXMLPicLinkageType;
	
	/*如果是banner 条广告*/
	unsigned short   	ucXMLPicLinkageChannelNo;	
    //unsigned short   	usXMLPicLinkageNetworkID;
    unsigned short      usXMLPicLinkageTsID;
    unsigned short      usXMLPicLinkageServiceID;
	
	unsigned char 		ucXMLName[STBDATA_AD_MAX_NAME_LENGTH];
	unsigned short      usXMLFileNumber;
	unsigned short      usXMLFileVersion;
	unsigned char       ucXMLFormat;
	unsigned short      usXMLPosX;
	unsigned short      usXMLPosY;
	unsigned short      usWidth;
	unsigned short      usHeight;
	unsigned short      usDuration;

	unsigned int        uiFucationName;

	/*RCVConfig Info*/
	unsigned char       ucNameType;          // 1:area name;2:manufacturer name;3:rcvtyp name;4:portion name.初始化为0xff.
	unsigned char       areaName[STBDATA_AD_MAX_NAME_LENGTH];
	unsigned char       manufactureName[STBDATA_AD_MAX_NAME_LENGTH];
	unsigned char       rcvtypeName[STBDATA_AD_MAX_NAME_LENGTH];
	unsigned char       portionName[STBDATA_AD_MAX_NAME_LENGTH];
	unsigned int        stbstartid;
	unsigned int        stbendid;

	/*PowerOnPara*/
	unsigned char        AdServiceName[STBDATA_AD_MAX_NAME_LENGTH];
	unsigned int         AdFrequency;
	unsigned int         AdSymolRate;
	unsigned char        AdModulation;
	unsigned short       AdNetworkID;
	unsigned short       AdTsID;
	unsigned short       AdServiceID;

	unsigned short       AdVideoPID;
	unsigned short       AdAudioPID[STBDATA_AD_MAX_NAME_LENGTH];

}STBDATAAdXMLInfo_t;


typedef struct STBDATAAdControlInfo
{
	unsigned short   	usXMLExtensionTableIDStart;
	unsigned short   	usXMLExtensionTableIDEnd;
	unsigned int     	uiXMLSize;

	unsigned short    	usPicInfoExtensionTableIDStart;
	unsigned short    	usPicInfoExtensionTableIDEnd;
	unsigned int      	uiPicInfoSize;
}STBDATAAdControlInfo_t;

typedef struct STBDATANVMAdPicInfo
{
	unsigned char   	ucInUse;
	unsigned char 		ucAdName[STBDATA_AD_MAX_NAME_LENGTH];
	unsigned short 		usAdID;
	unsigned char  		ucAdType;
	unsigned short 		usAdPosX;
	unsigned short 		usAdPosY;
	unsigned short 		usAdHeight;
	unsigned short 		usAdWidth;
	unsigned short 		usAdDuration;
	unsigned short      usAdVersion;

	unsigned int        uiAdOriginalSize;
	unsigned char       ucAdCompressedFlag;
	unsigned int        uiAdCompressedSize;
	
	unsigned char       ucSaveBlockNo;
	unsigned int        usBlockAddr;

	unsigned char       ucAdLinkageType;

	unsigned int        uiFucationName;

	//如果是banner 条广告
	unsigned short      ucAdLinkageChannelNo[STBDATA_STRIPE_MAX_CHANNEL_NUMBER];	
    //unsigned short    usLinkageNetWorkID;
    unsigned short      usLinkageTsID;
    unsigned short      usLinkageServiceID;
}STBDATANVMAdPicInfo_t;

/*用于广告数据更新*/
typedef struct STBDATAAdUpdateInfo
{
	unsigned int        uiFrequency;
	unsigned char     	ucModulation;
	unsigned int        uiSymbolRate;

	unsigned char     	ucAdHeadDataTableID;
	unsigned char    	ucADPicDataTableID;
	unsigned short    	usDownloadPid;
}STBDATAAdUpdateInfo_t;

typedef struct BATUpdateInfo
{
	unsigned short   	usNetWorkID;
	unsigned short   	usTSID;
	unsigned short   	usServiceID;

	unsigned int       	uiFrequency;
	unsigned char     	ucModulation;
	unsigned int        uiSymbolRate;

	unsigned char     	ucAdHeadDataTableID;
	unsigned char     	ucADPicDataTableID;

	unsigned short    	usPMTPid;
	unsigned short    	usDownloadPid;

	BOOL                bBATInfoReceived;
	BOOL                bNITInfoReceived;
	BOOL                bPATReceived;
	BOOL               	bPMTReceived;
}BATUpdateInfo_t;

/*----------------------------------------------------------------------------
 *	Global functions
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :  xyan  
  Function Name   : GetSkyWorthAdStripeByChannelNo
  Description     : 通过逻辑频道号获取广告信息
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
STBDATAAdPicInfo_t *GetSkyWorthAdStripeByChannelNo(unsigned short usChannelNo);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :  xyan  
  Function Name   : GetLinkageServiceByAdID
  Description     : 通过广告ID获取关联Service信息
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
STBDATAServiceInfo_t *GetLinkageServiceByAdID(unsigned short usAdID);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :  xyan  
  Function Name   : GetAdInitStatus
  Description     : 得到创维广告是否初始化状态 
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL GetAdInitStatus(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :  xyan  
  Function Name   : SetAdInitStatus
  Description     : 设置创维广告初始化状态
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void SetAdInitStatus(BOOL Flag);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :  xyan  
  Function Name   : STBDATA_NVMReadAdBATVersion
  Description     : 读取广告BAT版本号
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_NVMReadAdBATVersion(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :  xyan  
  Function Name   : STBDATA_NVMWriteAdBATVersion
  Description     : 保存广告BAT版本号
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteAdBATVersion(unsigned char flag);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :  xyan  
  Function Name   : STBDATA_NVMReadBootLogoTime
  Description     : 读取开机画面显示时间
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned short  STBDATA_NVMReadBootLogoTime(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :    llh
  Function Name   : STBDATA_LoadAdFlashData
  Description     : 将flash中广告数据读入NVM缓冲
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_LoadAdFlashData(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :  xyan  
  Function Name   : ReadAdUpdateInfo
  Description     : 读取广告更新信息
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/  
void ReadAdUpdateInfo(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :  xyan  
  Function Name   : GetAdUpdateAppStatus
  Description     : 得到创维广告更新状态,0为正在更新,1为成功,2为失败.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
unsigned char GetAdUpdateAppStatus(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :  xyan  
  Function Name   : SetAdUpdateAppStatus
  Description     : 设置创维广告更新状态,0为正在更新,1为成功,2为失败.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void SetAdUpdateAppStatus(unsigned char  Flag );

/*****************************************************************************
  Date&Time       : 2009-04-24 
  Author          :  xyan  
  Function Name   : SetAdHaveUpdateStatus
  Description     : 设置创维广告是否有更新标示,FLASE为没有更新,TRUE为有更新
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void SetAdHaveUpdateStatus(BOOL Flag );

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :  xyan  
  Function Name   : GetAdHaveUpdateStatus
  Description     : 得到创维广告是否有更新标示,FLASE为没有更新,TRUE为有更新
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL GetAdHaveUpdateStatus(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :  xyan  
  Function Name   : ShowSkyWorthAdBootLogoPicture
  Description     : 开机显示创维广告开机画面
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void ShowSkyWorthAdBootLogoPicture(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :  xyan  
  Function Name   : UpdateSkyWorthAd
  Description     : 开机时接收广告
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void  UpdateSkyWorthAd(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :  xyan  
  Function Name   : UpdateAdBATByUpdate
  Description     : 后台检测广告BAT是否有更新
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void UpdateAdBATByUpdate(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :  xyan  
  Function Name   : UpdateSkyWorthAdByUpdate
  Description     : 后台更新广告
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void UpdateSkyWorthAdByUpdate(void);

#endif

