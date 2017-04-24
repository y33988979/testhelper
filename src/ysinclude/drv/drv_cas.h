/*****************************************************************************
*
* Description: 
*    驱动层CAS模块头文件
*
* Revision History:
*    Date          Author         Modification
* ---------------------------------------------------------------------
* 1. 2005-10-17    Qiujordan      创建
*
*****************************************************************************/
#ifndef _DRV_CAS_H_
#define _DRV_CAS_H_

#include "drv_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum DRV_CasScStatus_e
{
    DRV_CAS_SMARTCARD_IN,
    DRV_CAS_SMARTCARD_OUT,
    DRV_CAS_SMARTCARD_ERROR
} DRV_CasScStatus;

/* T0 specific status parameters */
typedef struct DRV_SMART_T0Status_s
{
    U8_YS      Ins;
    U8_YS      Size;
    U8_YS      PB[2];
} DRV_SMART_T0Status_t;

/* T1 specific status parameters */
typedef struct DRV_SMART_T1Status_s
{
    U8_YS      ReservedForFutureUse;
} DRV_SMART_T1Status_t;

/* Smartcard status parameters */
typedef struct DRV_SMART_Status_s
{
    DRV_ErrCode          ErrorCode;
    S32_YS      Protocol;
    union
    {
        DRV_SMART_T0Status_t  T0;
        DRV_SMART_T1Status_t  T1;
    } StatusBlock;
} DRV_SMART_Status_t;

typedef enum STSMART_Reset_e
{
    SMART_COLD_RESET=0,
    SMART_WARM_RESET
} DRV_SMART_Reset_t;

typedef void (*SC_StatusCallbackFun)(DRV_CasScStatus status);


typedef enum
{
    DRV_CAS_NORMAL,                        
    DRV_CAS_CTI225,                       
    DRV_CAS_CTI300,                 
    DRV_CAS_NAGRA,
    DRV_CAS_TF21PRO,
    DRV_CAS_TF21PROEX,
    DRV_CAS_IRDETO,
    DRV_CAS_DVN
} DRV_SMART_CA_Type_t;

enum
{
    DRV_CAS_ERROR_HANDLE = DRV_CAS_BASE + 1,
    DRV_CAS_ERROR_SMART_TRANSFER,
    DRV_CAS_ERROR_SMART_NOT_INSERTED,
    DRV_CAS_ERROR_SMART_BUSY,
    DRV_CAS_ERROR_SMART_RESET
};


/*****************************************************************************
* Function    : DRV_CasInit
* Description :
*                   初始化智能卡模块.
* Note        :
******************************************************************************/
DRV_ErrCode DRV_CasInit(DRV_SMART_CA_Type_t ca_type);

/*****************************************************************************
* Function    : DRV_CasEnableCallback
* Description :
*                  注册智能卡状态回调函数
* Note        :
******************************************************************************/
DRV_ErrCode DRV_CasEnableCallback(SC_StatusCallbackFun Callback_f);

/*****************************************************************************
* Function    : DRV_CasDisableCallback
* Description :
*                  注消智能卡状态回调函数
* Note        :
******************************************************************************/
DRV_ErrCode DRV_CasDisableCallback(void);

/*****************************************************************************
* Function    : DRV_CasOpen
* Description :
*                   打开智能卡设备，准备工作
* Note        :
******************************************************************************/
DRV_ErrCode DRV_CasOpen(void);

/*****************************************************************************
* Function    : DRV_CasGetScStatus
* Description :
*                   获取智能卡是否插入、拔出的状态
* Note        :
******************************************************************************/
DRV_ErrCode DRV_CasGetScStatus(DRV_CasScStatus *Status_p);

/*****************************************************************************
* Function    : DRV_CasResetSc
* Description :
*                   重新复位智能卡
* Note        :
******************************************************************************/
DRV_ErrCode DRV_CasResetSc(U8_YS *Answer_p, U8_YS* Answer_Len, U8_YS ResetType);

/*****************************************************************************
* Function    : DRV_CasScSend
* Description :
*			给智能卡发送数据，有超时设置
* Note        :
******************************************************************************/
DRV_ErrCode DRV_CasScSend(const U8_YS *Data_p,
                              U32_YS  NumberToSend,
                              U32_YS *NumberSent_p,
                              U32_YS  Timeout);

/*****************************************************************************
* Function    : DRV_CasScReceive
* Description :
*			接收智能卡数据，有超时设置			
* Note        :
******************************************************************************/
DRV_ErrCode DRV_CasScReceive(U8_YS  *Data_p,
                                 U32_YS  NumberToReceive,
                                 U32_YS *NumberReceived_p,
                                 U32_YS  Timeout);


/*****************************************************************************
* Function    : DRV_CasScTransfer
* Description :
*			单一接口中实现智能卡读写
* Note        :
******************************************************************************/
DRV_ErrCode DRV_CasScTransfer(U8_YS  *Command_p,
                                 U32_YS  NumberToWrite,
                                 U32_YS *NumberToWrite_p,
                                 U8_YS   *Response_p,
                                 U32_YS  NumberToRead,
                                 U32_YS *NumberToRead_p,                                 
                                 DRV_SMART_Status_t  *Status_p);


/*****************************************************************************
* Function    : DRV_CasScTerm
* Description :
*                   停止智能卡工作
* Note        :
******************************************************************************/
DRV_ErrCode DRV_CasScTerm(void);

/*****************************************************************************
* Function    : DRV_CasScClose
* Description :
*                   关闭智能卡模块
* Note        :
******************************************************************************/
DRV_ErrCode DRV_CasScClose(void);


#ifdef __cplusplus
}
#endif

#endif

