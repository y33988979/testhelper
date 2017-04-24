/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : dvbreceiveeit.h

    Description : EIT数据管理

    Others:      

    History:  1. llh creat 2004-3-29
			  2. xyan Modify 2011-11-14 

*******************************************************************************/
#ifndef _YSEITREC_
#define _YSEITREC_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "msg.h"
#include "stbdata.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Global Function Prototype
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_GetSectionInUseMaskNew
  Description     : 获取标准表接收模块的m_uiSectionInUseMaskNew值
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned int DVB_GetSectionInUseMaskNew(void);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_EPGDataUpdataWait
  Description     : 在对Service中的EventList操作前，用信号量阻塞。
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DVB_EPGDataUpdataWait(void);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_EPGDataUpdataSignal
  Description     : 在对Service中的EventList操作后，解除信号量阻塞。
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DVB_EPGDataUpdataSignal(void);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_GetReReceiveEITFlag
  Description     : 获取重新收取EPG控制标识
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DVB_GetReReceiveEITFlag(void);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_SetReReceiveEITFlag
  Description     : 设置重新收取EPG控制标识
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DVB_SetReReceiveEITFlag(BOOL Flag);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_ResetMemoryLackTag
  Description     : 将EPG内存缺乏标识置位(置为FALSE)
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DVB_ResetMemoryLackTag(void);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_GetMemoryLackTag
  Description     : 获取EPG内存缺乏标识
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DVB_GetMemoryLackTag(void);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_FreeEPGData
  Description     : 释放所有的EPG数据
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DVB_FreeEPGData(void);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_ReceiveEPGForChangeChannel
  Description     : DTV切台收EPG数据接口
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DVB_ReceiveEPGForChangeChannel(unsigned short usChnlNO);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_ReceiveEPGForNVOD
  Description     : NVOD应用中收EPG数据接口
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DVB_ReceiveEPGForNVOD(STBDATAServiceInfo_t *pServiceInfo);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DVB_ReceiveEPGForUpdate
  Description     : 重新收取EPG数据
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DVB_ReceiveEPGForUpdate(void);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_SendCommandToStandardTableReceiveTaskNew
  Description     : 应用给EPG接收任务发送Cmd
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DVB_SendCommandToStandardTableReceiveTaskNew(unsigned int nCmd);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_SendMsgToStandardTableReceiveTaskNew
  Description     : 应用给EPG接收任务发送消息(Msg)
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DVB_SendMsgToStandardTableReceiveTaskNew(MessageInfo_t *pMsg);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_RequestStandardTableReceiveNew
  Description     : 应用向EPG接收任务发出EPG数据接收申请
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DVB_RequestStandardTableReceiveNew(unsigned int uiReceiveTableState,signed int siPriority,void *pData,DRVOS_Partition_t *pPtn,DvbDelivery_t stDelivery,
										unsigned short usTsID,unsigned short usOriginalNetworkID,unsigned short usServiceID,DRVOS_MsgQueue_t *pstMsgQueue);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_ReceiveStandardTableTaskInit
  Description     : 接收EPG 数据任务初始化
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DVB_ReceiveEITTaskInit(void);

#endif
