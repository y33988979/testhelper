/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : stbdataReceiveADTForSkyWorth.h

    Description : ��ά��湦��

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
#define SAVE_AD_MAX_BLOCK_NUMBER					15			//��������������Block�ı��

#define SAVE_BOOTLOGO_AD_FIRST_BLOCK_NO				0			//��Block0��ʼ��BootLogo(0,1,2)
#define SAVE_BOOTLOGO_AD_USE_BLOCK_NUMBER			3			//��ʹ��3��Block����BootLogo
#define SAVE_MAINMENU_AD_FIRST_BLOCK_NO				3			//��Block3��ʼ�����˵����(3,4)
#define SAVE_MAINMENU_AD_USE_BLOCK_NUMBER			2			//��ʹ��2��Block�������˵����
#define SAVE_AD_INFO_BLOCK_NO						5			//ʹ��Block5�洢���������ݺ͹����Ϣ
#define SAVE_STRIP_AD_FIRST_BLOCK_NO				6			//��Block5��ʼ��banner�����

#define BOOT_LOGO_AD_DEFAULT_SHOW_TIME				3			//��������Ĭ����ʾʱ��Ϊ3��

/*��Filter���ٽ��չ��ģʽ(Ĭ�ϲ��õ�Filter���ٽ���ģʽ)*/
#define Multi_Filter_Receive_Ad_Mode
#define Sky_Worth_Ad_Save_Flash_Mode

/*��������ݴ�Ƶ��������Ų��,����Block3��,�������ڱ��������ݶϵ�ᶪʧƵ������,
������Ա����ʵ�����ݵĴ�С����*/
#define STBDATA_AD_FLASH_SIZE          	  			(1024*50)
#define NVM_AD_MEM_INDEX_OFFSET         	 		0x0000

/*���ڱ������й��������Ϣ*/
//24 for AdUpdateInfo(0x0000-0x0018)
#define NVM_AD_MEM_UPDATEINFO_BASE        			(NVM_AD_MEM_INDEX_OFFSET + 0x0000)
#define NVM_AD_MEM_UPDATEINFO_END         			(NVM_AD_MEM_INDEX_OFFSET + 0x0018)

//34000 for NJAdPicInfo 680*50=34000(0x0018-0x84e8)
#define NVM_ADPICINFO_MEM__BASE                		(NVM_AD_MEM_INDEX_OFFSET + 0x0018)
#define NVM_ADPICINFO_MEM__END                 		(NVM_AD_MEM_INDEX_OFFSET + 0x84e8)

/*��ά���E2P�洢��ַ,��ʹ�ô�ά���,������Ա����ʵ��������е���*/
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
	AD_BOOTLOGO,  	//�������
	AD_MAINMENU, 	//���˵����
	AD_SUBMENU,   	//�Ӳ˵����
	AD_STRIPE,     	//baner �����
	AD_RCVCONFIG,	//�ն˽�����Ϣ
	AD_EPG			//��Ƶ���
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

	/*�����banner �����*/
	unsigned short      ucAdLinkageChannelNo[STBDATA_STRIPE_MAX_CHANNEL_NUMBER];	
    //unsigned short    usLinkageNetWorkID;
    unsigned short      usLinkageTsID;
    unsigned short      usLinkageServiceID;
}STBDATAAdPicInfo_t;

typedef struct STBDATAAdXMLInfo
{
	unsigned char   	ucXMLPicLinkageType;
	
	/*�����banner �����*/
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
	unsigned char       ucNameType;          // 1:area name;2:manufacturer name;3:rcvtyp name;4:portion name.��ʼ��Ϊ0xff.
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

	//�����banner �����
	unsigned short      ucAdLinkageChannelNo[STBDATA_STRIPE_MAX_CHANNEL_NUMBER];	
    //unsigned short    usLinkageNetWorkID;
    unsigned short      usLinkageTsID;
    unsigned short      usLinkageServiceID;
}STBDATANVMAdPicInfo_t;

/*���ڹ�����ݸ���*/
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
  Description     : ͨ���߼�Ƶ���Ż�ȡ�����Ϣ
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
  Description     : ͨ�����ID��ȡ����Service��Ϣ
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
  Description     : �õ���ά����Ƿ��ʼ��״̬ 
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
  Description     : ���ô�ά����ʼ��״̬
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
  Description     : ��ȡ���BAT�汾��
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
  Description     : ������BAT�汾��
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
  Description     : ��ȡ����������ʾʱ��
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
  Description     : ��flash�й�����ݶ���NVM����
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
  Description     : ��ȡ��������Ϣ
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
  Description     : �õ���ά������״̬,0Ϊ���ڸ���,1Ϊ�ɹ�,2Ϊʧ��.
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
  Description     : ���ô�ά������״̬,0Ϊ���ڸ���,1Ϊ�ɹ�,2Ϊʧ��.
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
  Description     : ���ô�ά����Ƿ��и��±�ʾ,FLASEΪû�и���,TRUEΪ�и���
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
  Description     : �õ���ά����Ƿ��и��±�ʾ,FLASEΪû�и���,TRUEΪ�и���
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
  Description     : ������ʾ��ά��濪������
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
  Description     : ����ʱ���չ��
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
  Description     : ��̨�����BAT�Ƿ��и���
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
  Description     : ��̨���¹��
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void UpdateSkyWorthAdByUpdate(void);

#endif

