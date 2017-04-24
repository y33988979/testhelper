/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : midware.h

    Description : 数据模块底层接口封装管理

    Others:      

    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef	__YSMHP_
#define	__YSMHP_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "stbtype.h" 
#include "dvbtablereceive.h"
#include "dvbreceiveeit.h"
#include "dvbtablemgr.h"
#include "dvbtableparse.h"
#include "dvbtabledata.h"

#include "stbdata.h"
#include "stbdatamgr.h"
#include "stbdatanvm.h"
#include "stbdataReceiveADT.h"
#include "stbdataReceiveADTForKonka.h"
#include "stbdataReceiveADTForSkyWorth.h"
#include "stbdataReceiveAUT.h"
#include "stbdataReceivePDT.h"
#include "stbdataReceivePIC.h"
#include "stbdataReceivePRG.h"
#include "stbdataReceiveXDT.h"
#include "stbdatasearch.h"
#include "stbdataset.h"
#include "stbdataupdate.h"
#include "stbdataupgrade.h"
#include "factcfg.h"

#include "app.h"
#include "booking.h"
#include "msg.h"
#include "guitk.h"
#include "dlg.h"
#include "xmlparser.h"
//#include "../testcmd/dbgtst/midwaredbg.h"
#include "appregion.h"


/*----------------------------------------------------------------------------
 *	Define(修改控制宏后,必须要重编Midware)
 *----------------------------------------------------------------------------*/

/***************开发平台控制宏选择(默认使用Linux开发平台)***************/

/*定义使用OS20的操作系统控制宏(默认不启用)*/
//#define USE_OS20_OPERATE_SYSTEM

/*定义使用OS21的操作系统控制宏(默认不启用)*/
//#define USE_OS21_OPERATE_SYSTEM

/*定义使用Linux操作系统控制宏(默认启用)*/
#define USE_LINUX_OPERATE_SYSTEM

/******************待机控制宏选择(默认使用主芯片真待机)******************/

/*定义使用假待机功能控制宏(默认不启用),需重编appys, midware*/
//#define USE_FAKE_STANDBY_MODE

/*定义前面板真待机模式(默认关闭), 仅在OS21上使用*/
//#define USE_PANEL_REAL_STANDBY_MODE

/***************************搜索功能控制宏选择***************************/

/*定义标准表数据接收是否采用指针置换模式的控制宏(默认启用)*/
#define GET_DATA_USE_POINTER_REPLACE

/*定义保存自动搜索前所有节目频道属性的控制宏(默认不启用)*/
//#define SAVE_CHANNEL_ATTRIBUTE_BEFORE_SEARCH

/*定义过滤未授权节目功能控制宏(默认不启用)*/
//#define USE_FILTER_NO_AUTHORIZED_PROGRAMME

/*定义使用保存扩展频道数据功能控制宏(默认启用,多伴音和多Subtitile及节目简介保存时使用)*/
#define USE_EXTEND_CHNLINFO_MODE

/*定义64K频道数据模式(默认不启用)*/
//#define USE_DATA_BLOCK_64K_MODE

/************************标准表处理功能控制宏选择************************/

/*定义使用Event扩展信息功能控制宏(默认不启用)*/
//#define USE_EXTENDED_EVENT_INFO

/*定义使用Epg内存缺乏模式(默认不启用,适用于前端播发Epg多且Epg内存不够的情况)*/
//#define USE_EPG_MEMORY_LOW_MODE

/*定义支持Epg多语言功能控制宏(默认不启用)*/
//#define SUPPORT_MULTY_LANGUAGE_EPG

/************************后台更新功能控制宏选择************************/

/*定义后台更新添加删除节目功能控制宏(默认不启用)*/
//#define UPDATE_ADD_AND_DELETE_PROG

/*定义使用区域升级功能控制宏(默认不启用)*/
//#define USE_ZONE_UPGRADE

/***************************应用功能控制宏选择***************************/

/*定义使用生产预设IP和S端子功能控制宏(默认不启用)*/
//#define USE_MANUFACTURE_DEFAULT_SET

/*定义S端子和分量互斥控制宏(默认不启用)*/
//#define YC_MUTEX_WITH_YUV

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
/*数据模块任务枚举*/
typedef enum Mid_Task_Id
{
	MID_CHSRCH_TASK=1,
	MID_UPDATE_TASK=2,
	MID_RECEIVE_STANDARD_TASK=3,
	MID_RECEIVE_EIT_TASK=4,
	MID_BOOT_TASK=5,
	MID_FILTER_TASK=6
}Mid_Task_Id_e;

typedef enum StreamType
{
	INVALID_STREAM_TYPE,
	VIDEO_STREAM_TYPE,
	AUDIO_STREAM_TYPE,
	PRIVATE_STREAM_TYPE,
	STREAM_TYPE_COUNT
}StreamType_e;

/*----------------------------------------------------------------------------
 *	External Function Prototype
 *----------------------------------------------------------------------------*/
extern int DRV_FILTReqFilterForStandard(DRVOS_MsgQueue_t *ReturnQueueId,FiltCallBack pCallbackFunc,sf_filter_mode_t	FilterMode,
										STPTI_Pid_t	ReqPid,int ReqTableId,int ReqProgNo,int	ReqSectionNo,int ReqVersionNo,
										int	nTimeOutCount,int iTsId,int iOriginalNetworkId);
extern BOOLEAN DRV_FILTFreeFilterReq(int SlotIndex);
extern int DRV_FILTReqFilterFree(FiltCallBack pCallbackFunc,sf_filter_mode_t FilterMode,unsigned char *paucFilterData,
								unsigned char *paucFilterMask,STPTI_Pid_t ReqPid,int nTimeOutCount);

/*----------------------------------------------------------------------------
 *	Global functions
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midmemset
  Description     : 数据模块封装底层memset函数,添加保护
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void midmemset(void *dst,int value,int len);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midmemcpy
  Description     : 数据模块封装底层memcpy函数,添加保护
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void midmemcpy(void *dst,void *src,int len);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midstrcpy
  Description     : 数据模块封装底层strcpy函数,添加保护
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void midstrcpy(char *dst,const char *src);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midstrncmp
  Description     : 数据模块封装底层strncmp函数,添加保护
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int midstrncmp(const char *dst,const char *src,int len);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midstrlen
  Description     : 数据模块封装底层strncmp函数,添加保护
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int midstrlen(const void *src);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : strlwr
  Description     : 将字符串大写字母转换成小写字母
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
char *strlwr(char *str);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_FindFileExtendName
  Description     : 从输入的文件全名中寻找扩展名
  Input           : 文件全名指针
  Outut           : 扩展名指针
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
char *Mid_FindFileExtendName(char *FileName);

/*****************************************************************************
  Date&Time    	  : 2012-11-15
  Author          :   wpeng
  Function Name   : Mid_CheckFileName
  Description     : 从输入的文件全名中检查非法字符
  Input           : 文件全名指针
  Outut           : 存在非法指针 返回FALSE 否则返回TRUE
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL Mid_CheckFileName(char *FileName);

/*****************************************************************************
  Date&Time       : 2012-03-29
  Author          :   xyan
  Function Name   : Mid_ConvertUnicodeToGBK
  Description     : 将Unicode转换为GBK
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL Mid_ConvertUnicodeToGBK(unsigned char *DestData,unsigned char *SrcData,int SrcDataLen);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_WriteDataInFlashBlock
  Description     : 数据模块封装底层写Block数据的接口
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL Mid_WriteDataInFlashBlock(unsigned char ucBlockNum,unsigned char *pData,unsigned int uiDataLength);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_ReadDataInFlashBlock
  Description     : 数据模块封装底层读Block数据的接口
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL Mid_ReadDataInFlashBlock(unsigned char ucBlockNum,unsigned char *pData,unsigned int uiDataLength);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_WritePicInFlashBlock
  Description     : 数据模块封装底层写Block图片数据的接口
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL Mid_WritePicInFlashBlock(unsigned char ucBlockNum,unsigned char *pData,unsigned int uiDataLength);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_ReadPicInFlashBlock
  Description     : 数据模块封装底层读Block图片数据的接口
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL Mid_ReadPicLengthInFlashBlock(unsigned char ucBlockNum,unsigned int *uiDataLength);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_ReadPicInFlashBlock
  Description     : 数据模块封装底层读Block图片数据的接口
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL Mid_ReadPicInFlashBlock(unsigned char ucBlockNum,unsigned char *pData,unsigned int uiDataLength);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_ShowSTBBootMpeg
  Description     : 显示机顶盒开机图片
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL Mid_ShowSTBBootMpeg(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_ShowSTBGBMpeg
  Description     : 显示机顶盒广播背景图片
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL Mid_ShowSTBGBMpeg(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_ReadFactCfg
  Description     : 数据模块封装底层读FactCfg数据的接口
  Input           : 
  Outut           : 返回FALSE: 未初始化; 返回TRUE:已经初始化
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL Mid_ReadFactCfg(unsigned char *pData);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_ReadAppCfg
  Description     : 数据模块封装底层读AppCfg数据的接口
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL Mid_ReadAppCfg(unsigned char *pData);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_WriteAppCfg
  Description     : 数据模块封装底层写AppCfg数据的接口
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void Mid_WriteAppCfg(unsigned char *pAppCfg);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_JudgeStreamType
  Description     : 根据传入StreamType判断流类型
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int Mid_JudgeStreamType(unsigned short usStreamType);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_GetSTBSerialNumber
  Description     : 获取外置机顶盒序列号
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char *Mid_GetSTBSerialNumber(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_SetSTBSerialNumber
  Description     : 设置外置机顶盒序列号
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Mid_SetSTBSerialNumber(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_GetMidVersion
  Description     : 获取数据模块版本号
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned int Mid_GetMidVersion(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_ShowMidVersion
  Description     : 串口显示数据模块版本号及编译时间
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Mid_ShowMidVersion(void);

#endif
