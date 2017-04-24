/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : ys_os.h

    Description :嵌入式操作系统的系统调用函数封装

    Others:      

    History:     
    ======================================================================
    时    间：2004-03-16
    作    者：Cao Songtao
    说    明：1. 创建
    ======================================================================
    时    间：2006-02-07
    作    者：Cao Songtao
    说    明：
    1. 修改由于头文件包含造成的第三方移植重复定义问题
    2. 由于重新构建结构不便于后续版本维护故包含stlie.h文件

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
/*基本类型定义*/
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




