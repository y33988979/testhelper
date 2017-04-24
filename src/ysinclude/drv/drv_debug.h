/*****************************************************************************
*
* Description: 
*    驱动层调试模块头文件
*
* Revision History:
*    Date          Author         Modification
* ---------------------------------------------------------------------
* 1. 2005-10-12    Qiujordan      创建
*
*****************************************************************************/
#ifndef _DRV_DEBUG_H_
#define _DRV_DEBUG_H_

#include "drv_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    severity_info   = 0,
    severity_warn  = 100,
    severity_error  = 200,
    severity_fatal = 300
} report_severity_t;


#ifndef DRV_DEBUG_INIT
#define DRV_DEBUG_INIT
#endif

#ifndef DRV_DEBUG_BASIC
#define DRV_DEBUG_BASIC
#endif

#ifndef DRV_DEBUG_OS
#define DRV_DEBUG_OS
#endif

#ifndef DRV_DEBUG_FILTER
#define DRV_DEBUG_FILTER
#endif

#ifndef DRV_DEBUG_AV
#define DRV_DEBUG_AV
#endif

#ifndef DRV_DEBUG_DISPLAY
#define DRV_DEBUG_DISPLAY
#endif

#ifndef DRV_DEBUG_TUNER
#define DRV_DEBUG_TUNER
#endif

#ifndef DRV_DEBUG_NVM
#define DRV_DEBUG_NVM
#endif

#ifndef DRV_DEBUG_OSD
#define DRV_DEBUG_OSD
#endif

#ifndef DRV_DEBUG_NET
#define DRV_DEBUG_NET
#endif

#ifndef DRV_DEBUG_VIDEO
#define DRV_DEBUG_VIDEO
#endif

#define INFO(Module,Msg)                                                \
    if(drv_DebugIsEnabelPrint(DRV_MODULE_ ## Module))                         \
    {                                                    \
        drv_print(severity_info,"[%s]:[%s] ",DRV_DebugGetModuleName(DRV_MODULE_ ## Module),__func__); \
        printf Msg;                              \
    }
#define ERROR(Module,Msg)                                               \
    if(1)                                                               \
    {                                                                   \
        drv_print(severity_error,"[%s]:[%12.12s@%04d] ",DRV_DebugGetModuleName(DRV_MODULE_ ## Module), __FILE__, __LINE__); \
        printf Msg;                                                     \
    }
#define WARN(Module,Msg)                                               \
    if(drv_DebugIsEnabelDebug(DRV_MODULE_##Module))                     \
    {                                                                   \
        drv_print(severity_warn,"[%s]:[%12.12s@%04d] ",DRV_DebugGetModuleName(DRV_MODULE_ ## Module), __FILE__, __LINE__); \
        printf Msg;                                                     \
    }


#ifdef DRV_DEBUG_INIT
#define INIT_INFO(Msg) INFO(INIT,Msg)
#define INIT_ERROR(Msg)ERROR(INIT,Msg)
#define INIT_WARN(Msg) WARN(INIT,Msg)
#else
#define INIT_INFO(Msg)  
#define INIT_ERROR(Msg)
#define INIT_WARN(Msg)    
#endif

#ifdef DRV_DEBUG_BASIC
#define BASIC_INFO(Msg) INFO(BASIC,Msg)
#define BASIC_ERROR(Msg)ERROR(BASIC,Msg)
#define BASIC_WARN(Msg) WARN(BASIC,Msg)
#else
#define BASIC_INFO(Msg)  
#define BASIC_ERROR(Msg)
#define BASIC_WARN(Msg)    
#endif

#ifdef DRV_DEBUG_OS
#define OS_INFO(Msg)   INFO(OS,Msg)
#define OS_ERROR(Msg)  ERROR(OS,Msg)
#define OS_WARN(Msg)   WARN(OS,Msg)
#else
#define OS_INFO(Msg)  
#define OS_ERROR(Msg)
#define OS_WARN(Msg)    
#endif

#ifdef DRV_DEBUG_FILTER
#define FILTER_INFO(Msg) INFO(FILTER,Msg)
#define FILTER_ERROR(Msg) ERROR(FILTER,Msg)
#define FILTER_WARN(Msg) WARN(FILTER,Msg)
#else
#define FILTER_INFO(Msg)  
#define FILTER_ERROR(Msg) 
#define FILTER_WARN(Msg)    
#endif

#ifdef DRV_DEBUG_NVM
#define NVM_INFO(Msg) INFO(NVM,Msg)
#define NVM_ERROR(Msg) ERROR(NVM,Msg)
#define NVM_WARN(Msg) WARN(NVM,Msg)
#else
#define NVM_INFO(Msg)  
#define NVM_ERROR(Msg)
#define NVM_WARN(Msg)    
#endif

#ifdef DRV_DEBUG_TUNER
#define TUNER_INFO(Msg) INFO(TUNER,Msg)
#define TUNER_ERROR(Msg) ERROR(TUNER,Msg)
#define TUNER_WARN(Msg) WARN(TUNER,Msg)
#else
#define TUNER_INFO(Msg)  
#define TUNER_ERROR(Msg)
#define TUNER_WARN(Msg)    
#endif

#ifdef DRV_DEBUG_DISPLAY
#define DISPLAY_INFO(Msg) INFO(DISPLAY,Msg)
#define DISPLAY_ERROR(Msg) ERROR(DISPLAY,Msg)
#define DISPLAY_WARN(Msg) WARN(DISPLAY,Msg)
#else
#define DISPLAY_INFO(Msg)  
#define DISPLAY_ERROR(Msg)
#define DISPLAY_WARN(Msg)    
#endif

#ifdef DRV_DEBUG_AUDIO
#define AUDIO_INFO(Msg) INFO(AUDIO,Msg)
#define AUDIO_ERROR(Msg) ERROR(AUDIO,Msg)
#define AUDIO_WARN(Msg) WARN(AUDIO,Msg)
#else
#define AUDIO_INFO(Msg)  
#define AUDIO_ERROR(Msg)
#define AUDIO_WARN(Msg)    
#endif

#ifdef DRV_DEBUG_VIDEO
#define VIDEO_INFO(Msg) INFO(VIDEO,Msg)
#define VIDEO_ERROR(Msg) ERROR(VIDEO,Msg)
#define VIDEO_WARN(Msg) WARN(VIDEO,Msg)
#else
#define VIDEO_INFO(Msg)  
#define VIDEO_ERROR(Msg)
#define VIDEO_WARN(Msg)    
#endif

#ifdef DRV_DEBUG_OSD
#define OSD_INFO(Msg) INFO(OSD,Msg)
#define OSD_ERROR(Msg) ERROR(OSD,Msg)
#define OSD_WARN(Msg) WARN(OSD,Msg)
#else
#define OSD_INFO(Msg)  
#define OSD_ERROR(Msg)
#define OSD_WARN(Msg)    
#endif

#ifdef DRV_DEBUG_NET
#define NET_INFO(Msg)  INFO(NET,Msg)
#define NET_ERROR(Msg) ERROR(NET,Msg)
#define NET_WARN(Msg)  WARN(NET,Msg)
#else
#define NET_INFO(Msg)  
#define NET_ERROR(Msg)
#define NET_WARN(Msg)    
#endif

#ifdef DRV_DEBUG_INJ
#define INJ_INFO(Msg) INFO(INIT,Msg)
#define INJ_ERROR(Msg)ERROR(INIT,Msg)
#define INJ_WARN(Msg) WARN(INIT,Msg)
#else
#define INJ_INFO(Msg)  
#define INJ_ERROR(Msg)
#define INJ_WARN(Msg)    
#endif

#ifdef DRV_DEBUG_PLAYER
#define PLAYER_INFO(Msg)  INFO(NET,Msg)
#define PLAYER_ERROR(Msg) ERROR(NET,Msg)
#define PLAYER_WARN(Msg)  WARN(NET,Msg)
#else
#define PLAYER_INFO(Msg)  
#define PLAYER_ERROR(Msg)
#define PLAYER_WARN(Msg)    
#endif

#define PLAYREC_INFO(Msg)  INFO(PLAYREC,Msg)
#define PLAYREC_ERROR(Msg) ERROR(PLAYREC,Msg)
#define PLAYREC_WARN(Msg)  WARN(PLAYRC,Msg)


#ifdef DRV_DEBUG_CAS
#define CAS_INFO(Msg)  INFO(CAS,Msg)
#define CAS_ERROR(Msg) ERROR(CAS,Msg)
#define CAS_WARN(Msg)  WARN(CAS,Msg)
#else
#define CAS_INFO(Msg)  
#define CAS_ERROR(Msg)
#define CAS_WARN(Msg)    
#endif


#define STAPI_ERROR(ErrCode)     do { drv_print(severity_fatal,"[STAPI ]:[%s]:[%12.12s@%04d]:[0x%X]%s\n",__func__,__FILE__, __LINE__,ErrCode,ST_GetErrorText(ErrCode));}while(0)

#define YSDRIVER_ERRORCODE(ErrCode)                                         \
    do                                                                  \
    {                                                                   \
        if(ErrCode == DRV_NO_ERROR)                                     \
        {                                                               \
            if(drv_DebugIsEnabelPrint(CURRENT_MODULE))                  \
            {                                                           \
                drv_print(severity_info,"[DRIVER]:[%s]%s\n",__func__,YSDRIVER_GetErrorText(ErrCode)); \
            }                                                           \
        }                                                               \
        else                                                            \
        {                                                               \
            if(1)/*drv_DebugIsEnabelDebug(CURRENT_MODULE))*/            \
            {                                                           \
                drv_print(severity_error,"[DRIVER]:[%s]:[%12.12s@%04d]:[0x%X:%s]\n",__func__,__FILE__, __LINE__,ErrCode,YSDRIVER_GetErrorText(ErrCode)); \
            }                                                           \
        }                                                               \
    }                                                                   \
    while(0)


#define YSDRIVER_ERRORCODE_M(ErrCode,format,args...)                    \
    do                                                                  \
    {                                                                   \
        if(ErrCode == DRV_NO_ERROR)                                     \
        {                                                               \
            if(drv_DebugIsEnabelDebug(CURRENT_MODULE))                  \
            {                                                           \
                drv_print(severity_info,"[DRIVER]:[%s]:[%12.12s@%04d]:[0x%X:%s][" format "]\n",__func__,__FILE__, __LINE__,ErrCode,YSDRIVER_GetErrorText(ErrCode),## args); \
            }                                                           \
        }                                                               \
        else                                                            \
        {                                                               \
            if(1)/*drv_DebugIsEnabelPrint(CURRENT_MODULE))*/            \
            {                                                           \
                drv_print(severity_error,"[DRIVER]:[%s]%s" format "\n",__func__,YSDRIVER_GetErrorText(ErrCode),## args); \
            }                                                           \
        }                                                               \
    }while(0)

#define YSDRIVER_ERRORCODE_RETURN(ErrCode)                              \
    do                                                                  \
    {                                                                   \
        if(ErrCode == DRV_NO_ERROR)                                     \
        {                                                               \
            if(drv_DebugIsEnabelPrint(CURRENT_MODULE))                  \
            {                                                           \
                drv_print(severity_info,"[DRIVER]:[%s]%s\n",__func__,YSDRIVER_GetErrorText(ErrCode)); \
            }                                                           \
        }                                                               \
        else                                                            \
        {                                                               \
            if(1)/*drv_DebugIsEnabelDebug(CURRENT_MODULE))*/            \
            {                                                           \
                drv_print(severity_error,"[DRIVER]:[%s]:[%12.12s@%04d]:[0x%X:%s]\n",__func__,__FILE__, __LINE__,ErrCode,YSDRIVER_GetErrorText(ErrCode)); \
            }                                                           \
        }                                                               \
        return ErrCode;                                                 \
    }                                                                   \
    while(0)

#define YSDRIVER_ERRORCODE_RETURN_M(ErrCode,format,args...)             \
    do                                                                  \
    {                                                                   \
        if(ErrCode != DRV_NO_ERROR)                                     \
        {                                                               \
            if(drv_DebugIsEnabelDebug(CURRENT_MODULE))                  \
            {                                                           \
                drv_print(severity_error,"[DRIVER]:[%s]:[%12.12s@%04d]:[0x%X:%s][" format "]\n",__func__,__FILE__, __LINE__,ErrCode,YSDRIVER_GetErrorText(ErrCode),## args); \
            }                                                           \
        }                                                               \
        else                                                            \
        {                                                               \
            if(drv_DebugIsEnabelPrint(CURRENT_MODULE))                  \
            {                                                           \
                drv_print(severity_info,"[DRIVER]:[%s]%s" format "\n",__func__,YSDRIVER_GetErrorText(ErrCode),## args); \
            }                                                           \
        }                                                               \
        return ErrCode;                                                 \
    }while(0)
    

#define YSDRIVER_ERROR_RETURN YSDRIVER_ERRORCODE_RETURN
#define YSDRIVER_ERROR YSDRIVER_ERRORCODE

#if 0
#define YSDRIVER_INFO_ERROR(Msg)                                        \
        if(drv_DebugIsEnabelDebug(CURRENT_MODULE))                          \
        {                                                               \
            drv_print(severity_error,"[DRIVER]:[%s]:[%12.12s@%04d]:[0x%X]\n",__func__, __FILE__, __LINE__);printf Msg; \
        }

#define YSDRIVER_INFO(Msg)                 \
        if(drv_DebugIsEnabelPrint(CURRENT_MODULE))                         \
        {                                                               \
            drv_print(severity_info,"[DRIVER]:[%s]",__func__);          \
            printf Msg;                                                 \
        }

#define YSDRIVER_INFO_SUCCESS YSDRIVER_INFO(("SUCCESS\n"))
#endif


enum
{
    DRV_PANEL_ERROR_HANDLE = DRV_PANEL_BASE + 1,
    DRV_PANEL_ERROR_INIT,
    DRV_PANEL_ERROR_TERM,
    DRV_PANEL_ERROR_PIO_OPEN,
    DRV_PANEL_ERROR_PIO_COMPARE,
};

#define GetErrorText(p) ST_GetErrorText(p)

void do_report(report_severity_t report_severity, char *format, ...);

char *YSDRIVER_GetErrorText(DRV_ErrCode ErrorCode);

/*****************************************************************************
* Function    : DRV_DebugGetRevision
* Description :
*   获得Debug模块的驱动版本号.
* Note        :
*  
******************************************************************************/
DRV_Revision DRV_DebugGetRevision(void);

/*****************************************************************************
* Function    : DRV_DebugGetDriverRevision
* Description :
*   获得驱动层的版本号.
* Note        :
*  
******************************************************************************/
DRV_Revision DRV_DebugGetDriverRevision(void);

/*****************************************************************************
* Function    : DRV_DebugGetPlatformId
* Description :
*   得到平台的ID.
* Note        :
******************************************************************************/
DRV_ErrCode DRV_DebugGetPlatformId(U32_YS *PlatformId_p);

/*****************************************************************************
* Function    : DRV_DebugSetPlatformErrorCode
* Description :
*   设置平台错误码.
* Note        :
******************************************************************************/
DRV_ErrCode DRV_DebugSetPlatformErrorCode(U32_YS ErrCode);

/*****************************************************************************
* Function    : DRV_DebugGetPlatformErrorCode
* Description :
*   得到平台错误码.
* Note        :
******************************************************************************/
DRV_ErrCode DRV_DebugGetPlatformErrorCode(U32_YS *ErrCode_p);

/*****************************************************************************
* Function    : DRV_DebugEnablePrint
* Description :
*  打开模块的打印信息.
* Note        :
*  如果参数为DRV_MODULE_MAX_NB，则打开所有模块
******************************************************************************/
DRV_ErrCode DRV_DebugEnablePrint(DRV_Module Module);

/*****************************************************************************
* Function    : DRV_DebugDisablePrint
* Description :
*  关闭模块的打印信息.
* Note        :
*  如果参数为DRV_MODULE_MAX_NB，则关闭所有模块
******************************************************************************/
DRV_ErrCode DRV_DebugDisablePrint(DRV_Module Module);

/*****************************************************************************
* Function    : DRV_DebugEnableDebug
* Description :
*  打开模块的调试信息.
* Note        :
*  如果参数为DRV_MODULE_MAX_NB，则打开所有模块
******************************************************************************/
DRV_ErrCode DRV_DebugEnableDebug(DRV_Module Module);

/*****************************************************************************
* Function    : DRV_DebugDisableDebug
* Description :
*  关闭模块的调试信息.
* Note        :
*  如果参数为DRV_MODULE_MAX_NB，则关闭所有模块
******************************************************************************/
DRV_ErrCode DRV_DebugDisableDebug(DRV_Module Module);
U8_YS* DRV_DebugGetModuleName(DRV_Module Module);


DRV_ErrCode DRV_DebugPrintMeminfo(void);

DRV_ErrCode DRV_DebugPrintRevision(DRV_Module Module);


#ifdef __cplusplus
}
#endif
#endif

