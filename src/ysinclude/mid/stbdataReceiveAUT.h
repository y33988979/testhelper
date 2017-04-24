/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : stbdataReceiveAUT.h

    Description : 九州频道跟随功能

    Others:      

    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15
                  
*******************************************************************************/
#ifndef _YSstbdata_updataAUT_
#define _YSstbdata_updataAUT_
/*----------------------------------------------------------------------------
*	Include
*----------------------------------------------------------------------------*/
#include "driver.h"
#include "dvb.h"
#include "midware.h"

/*----------------------------------------------------------------------------
*	Define
*----------------------------------------------------------------------------*/
#define APPLICATION_UPDATA_TABLE_PID		0X1da1
#define APPLICATION_UPDATA_TABLE_ID			0Xda    
#define SECTION_WAIT_TIMEOUT_AUT_MS       	2000 

#define PRIVATE_SET_DESC 					0X80
#define PRIVATE_COMPONENT_DESC 				0X01
#define DEFAULT_CHANNEL_REGION_CODE  		1
#define CABLE_DELIVERY_SYSTEM_REGION_DESC 	0X84
#define STREAM_IDENTIFIER_DESC_AUT			0x52

/*----------------------------------------------------------------------------
*	Struct
*----------------------------------------------------------------------------*/
typedef struct AppDataSection
{
    unsigned char 	aucSectionData[1024];	
    unsigned short  usDataLength;
    BOOL 			bInUsed;				
}AppDataSection_t;

enum
{
    CHANNEL_ORDER_NOT_CHANGED,
    CHANNEL_ORDER_CHANGED
};

/*存储用service 信息结构*/
typedef struct STBDATAServiceInfoTemp
{
    char 			    scServiceName[DVB_INFO_MAX_SERVICE_NAME_LENGTH];		/*service名称 */
    unsigned short		usStatus;				/* service的状态 */
    signed short		ssServiceNO;			/* 数组下标*/
    unsigned short		usServiceID;			/* Service ID */

    unsigned short		usVideoPID;				/* 视频pid */
    unsigned short		usAudio1PID;			/* 音频1pid */
    unsigned short		usPcrPID;				/* pcr pid */

    signed char         scTsNO;					/*该service 对应的TS序号*/
    unsigned char       ucServiceType;                    
    unsigned char       ucVideoType;			/* 视频类型*/
    unsigned char       ucAudio1Type;			/* 音频1类型 */

    unsigned short		usVideoEcmPID;			/*vedio ecm pid */
    unsigned short		usAudio1EcmPID;			/*audio ecm pid */
    unsigned short		usEmmPID;				/* emm pid */
    unsigned short      usCaSID;				/* CA标识符 */

    unsigned short      usChannelOrderNO;
    unsigned char       ucChnlStatus;
    unsigned char       ucChnlLock;
    unsigned char       ucAttribute;

    signed char			scVolumeOffset;			/*音量补偿值*/
    unsigned char		ucSoundChannel;			/*频道声道设定值*/

	unsigned short      usAuthority;            /*节目授权*/
	unsigned short      usPmtPid;               /*Pmtpid*/
	unsigned short      usNetChnlIndex;         /*网上频道号*/

}STBDATAServiceInfoTemp_t;

typedef struct STBDATANVODUnit_Info
{
    unsigned short  	usNVOD_TSID;
    unsigned short  	usNVOD_OriginalNetWorkID;
    unsigned short  	usNVOD_ServiceID;
    unsigned char   	ucStatus; 
    signed short     	ssServiceNO;    		/* 该频道 对应的service序号*/
}STBDATANVODUnit_t;

typedef struct STBDATANVODChannelInfoTemp
{
    STBDATANVODUnit_t 	NVODInfo[STBDATA_MAX_NVOD_OFFSET_SERVICE_NUMBER];
    signed short		ssChannelNo;          	/*频道号*/
    signed short    	ssServiceNO;    		/* 该频道 对应的service序号*/
    unsigned char       ucStatus;   			/* 频道状态 */
} STBDATANVODChannelInfoTemp_t;


/*----------------------------------------------------------------------------
 *	Global functions
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :    llh
  Function Name   : bIsAUTChanged
  Description     : 判断AUT是否改变标识
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL bIsAUTChanged(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :    llh
  Function Name   : GetChannelOrderChangedTag
  Description     : 获取频道序号变更标识
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
unsigned char GetChannelOrderChangedTag(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :    llh
  Function Name   : SetChannelOrderChangedTag
  Description     : 设置频道序号变更标识
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void SetChannelOrderChangedTag(unsigned char ucTag);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :    llh
  Function Name   : NVM_WriteChannelOrderChangedTag
  Description     : 写入频道序号变更标识
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void NVM_WriteChannelOrderChangedTag(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :    llh
  Function Name   : NVM_ReadChannelOrderChangedTag
  Description     : 读取频道序号变更标识
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void NVM_ReadChannelOrderChangedTag(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :    llh
  Function Name   : StartReceiveAppData
  Description     : 开始更新频道数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void StartReceiveAppData(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :    llh
  Function Name   : FinishReceiveAppData
  Description     : 结束更新频道数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void FinishReceiveAppData(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :    llh
  Function Name   : GetAppDataReceiveStatus
  Description     : 获取更新频道数据状态
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL GetAppDataReceiveStatus(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :    llh
  Function Name   : STBDATA_RequestApplicationData
  Description     : 申请接收频道跟随数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void STBDATA_RequestApplicationData(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :    llh
  Function Name   : UpdateApplicationData_New
  Description     : 更新应用(频道)数据(采用新的频道跟随表数据结构)
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void UpdateApplicationData_New(unsigned short StartRequestStatus);


#endif


