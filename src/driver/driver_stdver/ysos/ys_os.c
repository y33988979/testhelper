/*******************************************************************************
  Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

  File name : osdrv.c

  Description :嵌入式操作系统的系统调用函数封装。此版本主要
  针对ST OS20 进行封装，同时兼顾其他嵌入式操作系统接口；

  Others: 

  History:     
    ======================================================================
    时    间：2004-03-16
    作    者：Cao Songtao
    说    明：
    1. 创建
    ======================================================================
    时    间：2006-02-07
    作    者：Cao Songtao
    说    明：
    1. 修改由于头文件包含造成的第三方移植重复定义问题
    2. 由于重新构建结构不便于后续版本维护故包含stlie.h文件

*******************************************************************************/

/*----------------------------------------------------------------------------
 *  Include
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 在此包含其他模块的头文件，自己的头文件放在函数定义之前
*----------------------------------------------------------------------------*/
#include "drv_os.h"

#include <string.h>//memcpy
/*----------------------------------------------------------------------------
 *  Debug
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 调试相关定义，可根据情况放在以下各区中，但必须使用调试开关，
* 调试开关可分多级，但必须由总控开关（各模块调试开关或项目调试开关）控制，
* 正常情况下调试输出可单独定义为一级，测试数据可定义为一级，
* 非正常返回处必须有调试输出
*----------------------------------------------------------------------------*/
/*由配置文件定义*/

/*----------------------------------------------------------------------------
 *  External Function Prototype
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 引用的外部函数原型
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *  External variable
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *  Struct
 *----------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------
 *  Private Constant & Enum & Macro
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 *  Global variable
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *  Private Module Variable
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 必须定义为static变量，防止外部引用
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *  Private Function Prototype
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 必须定义为static变量，防止外部引用
*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
* 在此包含自己的头文件，以确保头文件中所有类型都已声明
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 *  Function
 *----------------------------------------------------------------------------*/
#if 1/*common funtion*/

#endif


#if 1/*task function*/
void YS_OS_TaskLock(void)
{
  DRV_OS_TaskLock();
}

void YS_OS_TaskUnlock(void)
{
  DRV_OS_TaskUnlock();
}

#endif/*task function*/

#if 1/*message function*/
BOOL_YS YS_OS_GetMsg_Wait(DRVOS_MsgQueue_t *MsgQueue, OUT_YS void *Msg, U32_YS MsgSize)
{
  void *MsgBuffer;
  BOOL_YS IsSuccess = TRUE_YS;
  
  if ((NULL_YS != MsgQueue) && (NULL_YS != Msg))
  {
    MsgBuffer = DRV_OS_MsgReceive(MsgQueue, DRVOS_WAIT_INFINITY, 0);
    if(NULL_YS != MsgBuffer)
    {
      memcpy(Msg, MsgBuffer, MsgSize);
      DRV_OSMsgRelease(MsgQueue, MsgBuffer);
    }
    else
    {
      IsSuccess = FALSE_YS;
    }
  }
  else
  {
    IsSuccess = FALSE_YS;
  }
	
  return IsSuccess;
}

BOOL_YS YS_OS_GetMsg_Timeout(DRVOS_MsgQueue_t *MsgQueue, OUT_YS void *Msg, U32_YS MsgSize, 
                                         DRVOS_WaitMode_e WaitMode, DRVOS_TimeMS_t MSToWait)
{
  void *MsgBuffer;
  BOOL_YS IsSuccess = TRUE_YS;
  
  if ((NULL_YS != MsgQueue) && (NULL_YS != Msg))
  {
    switch (WaitMode)
    {
      case DRVOS_WAIT_NO:
        MsgBuffer = DRV_OS_MsgReceive_MS(MsgQueue, DRVOS_WAIT_NO, 0);
        break;
      case DRVOS_WAIT_FINITY:
        MsgBuffer = DRV_OS_MsgReceive_MS(MsgQueue, DRVOS_WAIT_FINITY, MSToWait);     
        break;
      case DRVOS_WAIT_INFINITY:
        MsgBuffer = DRV_OS_MsgReceive_MS(MsgQueue, DRVOS_WAIT_INFINITY, 0);
        break;
      default:
        MsgBuffer = NULL_YS;
        break;
    }
	 
    if (NULL_YS != MsgBuffer)
    {
      memcpy(Msg, MsgBuffer, MsgSize);
      DRV_OSMsgRelease(MsgQueue, MsgBuffer);
    }
    else
    {
      IsSuccess = FALSE_YS;
    }
  }
  else
  {
    IsSuccess = FALSE_YS;
  }
	
  return IsSuccess;
}

BOOL_YS YS_OS_SendMsg_Wait(DRVOS_MsgQueue_t *MsgQueue, void *Msg, U32_YS MsgSize)
{
  void *MsgBuffer;
  BOOL_YS IsSuccess = TRUE_YS;
  
  if ((NULL_YS != MsgQueue) && (NULL_YS != Msg))
  {
    MsgBuffer = DRV_OS_MsgClaim(MsgQueue, DRVOS_WAIT_INFINITY, 0);
    if (NULL_YS != MsgBuffer)
    {
      memcpy(MsgBuffer, Msg, MsgSize);
      DRV_OSMsgSend(MsgQueue, MsgBuffer);
    }
    else
    {
      IsSuccess = FALSE_YS;
    }
  }
  else
  {
    IsSuccess = FALSE_YS;
  }
	
  return IsSuccess;
}

BOOL_YS YS_OS_SendMsg_Timeout(DRVOS_MsgQueue_t *MsgQueue, void *Msg, U32_YS MsgSize, 
                                           DRVOS_WaitMode_e WaitMode, DRVOS_TimeMS_t MSToWait)
{
  void *MsgBuffer;
  BOOL_YS IsSuccess = TRUE_YS;
  
  if ((NULL_YS != MsgQueue) && (NULL_YS != Msg))
  {
    switch (WaitMode)
    {
      case DRVOS_WAIT_NO:
        MsgBuffer = DRV_OS_MsgClaim_MS(MsgQueue, DRVOS_WAIT_NO, 0);
        break;
      case DRVOS_WAIT_FINITY:
        MsgBuffer = DRV_OS_MsgClaim_MS(MsgQueue, DRVOS_WAIT_FINITY, MSToWait);     
        break;
      case DRVOS_WAIT_INFINITY:
        MsgBuffer = DRV_OS_MsgClaim_MS(MsgQueue, DRVOS_WAIT_INFINITY, 0);
        break;
      default:
        MsgBuffer = NULL_YS;
        break;
    }
	 
    if (NULL_YS != MsgBuffer)
    {
      memcpy(MsgBuffer, Msg, MsgSize);
      DRV_OSMsgSend(MsgQueue, MsgBuffer);
    }
    else
    {
      IsSuccess = FALSE_YS;
    }
  }
  else
  {
    IsSuccess = FALSE_YS;
  }
	
  return IsSuccess;
}

#endif

#if 1/*interrupt function*/
void YS_OS_InterruptLock(void)
{
  DRV_OS_InterruptLock();
}

void YS_OS_InterruptUnlock(void)
{
  DRV_OS_InterruptUnlock();
}

#endif/*interrupt function*/




