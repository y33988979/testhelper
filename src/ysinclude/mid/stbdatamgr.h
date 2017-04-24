/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : stbdatamgr.h

    Description : 频道数据接口管理

    Others:      

    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef	__YSSTBDATAMGR_
#define	__YSSTBDATAMGR_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "midware.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#define STBDATA_INVALID_TS_NO				-1
#define STBDATA_INVALID_SERVICE_NO			-1
#define STBDATA_INVALID_TV_CHANNEL_NO		-1
#define STBDATA_DEFAULT_MAX_TV_CHANNEL_NO   0x7fff
#define STBDATA_INVALID_RADIO_CHANNEL_NO	-1

#define STBDATA_FIRST_CHANNELINFO_NO		0
#define DEFAULT_NETWORKID    				0x1FFF
#define DEFAULT_TSID                		0x1FF
#define STBDATA_DEFAULT_VOLUME_OFFSET       0x00

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
typedef enum STBDATATsInfoStatus
{
	STBDATA_TS_FREE,
	STBDATA_TS_IN_USE
}STBDATATsInfoStatus_e;

typedef enum STBDATABATInfoStatus
{
	STBDATA_BAT_FREE,
	STBDATA_BAT_IN_USE
}STBDATABATInfoStatus_e;

typedef enum STBDATAServiceInfoStatus
{
	STBDATA_SERVICE_FREE,
	STBDATA_SERVICE_IN_USE
}STBDATAServiceInfoStatus_e;

typedef enum STBChannelInfoStatus
{
	STBDATA_CHANNEL_FREE,
	STBDATA_CHANNEL_IN_USE,
	STBDATA_CHANNEL_SKIP
}STBChannelInfoStatus_e;

typedef enum STBChannelInfoAttribute
{
	STBDATA_CHANNEL_NORMAL,
	STBDATA_CHANNEL_FAVORITE
}STBChannelInfoAttribute_e;

typedef enum STBDATATsInversionStatus
{
	STBDATA_TS_NOT_INVERSION,
	STBDATA_TS_INVERSION,
	STBDATA_TS_INVALID_INVERSION
}STBDATATsInversionStatus_e;

typedef enum STBSoundChannel
{
	STB_SOUND_STEREO,
	STB_SOUND_LEFT_CHANNEL,
	STB_SOUND_RIGHT_CHANNEL,
	STB_SOUND_SINGLE_CHANNEL,
	STB_SOUND_COUNT
}STBSoundChannel_e;

typedef enum STBDATAChannelLockStatus
{
	STBDATA_CHANNEL_NOT_LOCKED,
	STBDATA_CHANNEL_LOCKED
}STBDATAChannelLockStatus_e;

/*----------------------------------------------------------------------------
 *	Global functions
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_PrintEventData
  Description     : 打印NVOD时移和参考业务以及节目类型为0的Service的Event信息
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_PrintEventData(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_WatchChnlExtendedEventInfo
  Description     : 查看频道所有扩展Event信息接口
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_WatchChnlExtendedEventInfo(unsigned short usChannelNo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_PrintEventData
  Description     : 以TXT格式打印出Service信息
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_PrintServiceData(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_PrintServiceDataNew
  Description     : 以XML格式打印出Service信息
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_PrintServiceDataNew(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_VerifyTsNO
  Description     : 检查TS 号的有效性
  Input           : TS 号
  Outut           : true:无效；false:有效
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_VerifyTsNO(signed short ssTsNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_VerifyBATNo
  Description     : 检查TS 号的有效性
  Input           : TS 号
  Outut           : true:无效；false:有效
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_VerifyBATNo(signed short ssBATNo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_VerifyServiceNO
  Description     : 检查service 号的有效性
  Input           : service 号
  Outut           : true:无效；false:有效
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_VerifyServiceNO(signed short  ssServiceNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_VerifyMosaicNO
  Description     : 检查mosaic 号的有效性
  Input           : service 号
  Outut           : true:无效；false:有效
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_VerifyMosaicNO(signed short  ssMosaicNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_VerifyTVChannelNO
  Description     : 检查电视节目频道 号的有效性
  Input           : 电视节目频道 号
  Outut           : true:无效；false:有效
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_VerifyTVChannelNO(signed short ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_VerifyRadioChannelNO
  Description     : 检查广播节目频道 号的有效性
  Input           : 广播节目频道 号
  Outut           : true:无效；false:有效
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_VerifyRadioChannelNO(signed short ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_VerifyNVODChannelNO
  Description     : 检查广播节目频道 号的有效性
  Input           : 广播节目频道 号
  Outut           : true:无效；false:有效
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_VerifyNVODChannelNO(signed short ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_VerifyJZDBCChnlNO
  Description     : 检查数据广播节目频道号的有效性
  Input           : 数据广播节目频道 号
  Outut           : true:无效；false:有效
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_VerifyJZDBCChnlNO(signed short ssDBChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitTSInfo
  Description     : 初始化TS 列表数组
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitTSInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitBATInfo
  Description     : 初始化BAT列表数组
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitBATInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitServiceInfo
  Description     : 初始化service列表数组
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitServiceInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitSingleServiceInfo
  Description     : 初始化指定ServiceInfo结构
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitSingleServiceInfo(STBDATAServiceInfo_t *pSTBServiceInfo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitTVChannelInfo
  Description     : 初始化电视节目列表数组
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitTVChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitRadioChannelInfo
  Description     : 初始化广播节目列表数组
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitRadioChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitSingleChannelInfo
  Description     : 初始化指定STBChannelInfo结构
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitSingleChannelInfo(STBDATAChannelInfo_t *pSTBChannelInfo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitNVODChannelInfo
  Description     : 初始化广播节目列表数组
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitNVODChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitMosaicChannelInfo
  Description     : 初始化马赛克节目列表数组
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitMosaicChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitJZDBCInfo
  Description     : 初始化数据广播节目列表数组
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitJZDBCInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitServiceAudioInfo
  Description     : 初始化Service伴音列表数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitServiceAudioInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitSingleServiceAudioInfo
  Description     : 初始化指定ServiceInfo伴音结构
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitSingleServiceAudioInfo(STBDATAServiceAudioInfo_t *pSTBServiceAudioInfo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitServiceSubtitleInfo
  Description     : 初始化Service多Subtitle列表数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitServiceSubtitleInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitSingleServiceSubtitleInfo
  Description     : 初始化指定Service多Subtitle结构
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitSingleServiceSubtitleInfo(STBDATAServiceSubtitleInfo_t *pSubtitleInfo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :    xyan
  Function Name   : STBDATA_InitServiceInformation
  Description     : 初始化serviceInformation列表数组
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitServiceInformation(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitSTBDATAInfo
  Description     : 初始化STB DATA
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitSTBDATAInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_DeleteTsInfoOfNO
  Description     : 根据输入的TS号删除TS数据
  Input           : 要删除的TS的TS号
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_DeleteTsInfoOfNO(signed short ssTsNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_DeleteAllTsInfoByTsNO
  Description     : 根据输入的TS号删除TS数据及对应的service 及频道数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_DeleteAllTsInfoByTsNO(signed short ssTsNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_DeleteServiceInfoOfNO
  Description     : 根据输入的service 数据号删除频道数据
  Input           : 要删除的service 的service 号
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_DeleteServiceInfoOfNO(signed short ssServiceNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_DeleteTVChannelInfoOfNO
  Description     : 根据输入的电视频道 数据号删除电视频道数据
  Input           : 要删除的电视频道 数据 的电视频道 数据号
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_DeleteTVChannelInfoOfNO(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_DeleteRadioChannelInfoOfNO
  Description     : 根据输入的广播频道 数据号删除广播频道数据
  Input           : 要删除的广播频道 数据 的广播频道 数据号
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_DeleteRadioChannelInfoOfNO(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_DeleteNVODChannelInfoOfNO
  Description     : 根据输入的NVOD 频道号删除NVOD 频道数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_DeleteNVODChannelInfoOfNO(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_DeleteMosaicChnlInfoOfNO
  Description     : 根据输入的马赛克频道号删除马赛克频道数据
  Input           :  
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_DeleteMosaicChnlInfoOfNO(signed short ssMosaicNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_DeleteJZDbcChannelInfoOfNO
  Description     : 根据输入的Dbc频道数据号删除Dbc频道数据
  Input           : 要删除的Dbc频道数据的Dbc频道 数据号
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_DeleteJZDbcChannelInfoOfNO(signed short  ssDBCChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_DeleteServiceAudioInfoOfNO
  Description     : 根据输入的AudioNo号删除该AudioInfo下所有伴音
  Input           :  
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_DeleteServiceAudioInfoOfNO(signed short ssAudioNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_DeleteServiceSubtitleInfoOfNO
  Description     : 根据输入的SubtitleNo号删除该SubtitleInfo下所有Subtitle
  Input           :  
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_DeleteServiceSubtitleInfoOfNO(signed short ssSubtitleNo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :    xyan
  Function Name   : STBDATA_DeleteServiceInformationOfID
  Description     : 根据输入的service ID号删除频道简介 数据
  Input           :  
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_DeleteServiceInformationOfID(unsigned short usNetID,unsigned short usTSID,unsigned short usServiceID);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetFreeTsInfo
  Description     : 得到空闲的TS数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATATsInfo_t * STBDATA_GetFreeTsInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetFreeServiceInfo
  Description     : 得到空闲的service数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceInfo_t * STBDATA_GetFreeServiceInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetFreeTVChannelInfo
  Description     : 得到空闲的电视节目数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t * STBDATA_GetFreeTVChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetFreeRadioChannelInfo
  Description     : 得到空闲的广播节目数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t * STBDATA_GetFreeRadioChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetFreeNVODChannelInfo
  Description     : 得到空闲的TS数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATANVODChannelInfo_t * STBDATA_GetFreeNVODChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetFreeMosaicChannelInfo
  Description     : 得到空闲的马赛克数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAMosaicInfo_t * STBDATA_GetFreeMosaicChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetFreeJZDBCInfo
  Description     : 得到空闲的数据广播数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATABroadcastInfo_t *  STBDATA_GetFreeJZDBCInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetFreeBATInfo
  Description     : 得到空闲的BAT数据 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATABATInfo_t * STBDATA_GetFreeBATInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetFreeServiceAudioInfo
  Description     : 得到空闲的Service多伴音数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceAudioInfo_t * STBDATA_GetFreeServiceAudioInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetFreeServiceSubtitleInfo
  Description     : 得到空闲的Service多Subtitle数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceSubtitleInfo_t * STBDATA_GetFreeServiceSubtitleInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :    xyan
  Function Name   : STBDATA_GetFreeServiceInformation
  Description     : 得到空闲的serviceInformation 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATANVMServiceInformation_t *STBDATA_GetFreeServiceInformation(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetNextTSInfoInUse
  Description     : 取得从当前号开始第一个使用中的Ts 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATATsInfo_t *STBDATA_GetNextTSInfoInUse(signed short  ssTsNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetNextServiceInfoInUse
  Description     : 取得从当前号开始第一个使用中的service 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceInfo_t  *STBDATA_GetNextServiceInfoInUse(signed short  ssServiceNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetNextTVChannelInfoInUse
  Description     : 取得从当前号开始第一个使用中的电视频道 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t  *STBDATA_GetNextTVChannelInfoInUse(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetNextRadioChannelInfoInUse
  Description     : 取得从当前号开始第一个使用中的广播频道 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t  *STBDATA_GetNextRadioChannelInfoInUse(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTsInfoInUseNo
  Description     : 取得使用中的TS 个数
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short  STBDATA_GetTsInfoInUseNo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceInfoInUseNo
  Description     : 取得使用中的service 个数
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
signed short  STBDATA_GetServiceInfoInUseNo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceInfoInUseNo
  Description     : 取得使用中的service 个数
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short  STBDATA_GetServiceInfoInUseNo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetBATInfoInUseNo
  Description     : 取得使用中的bat 个数
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBDATA_GetBATInfoInUseNo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetMosaicInfoInUseNo
  Description     : 取得使用中的mosaic  个数
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBDATA_GetMosaicInfoInUseNo (void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetJZDBCInfoInUseNo
  Description     : 取得使用中的数据广播  个数
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short  STBDATA_GetJZDBCInfoInUseNo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTVChannelInfoInUseNo
  Description     : 取得使用中的电视节目频道个数
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short  STBDATA_GetTVChannelInfoInUseNo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetRadioChannelInfoInUseNo
  Description     : 取得使用中的广播节目频道个数
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short  STBDATA_GetRadioChannelInfoInUseNo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetNVODChannelInfoInUseNo
  Description     : 取得使用中的Nvod节目频道个数
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short  STBDATA_GetNVODChannelInfoInUseNo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceAudioInfoInUseNo
  Description     : 统计使用中的所有多伴音信息
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned short STBDATA_GetServiceAudioInfoInUseNo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceSubtitleInfoInUseNo
  Description     : 统计使用中的所有多Subtitle信息
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned short STBDATA_GetServiceSubtitleInfoInUseNo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceInformationInUseNo
  Description     : 取得使用中的ServiceInformation个数
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned short  STBDATA_GetServiceInformationInUseNo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetCountOfServiceByTSNO
  Description     : 取得同一TS 下service个数
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBDATA_GetCountOfServiceByTSNO(signed short ssTsNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetCountOfTVServiceByTSNO
  Description     : 取得同一TS 下电视service个数
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBDATA_GetCountOfTVServiceByTSNO(signed short ssTsNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetCountOfTVServiceByTSNOForSubSection
  Description     : 取得同一TS 下电视service 个数(手动分段搜索专用)
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 不判断此Service是否加入到电视节目列表中
*****************************************************************************/ 
signed short STBDATA_GetCountOfTVServiceByTSNOForSubSection(signed short ssTsNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetCountOfRadioServiceByTSNO
  Description     : 取得同一TS 下广播service 个数
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBDATA_GetCountOfRadioServiceByTSNO(signed short ssTsNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetCountOfRadioServiceByTSNOForSubSection
  Description     : 取得同一TS 下广播service 个数(手动分段搜索专用)
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 不判断此Service是否加入到广播节目列表中
*****************************************************************************/ 
signed short STBDATA_GetCountOfRadioServiceByTSNOForSubSection(signed short ssTsNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetCountOfNvodReferenceServiceByTSNO
  Description     : 取得同一TS 下NVOD service 个数
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBDATA_GetCountOfNvodReferenceServiceByTSNO(signed short ssTsNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetCountOfDBCServiceByTSNO
  Description     : 取得同一TS 下数据广播service 个数
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBDATA_GetCountOfDBCServiceByTSNO(signed short ssTsNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTsInfoOfID
  Description     : 根据network_id 和ts_id 取得TS 数据
  Input           : network_id 和ts_id 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATATsInfo_t *STBDATA_GetTsInfoOfID(unsigned short  usNetID, unsigned short usTsID);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTsInfoOfLinkageID
  Description     : 根据LinkageTsID 取得TS 数据(在创维广告中使用)
  Input           : LinkageTsID 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATATsInfo_t *STBDATA_GetTsInfoOfLinkageID(unsigned short usLinkageTsID);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTsInfoOfTsNO
  Description     : 根据Ts 号取得Ts数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATATsInfo_t *STBDATA_GetTsInfoOfTsNO(signed short  ssTsNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTsInfoOfServiceNO
  Description     : 根据service 号取得Ts数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATATsInfo_t *STBDATA_GetTsInfoOfServiceNO(signed short  ssServiceNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTsInfoOfTVChannelNO
  Description     : 根据电视频道号取得Ts数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATATsInfo_t *STBDATA_GetTsInfoOfTVChannelNO(signed short ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTsInfoOfRadioChannelNO
  Description     : 根据广播频道号取得Ts数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATATsInfo_t *STBDATA_GetTsInfoOfRadioChannelNO(signed short ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTsInfoOfTVOrRadioChannelNO
  Description     : 根据广播或者电视频道号取得Ts数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATATsInfo_t *STBDATA_GetTsInfoOfTVOrRadioChannelNO(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTsInfoByDelivery
  Description     : 根据Delivery 取得TS 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATATsInfo_t * STBDATA_GetTsInfoByDelivery(STBDATADeliveryInfo_t *pDelivery);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_SetTsInversionStatus
  Description     : 根据Delivery 设置TS 反转标志
  Input           : 
  Outut           :    
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char  STBDATA_GetTsInversionStatus(unsigned int    uiFrequency,unsigned int   uiSymbolRate, unsigned short  usModulation);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTsInversionStatus
  Description     : 根据Delivery 取得TS 反转标志
  Input           : 
  Outut           : 反转标志
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_SetTsInversionStatus(unsigned int   uiFrequency,unsigned int  uiSymbolRate, unsigned short  usModulation, unsigned char ucInversion);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTsInfoOfID
  Description     : 根据network_id 和ts_id 取得TS 数据
  Input           : network_id 和ts_id 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceInfo_t *STBDATA_GetFirstServiceInfoOfTsNO(unsigned char ucTsNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceInfoOfServiceNO
  Description     : 根据service 号取得service 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceInfo_t *STBDATA_GetServiceInfoOfServiceNO(signed short  ssServiceNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceInfoOfTVChannelNO
  Description     : 根据电视节目频道号取得service 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceInfo_t *STBDATA_GetServiceInfoOfTVChannelNO(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceInfoOfRadioChannelNO
  Description     : 根据广播节目频道号取得service 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceInfo_t *STBDATA_GetServiceInfoOfRadioChannelNO(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceInfoOfRadioChannelNO
  Description     : 根据广播或者电视节目频道号取得service 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceInfo_t *STBDATA_GetServiceInfoOfTVorRadioChannelNO(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceInfoOfServiceNameAndType
  Description     : 根据Service名字和类型取得service 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceInfo_t *STBDATA_GetServiceInfoOfServiceNameAndType(char *scServiceName, unsigned char ucServiceType);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceInfoOfServiceName
  Description     : 根据Service名字取得service 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceInfo_t *STBDATA_GetServiceInfoOfServiceName(char * scServiceName);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceInfoOfID
  Description     : 根据network_id ts_id和service_id取得service 数据
  Input           : network_id   ts_id  和 service_id
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceInfo_t *STBDATA_GetServiceInfoOfID(unsigned short  usNetID, unsigned short  usTsID, unsigned short  usServiceID);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceInfoByID
  Description     : 根据LinkageTsIDs和LinkageServiceID取得service数据(在创维广告中使用)
  Input           : LinkageTsIDs和LinkageServiceID
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceInfo_t *STBDATA_GetServiceInfoOfLinkageID(unsigned short  usLinkageTsID, unsigned short  usLinkageServiceID);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceInfoOfServiceID
  Description     : 根据service_id   取得service 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceInfo_t *STBDATA_GetServiceInfoOfServiceID(unsigned short usServiceId);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_SetServiceExtendedData
  Description     : 初始化service数据使用的扩展数据结构
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_SetServiceExtendedData(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_ClearServiceExtendedData
  Description     : 清除service数据使用的扩展数据结构
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_ClearServiceExtendedData(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_TrimOverdueEventData
  Description     : 删除service数据中所有过期的Event信息
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_TrimOverdueEventData(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTVChannelInfoOfTVChannelNO
  Description     : 根据电视频道 号取得电视频道 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t *STBDATA_GetTVChannelInfoOfTVChannelNO(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetRadioChannelInfoOfRadioChannelNO
  Description     : 根据广播频道 号取得广播频道 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t *STBDATA_GetRadioChannelInfoOfRadioChannelNO(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetChannelInfoOfTVorRadioChannelNO
  Description     : 根据频道号获取电视或者广播的频道数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t *STBDATA_GetChannelInfoOfTVorRadioChannelNO(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetJZDBCInfoOfChnlNo
  Description     : 根据数据广播频道号获取数据广播信息
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATABroadcastInfo_t *STBDATA_GetJZDBCInfoOfChnlNo(signed short ssDBCChnlNo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetJZDBCInfoOfServiceNo
  Description     : 根据数据广播的ServiceNo获取数据广播信息
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATABroadcastInfo_t *STBDATA_GetJZDBCInfoOfServiceNo(signed short ServiceNo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTVChannelInfoOfServiceNo
  Description     : 根据service 号取得电视频道 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t  *STBDATA_GetTVChannelInfoOfServiceNo(signed short  ssServiceNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetRadioChannelInfoOfServiceNo
  Description     : 根据service 号取得广播频道 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t  *STBDATA_GetRadioChannelInfoOfServiceNo(signed short  ssServiceNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetNVODChannelInfoOfServiceNo
  Description     : 根据service 号取得广播频道 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATANVODChannelInfo_t  *STBDATA_GetNVODChannelInfoOfServiceNo(signed short  ssServiceNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetNextTVChannelInfoForDTV
  Description     : 取得下一个电视频道数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t  *STBDATA_GetNextTVChannelInfoForDTV(signed short    ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetPrevTVChannelInfoForDTV
  Description     : 取得上一个电视频道数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t  *STBDATA_GetPrevTVChannelInfoForDTV(signed short    ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetNextRadioChannelInfoForDTV
  Description     : 取得下一个广播频道数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t  *STBDATA_GetNextRadioChannelInfoForDTV(signed short    ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetPrevRadioChannelInfoForDTV
  Description     : 取得上一个广播频道数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t  *STBDATA_GetPrevRadioChannelInfoForDTV(signed short    ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetNextTVChannelInfo
  Description     : 取得下一个电视频道数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t  *STBDATA_GetNextTVChannelInfo(signed short    ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetNextRadioChannelInfo
  Description     : 取得下一个广播频道数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t  *STBDATA_GetNextRadioChannelInfo(signed short    ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetNextTVOrRadioChannelInfo
  Description     : 取得下一个广播或者电视频道数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t *STBDATA_GetNextTVOrRadioChannelInfo(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetPrevTVChannelInfo
  Description     : 取得上一个电视频道数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t  *STBDATA_GetPrevTVChannelInfo(signed short    ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetPrevRadioChannelInfo
  Description     : 取得上一个广播频道数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t  *STBDATA_GetPrevRadioChannelInfo(signed short    ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetPrevTVOrRadioChannelInfo
  Description     : 取得上一个广播或者电视频道数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t *STBDATA_GetPrevTVOrRadioChannelInfo(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetEventInfoOfTime
  Description     : 取得给定时间的event 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_GetEventInfoOfTime(signed short ssServiceNO,StbTime_t StbTm,STBDATAEvent_t **pSTBEventInUse);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetNextEventInfoOfTime
  Description     : 取得给定时间后的下一个event 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL  STBDATA_GetNextEventInfoOfTime(signed short ssServiceNO,StbTime_t  StbTm,STBDATAEvent_t **pSTBEventInUse);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetEventInfoOfTimeFromPF
  Description     : 从PF 中取得给定时间的event 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_GetEventInfoOfTimeFromPF(signed short ssServiceNO,StbTime_t StbTm,STBDATAEvent_t **pSTBEventInUse);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetEventInfoOfTimeFromSchedule
  Description     : 从Schedule 中取得给定时间的event 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL  STBDATA_GetEventInfoOfTimeFromSchedule(signed short ssServiceNO,StbTime_t StbTm,STBDATAEvent_t **pSTBEventInUse);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetEventInfoOfId
  Description     : 根据ID 号取得event 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAEvent_t *STBDATA_GetEventInfoOfId(signed short ssServiceNO, unsigned short usEventid);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetPrevEventInfoOfTime
  Description     : 取得给定时间后的上一个event 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_GetPrevEventInfoOfTime(signed short ssServiceNO,StbTime_t StbTm,STBDATAEvent_t  **pSTBEventInUse);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTVChannelInfoOfEventInfo
  Description     : 根据EventInfo 取得TV Channel Info
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t *STBDATA_GetTVChannelInfoOfEventInfo(STBDATAEvent_t *STBEvent);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetAttributeOfTVChannelNo
  Description     : 获取当前电视频道属性
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_GetAttributeOfTVChannelNo(signed short ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetAttributeOfRadioChannelNo
  Description     : 获取当前广播频道属性
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_GetAttributeOfRadioChannelNo(signed short ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetAttributeOfTVorRadioChannelNo
  Description     : 获取当前电视或者广播频道属性
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_GetAttributeOfTVorRadioChannelNo(signed short ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetLockOfTVChannelNo
  Description     : 根据频道号获取电视节目是否加锁 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_GetLockOfTVChannelNo(signed short ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetLockOfRadioChannelNo
  Description     : 根据频道号获取广播节目是否加锁 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_GetLockOfRadioChannelNo(signed short ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetAttributeOfTVorRadioChannelNo
  Description     : 根据频道号获取电视节目或者广播节目是否加锁 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_GetLockOfTVorRadioChannelNo(signed short ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetChannelNoOfID
  Description     : 通过networkid, tsid, service id  获得对应的频道序号
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBDATA_GetChannelNoOfID(unsigned short usNetID, unsigned short usTsID, unsigned short usServiceId);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetBATInfoOfID
  Description     : 根据BAT ID获取BAT信息
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATABATInfo_t * STBDATA_GetBATInfoOfID(unsigned short usBATID);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_VerifyServiceContentOfBATNO
  Description     : 检查根据BAT号和ServiceNO获取的BAT信息是否存在
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_VerifyServiceContentOfBATNO(signed short ssBATNo, signed short ssServiceNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitMosaicInfoTemp
  Description     : 初始化Mosaic 数据(用于频道跟随功能中)
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitMosaicInfoTemp(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetFreeMosaicInfoTemp
  Description     : 得到空闲的Mosaic 数据(用于频道跟随功能中)
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATATempMosaicInfo_t * STBDATA_GetFreeMosaicInfoTemp(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetFreeMosaicInfoTemp
  Description     : 得到空闲的Mosaic 数据(用于频道跟随功能中)
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATATempMosaicInfo_t * STBDATA_GetFreeMosaicInfoTemp(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_MallocMosaicTmp
  Description     : 为Mosaic 数据分配内存(用于频道跟随功能中) 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_MallocMosaicTmp(DRVOS_Partition_t * pPtn);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_FreeMosaicTmp
  Description     : 释放Mosaic 数据的内存(用于频道跟随功能中)  
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_FreeMosaicTmp(DRVOS_Partition_t * pPtn);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetMosaicEntryChnl
  Description     : 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAMosaicInfo_t * STBDATA_GetMosaicEntryChnl(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetMosaicCellInfoOfLogicalCellID
  Description     : 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAMosaicLogicalCellInfo_t * STBDATA_GetMosaicCellInfoOfLogicalCellID(signed short ssMosaicChnlNo, unsigned char ucLogicalCellID);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetMosaicInfoOfChnlNo
  Description     : 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAMosaicInfo_t *STBDATA_GetMosaicInfoOfChnlNo(signed short ssMosaicChnlNo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetMosaicInfoOfServiceNo
  Description     : 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAMosaicInfo_t *STBDATA_GetMosaicInfoOfServiceNo(signed short ssServiceNo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetMosaicLogicalIDofElemID
  Description     : 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed char STBDATA_GetMosaicLogicalIDofElemID(signed short ssMosaicChnlNo, unsigned char ucElemID);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetMosaicLogicalIDofElemID
  Description     : 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBDATA_GetMosaicPFServiceNo(signed short ssMosaicChnlNo,unsigned int MsgCode);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_SetMosaicChnlInfo
  Description     : 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_SetMosaicChnlInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30 
  Author          :  xyan
  Function Name   : STBDATA_StorageChannelInfo
  Description     : 保存搜索前的频道信息
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_StorageChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :  xyan
  Function Name   : STBDATA_SetStorageChannelInfo
  Description     : 设置搜索前保存的频道信息
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_SetStorageChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30 
  Author          :  xyan
  Function Name   : STBDATA_FreeStorageChannelInfo
  Description     : 释放保存搜索前频道信息的内存
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_FreeStorageChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceAudioInfoOfAudioNO
  Description     : 根据AudioNO获取ServiceAudioInfo
  Input           :
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceAudioInfo_t *STBDATA_GetServiceAudioInfoOfAudioNO(signed short ssAudioNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetAudioInfoOfAudioPID
  Description     : 根据AudioPID获取AudioInfo
  Input           :
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBAudioInfo_t  *STBDATA_GetAudioInfoOfAudioPID(STBDATAServiceAudioInfo_t *pServiceAudioInfo,unsigned short usAudioPID);

/*****************************************************************************
  Date&Time    	  : 2004-03-30 
  Author          : xyan
  Function Name   : STBDATA_FreeStorageChannelInfo
  Description     : 根据语言码获取语言索引
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_GetLanguageIndexOfCode(char *Code);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceSubtitleInfoOfSubtitleNO
  Description     : 根据SubtitleNO获取ServiceSubtitleInfo
  Input           :
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceSubtitleInfo_t *STBDATA_GetServiceSubtitleInfoOfSubtitleNO(signed short ssSubtitleNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetSubtitleInfoOfSubtitlePID
  Description     : 根据SubtitlePID获取SubtitleInfo
  Input           :
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBSubtitleInfo_t  *STBDATA_GetSubtitleInfoOfSubtitlePID(STBDATAServiceSubtitleInfo_t *pServiceSubtitleInfo,unsigned short usSubtitlePID,unsigned char *ucSubtitleNo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :    xyan
  Function Name   : STBDATA_GetServiceInformationOfID
  Description     : 根据network_id ts_id和service_id 取得serviceInformation数据
  Input           : network_id   ts_id  和 service_id
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATANVMServiceInformation_t *STBDATA_GetServiceInformationOfID(unsigned short  usNetID, unsigned short  usTsID, unsigned short  usServiceID);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :  xyan
  Function Name   : STBDATA_SetChannelStatus
  Description     : 设置频道的状态
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_SetChannelStatus(STBDATAChannelInfo_t *pSTBChannelInfo,unsigned char ucStatus);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :  xyan
  Function Name   : STBDATA_SetChannelAttribute
  Description     : 设置频道的喜爱属性
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_SetChannelAttribute(STBDATAChannelInfo_t *pSTBChannelInfo,unsigned char ucAttribute);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :  xyan
  Function Name   : STBDATA_SetChannelLocked
  Description     : 设置频道的加锁属性
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_SetChannelLocked(STBDATAChannelInfo_t *pSTBChannelInfo,unsigned char ucLocked);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :  xyan
  Function Name   : STBDATA_SetServiceSoundChannel
  Description     : 设置Service结构中的声道值
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_SetServiceSoundChannel(STBDATAServiceInfo_t *pSTBServiceInfo,unsigned char ucSoundChannel);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :  xyan
  Function Name   : STBDATA_SetServiceVolumeOffset
  Description     : 设置Service结构中的音量补偿值
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_SetServiceVolumeOffset(STBDATAServiceInfo_t *pSTBServiceInfo,signed char   scVolumeOffset);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   wrwei
  Function Name   : STBDATA_GetTVChannelListInString
  Description     : 获取电视频道信息，以字符中的形式返回所有频道的频道号和频道名称
  Input           : pChannelList保存数据的指针，siDataLength数据空间大小，GetStringLength获取数据的实际长度
  Outut           : 1成功;0失败
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : For Andriod use
*****************************************************************************/
signed int STBDATA_GetTVChannelListInString(unsigned char *pChannelList,signed int siDataLength,signed int *GetStringLength);

#endif


