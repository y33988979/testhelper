/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : ys_os.h

    Description :Ƕ��ʽ����ϵͳ��ϵͳ���ú�����װ

    Others:      

    History:     
    ======================================================================
    ʱ    �䣺2004-03-16
    ��    �ߣ�Cao Songtao
    ˵    ����1. ����
    ======================================================================
    ʱ    �䣺2006-02-07
    ��    �ߣ�Cao Songtao
    ˵    ����
    1. �޸�����ͷ�ļ�������ɵĵ�������ֲ�ظ���������
    2. �������¹����ṹ�����ں����汾ά���ʰ���stlie.h�ļ�

*******************************************************************************/

#ifndef _YS_OS_H_
#define _YS_OS_H_

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
/*�������Ͷ���*/
#include "ys_typedef.h"
#include "drv_os.h"
#include "ysoslist.h"
#include "ysostime.h"

/*----------------------------------------------------------------------------
 *	Enum & Macro
 *----------------------------------------------------------------------------*/
#ifndef OS_IN_RANGE
  #define OS_IN_RANGE(v,a,b)  (((v)>=(a)) && ((v)<=(b)))
#endif

#ifndef OS_MAX
#define OS_MAX(v,w) (((v)>(w))?(v):(w))
#endif

#ifndef OS_MIN
#define OS_MIN(v,w) (((v)>(w))?(w):(v))
#endif

#ifndef OS_LONGMAX
  #define OS_LONGMAX  2147483647   /*Maximum S32 value*/
#endif

#ifndef MidPause
  #define MidPause(x) DRV_OS_TaskDelay_MS(x)
#endif

/*----------------------------------------------------------------------------
 *  Struct
 *----------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------
 *  Function
 *----------------------------------------------------------------------------*/

#if 1/*task function*/
void YS_OS_TaskLock(void);

void YS_OS_TaskUnlock(void);
#endif/*task function*/

#if 1/*message function*/
BOOL_YS YS_OS_GetMsg_Wait(DRVOS_MsgQueue_t *MsgQueue, OUT_YS void *Msg, U32_YS MsgSize);

BOOL_YS YS_OS_GetMsg_Timeout(DRVOS_MsgQueue_t *MsgQueue, OUT_YS void *Msg, U32_YS MsgSize, 
                                                               DRVOS_WaitMode_e WaitMode, DRVOS_TimeMS_t MSToWait);

BOOL_YS YS_OS_SendMsg_Wait(DRVOS_MsgQueue_t *MsgQueue, void *Msg, U32_YS MsgSize);

BOOL_YS YS_OS_SendMsg_Timeout(DRVOS_MsgQueue_t *MsgQueue, void *Msg, U32_YS MsgSize, 
                                                               DRVOS_WaitMode_e WaitMode, DRVOS_TimeMS_t MSToWait);

#endif

#if 1/*interrupt function*/
void YS_OS_InterruptLock(void);

void YS_OS_InterruptUnlock(void);

#endif/*interrupt function*/

/* C++ support */
/* ----------- */
#ifdef __cplusplus
}
#endif
#endif




