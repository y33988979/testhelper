/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : stbdatasearch.h

    Description : STB搜索管理

    Others:      

    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef _YSSTBDATASEARCH_
#define _YSSTBDATASEARCH_
/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "driver.h"
#include "midware.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Global Function Prototype
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSet_GetCasVerifyStatus
  Description     : 获取节目授权检查状态
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBSet_GetCasVerifyStatus(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSet_SetCasVerifyStatus
  Description     : 设置节目授权检查状态
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetCasVerifyStatus(BOOL Staus);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSet_GetCasVerifyResult
  Description     : 获取节目授权检查结果
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBSet_GetCasVerifyResult(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSet_SetCasVerifyResult
  Description     : 设置节目授权检查结果
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetCasVerifyResult(BOOL Staus);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSet_CheckCasAuthority
  Description     : 检查节目是否授权
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_CheckCasAuthority(unsigned char ucMsgType);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSearch_SetAppMsgQueue
  Description     : 设置应用的消息队列
  Input           : pMsgApp 消息队列
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 用于将midware的搜索任务的状态传递给搜索应用
*****************************************************************************/
void STBSearch_SetAppMsgQueue( DRVOS_MsgQueue_t* pMsgApp);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSearch_SendFinishCahnnelSearchMsgToSearchTask
  Description     : 发送中断频道搜索消息到midware的搜索任务
  Input           : 
  Outut           : true发送成功，false发送失败 
  Calls           : 搜索应用
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void STBSearch_SendFinishCahnnelSearchMsgToSearchTask(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSearch_SendChannelSearchFinishedMsgToApp
  Description     : 发送频道搜索结束消息到应用
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void STBSearch_SendChannelSearchFinishedMsgToApp(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSet_SendChannelSearchTSChangedMsgToApp
  Description     : 发送频道搜索TS切换消息到应用
  Input           : uiTsNO TS号，uiChnlSearchMode 搜索模式
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SendChannelSearchTSChangedMsgToApp(unsigned int uiTsNO, unsigned int uiChnlSearchMode);

/*****************************************************************************
  Date&Time       : 2009-04-24  
  Author          :   xyan 
  Function Name   : STBSet_SetSTBJZDBCInfo
  Description     : 储存数据广播索引信息
  Input           : pTS 搜索到的TS数据结构
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 如需aut支持avit，需另作处理。可参考成都5516版本06-10-18的记录
*****************************************************************************/ 
void STBSet_SetSTBJZDBCInfo(void);

/*****************************************************************************
  Date&Time       : 2009-04-24 
  Author          :   xyan 
  Function Name   : STBSet_SetSTBBATInfo
  Description     : 储存BAT 分类表索引信息
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetSTBBATInfo(DvbSI_t *pDvbSi);

/*****************************************************************************
  Date&Time    	  : 2010-06-01
  Author          :   xyan 
  Function Name   : STBSet_SetSTBChannelInfoForUpdate
  Description     : 按照一定的顺序对频道进行排序(提供给后台更新添加删除频道使用)
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetSTBChannelInfoForUpdate(void);

/*****************************************************************************
  Date&Time       : 2009-04-24 
  Author          :   xyan
  Function Name   : STBSet_StartChannelSearching
  Description     : 开始频道搜索
  Input           : 
  Outut           : true 搜索成功，false搜索失败
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBSet_StartChannelSearching(void);

/*****************************************************************************
  Date&Time       : 2009-04-24 
  Author          :   xyan
  Function Name   : STBSet_SetSeachingResult
  Description     : 保存搜索结果
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetSeachingResult(void);

/*****************************************************************************
  Date&Time       : 2009-04-24 
  Author          :   xyan
  Function Name   : STBSet_CancelSeachingResult
  Description     : 放弃搜索到的数据，恢复以前的数据
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_CancelSeachingResult(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSet_SetChannelSearchedNUM
  Description     : 设置搜索到电视节目、广播节目、NVOD、数据广播节目的数目
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetChannelSearchedNUM(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSet_SetChannelSearchedNUMForSubSectionSearch
  Description     : 设置手动分段搜索到电视节目、广播节目、NVOD、数据广播节目的数目
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetChannelSearchedNUMForSubSectionSearch(signed short ssTsNO);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSet_GetTVChannelSearchedNUM
  Description     : 获取搜索到的电视节目数
  Input           : 
  Outut           : 电视节目数
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBSet_GetTVChannelSearchedNUM(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSet_GetRadioChannelSearchedNUM
  Description     : 获取搜索到的广播节目数
  Input           :
  Outut           : 广播节目数
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBSet_GetRadioChannelSearchedNUM(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSet_GetNVODChannelSearchedNUM
  Description     : 获取搜索到的NVOD节目数
  Input           :
  Outut           : NVOD节目数
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBSet_GetNVODChannelSearchedNUM(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSet_GetDBCChannelSearchedNUM
  Description     : 获取搜索到的数据广播节目数  
  Input           :
  Outut           : 数据广播节目数
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBSet_GetDBCChannelSearchedNUM(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSet_AllMemoryPartitionPrint
  Description     : 测试所有内存碎片函数
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_AllMemoryPartitionPrint(void);

#endif

