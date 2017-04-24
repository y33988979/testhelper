/*******************************************************************************
  Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

  File name : osdrv.c

  Description :Ƕ��ʽ����ϵͳ��ϵͳ���ú�����װ���˰汾��Ҫ
  ���ST OS20 ���з�װ��ͬʱ�������Ƕ��ʽ����ϵͳ�ӿڣ�

  Others: 

  History:     
    ======================================================================
    ʱ    �䣺2004-03-16
    ��    �ߣ�Cao Songtao
    ˵    ����
    1. ����
    ======================================================================
    ʱ    �䣺2006-02-07
    ��    �ߣ�Cao Songtao
    ˵    ����
    1. �޸�����ͷ�ļ�������ɵĵ�������ֲ�ظ���������
    2. �������¹����ṹ�����ں����汾ά���ʰ���stlie.h�ļ�

*******************************************************************************/

/*----------------------------------------------------------------------------
 *  Include
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* �ڴ˰�������ģ���ͷ�ļ����Լ���ͷ�ļ����ں�������֮ǰ
*----------------------------------------------------------------------------*/
#include "drv_os.h"

#include <string.h>//memcpy
/*----------------------------------------------------------------------------
 *  Debug
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* ������ض��壬�ɸ�������������¸����У�������ʹ�õ��Կ��أ�
* ���Կ��ؿɷֶ༶�����������ܿؿ��أ���ģ����Կ��ػ���Ŀ���Կ��أ����ƣ�
* ��������µ�������ɵ�������Ϊһ�����������ݿɶ���Ϊһ����
* ���������ش������е������
*----------------------------------------------------------------------------*/
/*�������ļ�����*/

/*----------------------------------------------------------------------------
 *  External Function Prototype
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* ���õ��ⲿ����ԭ��
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
* ���붨��Ϊstatic��������ֹ�ⲿ����
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *  Private Function Prototype
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* ���붨��Ϊstatic��������ֹ�ⲿ����
*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
* �ڴ˰����Լ���ͷ�ļ�����ȷ��ͷ�ļ����������Ͷ�������
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




