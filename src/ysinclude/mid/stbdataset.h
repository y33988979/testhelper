/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : stbdataset.h

    Description : STB数据设置管理

    Others:      

    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef _YSSTBDATASET_
#define _YSSTBDATASET_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "driver.h"
#include "midware.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#define STB_SET_CHANNEL_SEARCHING_TASK_STACK_SIZE 		(USER_WS_SIZE*16)
#define STB_SET_CHANNEL_SEARCHING_TASK_PARTITION_SIZE 	(NORMAL_PART_SIZE*16)

/*搜索任务User消息MsgCode定义*/
#define STBSET_STOP_CHANNEL_SEARCHING   				CHSRCH_USER_BASE 
#define STB_SET_CHANNEL_SEARCHING_TS_CHANGED 			(CHSRCH_USER_BASE+0x01)
#define STB_SET_CHANNEL_SEARCHING_FINISHED 				(CHSRCH_USER_BASE+0x02)
#define STB_SET_CHANNEL_SEARCHING_AUTO 					(CHSRCH_USER_BASE+0x03)
#define STB_SET_CHANNEL_SEARCHING_HANDWORK 				(CHSRCH_USER_BASE+0x04)
#define STB_SET_CHANNEL_SEARCHING_FULL_FRE_TEST 		(CHSRCH_USER_BASE+0x05)
#define STB_SET_CHANNEL_SEARCHING_FULL_FRE_SUCCESS 		(CHSRCH_USER_BASE+0x06)
#define STB_SET_CHANNEL_SEARCHING_FULL_FRE_SUCCESS_FAKE (CHSRCH_USER_BASE+0x07)

/*透明度默认定义*/
#define STBSET_MIN_COLOR_TRANSPARENCE					37
#define STBSET_MAX_COLOR_TRANSPARENCE 					63
#define STBSET_DEFAULT_COLOR_TRANSPARENCE 				63

/*亮度、色度和对比度默认定义*/
#define STBSET_DEFAULT_BRIGHT							5
#define STBSET_DEFAULT_CONTRAST							5
#define STBSET_DEFAULT_SATURATION						5
#define STBSET_MAX_BRIGHT								10
#define STBSET_MAX_CONTRAST								10
#define STBSET_MAX_SATURATION							10

/*智能卡默认卡值*/
#define DEFAULT_CA_BATID 								0x101

/*生产模式标识值*/
#define FACTORY_MODE_FLAG								0xabccddef   

/*自动开关机标识值*/
#define STB_POWEROFF_REPEAT_MODE						0xab
#define STB_POWEROFF_FLAG								0xa3
#define STB_POWERON_FLAG								0xb3

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
typedef enum ChsrchMode
{
	SEARCHING_CHANNEL_AUTO,
	SEARCHING_CHANNEL_FULL_FREQUENCY,
	SEARCHING_CHANNEL_HANDWORK	
}ChsrchMode_e;

/*界面语言类型(与LanguageCode_e对应)*/
typedef enum LanguageType
{
	CHINESE = 0 ,
	ENGLISH = 1 ,
	LANGUAGE_TYPE_COUNT
}LanguageType_e;

typedef struct STBSYSInfo
{
	char scHardwareInfo[15];
	char scSoftInfo[15];
	char scNetworkSoftInfo[15];
}STBSYSInfo_t;

typedef struct STBSoundSet
{
	signed char scTVSoundChannel;
	signed char scRadioSoundChannel;
}STBSoundSet_t;

typedef struct STBDisplaySet
{
	signed short ssBackgroundTransparence;
	signed char scDisplayProportion;
}STBDisplaySet_t;

/*!!!注意，此数据结构修改需与bios对应，并注意版本问题!!!*/
typedef struct 
{
	unsigned int 	CfgMark;	/* 0xbeebebee*/

	/*升级数据缺省流设置*/	
	unsigned int 	FreUpgrade;	
	unsigned short	SymUpgrade;
	unsigned short	QamUpgrade;
	unsigned short	UMT_pid;
	unsigned short	UMT_tableid;
	
	/* 电视节目默认声道*/			
	unsigned char 	TVSoundChannel ;
	/*广播节目默认声道*/
	unsigned char 	RadioSoundChannel ;
	/*显示比例*/
	unsigned char 	DisplayProportion ;
	/*背景透明度*/
	unsigned char 	BackgroundTransparence ;
	
	/*搜索缺省流设置*/
	unsigned int	FreAuto;	
	unsigned short	SymAuto;
	unsigned short	QamAuto;
	/* 手动*/
	unsigned int	FreHand;	
	unsigned short	SymHand;
	unsigned short	QamHand;
	/*全频*/
	unsigned short	SymFull;
	unsigned short	QamFull;

	/*搜索模式*/
	unsigned char 	ChannelSearchMode ;
	/*升级提示标志*/
	unsigned char 	UpgradeTag;
	/*网上软件版本号*/
	unsigned short 	NetworkSoftWareVersion;
	
	/*应用数据版本号*/
	unsigned char 	ucApplicationDataVersion;
	
	unsigned int 	Reserved[4];
}APP_Config_t;


/*----------------------------------------------------------------------------
 *	Global Function Prototype
 *----------------------------------------------------------------------------*/

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_ShowBootPicture
  Description	  : 开机显示开机画面
  Input 		  : 
  Outut 		  :  
  Calls 		  : 
  Called by 	  :
  Global Variables: 
  Others		  : 
*****************************************************************************/ 
void STBSet_ShowBootPicture(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_ReceiveDataForBoot
  Description	  : 开机接收九州升级信息和NIT信息
  Input 		  : 
  Outut 		  :  
  Calls 		  : 
  Called by 	  :
  Global Variables: 
  Others		  : 
*****************************************************************************/ 
void STBSet_ReceiveDataForBoot(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_LoadCABATID
  Description	  : 从智能卡中获取授权业务群的id 值
  Input 		  : 
  Outut 		  :  
  Calls 		  : 
  Called by 	  :
  Global Variables: 
  Others		  : 
*****************************************************************************/ 
BOOL STBSet_LoadCABATID(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_IsChannelLocked
  Description     : 频道锁定功能判断函数
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBSet_IsChannelLocked(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_UnlockChannel
  Description     : 关闭频道锁定功能
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_UnlockChannel(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_UnlockChannel
  Description     : 打开频道锁定功能
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_LockChannel(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_InitAppSettingData
  Description     : 初始化应用设置数据
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_InitAppSettingData( void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_LoadAppSettingData
  Description     : 从E2PROM中读取应用设置数据
  Input           : 
  Outut           : true读取成功，false读取失败
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 	
BOOL STBSet_LoadAppSettingData(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_WriteAppSettingData
  Description     : 将应用设置数据写入E2PROM
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_WriteAppSettingData(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetCABATID
  Description	  : 获取智能卡中的授权业务群id 值  
  Input 		  : 
  Outut 		  :  
  Calls 		  : 
  Called by 	  :
  Global Variables: 
  Others		  : 
*****************************************************************************/ 
unsigned int STBSet_GetCABATID(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetSYSInfo
  Description     : 获得硬件、软件及网上软件的信息
  Input           : pSTBSYSInfo系统信息结构指针
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_GetSYSInfo(STBSYSInfo_t *pSTBSYSInfo);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetApplicationDataVersion
  Description     : 获得频道跟随表数据版本号
  Input           : 
  Outut           : 版本号
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBSet_GetApplicationDataVersion(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetApplicationDataVersion
  Description     : 设置应用数据版本号
  Input           : ucVersion 版本号
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetApplicationDataVersion(unsigned char ucVersion);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetTVDefaultSoundChannel
  Description     : 获得默认电视节目声道
  Input           : 
  Outut           : 声道
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int STBSet_GetTVDefaultSoundChannel(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetTVDefaultSoundChannel
  Description     : 设置默认电视节目声道
  Input           : siTVSoundChannel声道值
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetTVDefaultSoundChannel(int siTVSoundChannel);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetRadioDefaultSoundChannel
  Description     : 获得默认广播节目声道
  Input           : 
  Outut           :  声道
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int STBSet_GetRadioDefaultSoundChannel(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetRadioDefaultSoundChannel
  Description     : 设置默认广播节目声道
  Input           : siRadioSoundChannel声道值
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetRadioDefaultSoundChannel(int siRadioSoundChannel);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetDisplayProportion
  Description     : 获得显示比例
  Input           : 
  Outut           : 显示比例
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int STBSet_GetDisplayProportion( void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetDisplayProportion
  Description     : 设置显示比例
  Input           : siDisplayProportion 显示比例
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetDisplayProportion(int siDisplayProportion);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetVideoMode
  Description     : 获取音视频制式模式
  Input           : 
  Outut           : g_VideoMode
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int STBSet_GetVideoMode( void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetVideoMode
  Description     : 设置音视频制式模式
  Input           : 制式模式siMode
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetVideoMode( int siMode);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSET_ResetBackgroundTransparence
  Description     : 设置调色板颜色透明度
  Input           : siBackgroundTransparence 透明度
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSET_ResetBackgroundTransparence(int siBackgroundTransparence);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetBackgroundTransparence
  Description     : 获得背景透明度
  Input           : 
  Outut           : 透明度
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int STBSet_GetBackgroundTransparence(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetBackgroundTransparence
  Description     : 设置背景透明度
  Input           : siBackgroundTransparence 背景透明度
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetBackgroundTransparence(int siBackgroundTransparence);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetChannelSearchMode
  Description     : 获得频道搜索模式
  Input           : 
  Outut           : 搜索模式
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int STBSet_GetChannelSearchMode(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetChannelSearchMode
  Description     : 设置频道搜索模式
  Input           : siChannelSearchMode搜索模式
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetChannelSearchMode(int siChannelSearchMode);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetDelivery
  Description     : 获得默认频率值
  Input           : scChannelSearchMode搜索模式
  Outut           : 频点结构指针
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATADeliveryInfo_t  *STBSet_GetDelivery(unsigned char scChannelSearchMode);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetDelivery
  Description     : 设置默认频率值
  Input           : DeliveryOfSet 频点数据,scChannelSearchMode 搜索模式
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetDelivery(STBDATADeliveryInfo_t DeliveryOfSet,unsigned char scChannelSearchMode);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetUpgradeDelivery
  Description     : 获得默认升级频率值
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATADeliveryInfo_t  *STBSet_GetUpgradeDelivery(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetUpgradeDelivery
  Description     : 设置升级频率值
  Input           : DeliveryOfSet 频点值
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetUpgradeDelivery(STBDATADeliveryInfo_t DeliveryOfSet);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetUMTPID
  Description     : 获取UMT表的PID
  Input           :
  Outut           : PID
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned short STBSet_GetUMTPID(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetUMTPID
  Description     : 设置UMT表的PID
  Input           : usUMTPID UMT表的PID
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetUMTPID(unsigned short usUMTPID);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetUMTTableID
  Description     : 获取UMT表的TABLEID
  Input           : 
  Outut           : TABLEID
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned short STBSet_GetUMTTableID(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetUMTTableID
  Description     : 设置UMT表的TABLEID
  Input           : usUMTTableID UMT表的TABLEID
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetUMTTableID(unsigned short usUMTTableID);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetLanguageSetting
  Description     : 获取语言设置
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBSet_GetLanguageSetting(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetLanguageSetting
  Description     : 设置语言
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetLanguageSetting(unsigned char ucLanguageSetting);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_ReadLanguageSetting
  Description     : 从FLASH中读出语言设置
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_ReadLanguageSetting( void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetNetworkSoftWareVersion
  Description     : 获取网上软件版本号
  Input           : 
  Outut           : 版本号
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned short STBSet_GetNetworkSoftWareVersion(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetNetworkSoftWareVersion
  Description     : 设置网上软件版本号
  Input           : usNetworkSoftWareVersion 版本号
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetNetworkSoftWareVersion(unsigned short usNetworkSoftWareVersion);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetUpgradeTag
  Description     : 获取升级标志
  Input           :
  Outut           : 升级标志
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBSet_GetUpgradeTag(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetUpgradeTag
  Description     : 设置升级标志
  Input           : ucUpgradeTag 升级标志
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetUpgradeTag(unsigned char ucUpgradeTag);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetCurrentTSNoCount
  Description     : 设置当前已经搜索的TS个数(为需要搜索进度的自动搜索和全频搜索使用)
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetCurrentTSNoCount(signed short siCount);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetCurrentTSNoCount
  Description     : 获取当前已经搜索的TS个数(为需要搜索进度的自动搜索和全频搜索使用)  	
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBSet_GetCurrentTSNoCount(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetTotalTSNoCount
  Description     : 设置要搜索的TS总数(为需要搜索进度的自动搜索和全频搜索使用)
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetTotalTSNoCount(signed short siCount);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetTotalTSNoCount
  Description     : 获取要搜索的TS总数(为需要搜索进度的自动搜索和全频搜索使用)  	
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBSet_GetTotalTSNoCount(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetManufactureModeVout
  Description     : 设置生产模式下的输出
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void STBSet_SetManufactureModeVout(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetManufactureModeIP
  Description     : 设置生产模式下的IP
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void STBSet_SetManufactureModeIP(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_UserModeComeback
  Description     : 将生产模式恢复到用户模式
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_UserModeComeback(void);

/*****************************************************************************
  Date&Time    :    2004-04-09
  Author          :   xyan
  Function Name   : STBSet_DefaultDataComeback
  Description     : 恢复默认数据
  Input           : siTure 是否恢复
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_DefaultDataComeback( int siTure);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMLoadUpgradeDeliverySetting
  Description     : 从应用设置数据缓冲中读出升级频点
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMLoadUpgradeDeliverySetting(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMWriteUpgradeDeliverySetting
  Description     : 将升级频点写入应用设置数据缓冲中
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMWriteUpgradeDeliverySetting(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMLoadUMTID
  Description     : 从应用设置数据缓冲中读出UMT的PID及TABLEID
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMLoadUMTID(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMWriteUMTID
  Description     : 将UMT的PID及TABLEID写入应用设置数据缓冲中
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMWriteUMTID(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMLoadNetworkSoftWareVersion
  Description     : 从应用设置数据缓冲中读出网上软件版本号
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMLoadNetworkSoftWareVersion(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMWriteNetworkSoftWareVersion
  Description     : 将网上软件版本号写入应用设置数据缓冲中
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMWriteNetworkSoftWareVersion(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMLoadUpgradeTag
  Description     : 从应用设置数据缓冲中读出升级标志
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMLoadUpgradeTag(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMWriteUpgradeTag
  Description     : 将升级标志写入应用设置数据缓冲中
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMWriteUpgradeTag(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMLoadDeliverySetting
  Description     : 从应用设置数据缓冲中读出默认频点设置
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMLoadDeliverySetting(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMWriteDeliverySetting
  Description     : 将默认频点设置写入应用设置数据缓冲中
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMWriteDeliverySetting(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMLoadChannelSearchModeSetting
  Description     : 从应用设置数据缓冲中读出搜索模式
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMLoadChannelSearchModeSetting(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMWriteChannelSearchModeSetting
  Description     : 将搜索模式写入应用设置数据缓冲中
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMWriteChannelSearchModeSetting(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMLoadDisplaySetting
  Description     : 从应用设置数据缓冲中读出显示设置
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMLoadDisplaySetting(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMWriteDisplaySetting
  Description     : 将显示设置写入应用设置数据缓冲中
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMWriteDisplaySetting(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMLoadSoundSetting
  Description     : 从应用设置数据缓冲中读出声音设置
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMLoadSoundSetting(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMWriteSoundSetting
  Description     : 将声音设置写入应用设置数据缓冲中
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMWriteSoundSetting(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_InsertTVChnl
  Description     : 将原频道插入目标频道
  Input           : ssTVChnlNOSource 源频道号ssTVChnlNODestination目标频道号
  Outut           : true 插入成功，false插入失败
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBSet_InsertTVChnl(signed short ssTVChnlNOSource,signed short ssTVChnlNODestination);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_TVChannelStatusChange
  Description     : 修改电视频道属性
  Input           : ssTVChannelNO 电视频道号 ucStatus 频道属性
  Outut           : true 修改成功，false修改失败
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBSet_TVChannelStatusChange( signed short ssTVChannelNO, unsigned char ucStatus);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_TVChannelAttributeChange
  Description     : 修改电视频道属性(喜好)
  Input           : ssTVChannelNO 电视频道号 ucStatus 频道属性
  Outut           : true 修改成功，false修改失败
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBSet_TVChannelAttributeChange( signed short ssTVChannelNO, unsigned char ucAttribute);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_TVChannelLockChange
  Description     : 修改电视频道锁定状态
  Input           : ssTVChannelNO 电视频道号 ucLocked 锁定状态
  Outut           : true 修改成功，false修改失败
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBSet_TVChannelLockChange( signed short ssTVChannelNO, unsigned char ucLocked);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_RadioChannelAttributeChange
  Description     : 修改广播频道属性(喜好)
  Input           : ssRadioChannelNO 广播频道号 ucStatus 频道属性
  Outut           : true 修改成功，false修改失败
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBSet_RadioChannelAttributeChange( signed short ssRadioChannelNO, unsigned char ucAttribute);


/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_InsertTVChnl
  Description     : 将原频道插入目标频道
  Input           : ssTVChnlNOSource 源频道号ssTVChnlNODestination目标频道号
  Outut           : true 插入成功，false插入失败
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBSet_InsertRadioChnl(signed short ssTVChnlNOSource,signed short ssTVChnlNODestination);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_RadioChannelStatusChange
  Description     : 修改电视频道属性
  Input           : ssTVChannelNO 电视频道号 ucStatus 频道属性
  Outut           : true 修改成功，false修改失败
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBSet_RadioChannelStatusChange( signed short ssTVChannelNO, unsigned char ucStatus);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_RadioChannelLockChange
  Description     : 修改广播频道锁定状态
  Input           : ssTVChannelNO 广播频道号 ucLocked 锁定状态
  Outut           : true 修改成功，false修改失败
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBSet_RadioChannelLockChange( signed short ssTVChannelNO, unsigned char ucLocked);

#endif
