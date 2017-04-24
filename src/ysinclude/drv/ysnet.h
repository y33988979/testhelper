/*****************************************************************************
*
* Description: 
*    驱动层网络模块头文件
*
* Revision History:
*    Date          Author         Modification
* ---------------------------------------------------------------------
* 1. 2005-10-12    Qiujordan      创建
*
*****************************************************************************/
#ifndef _YSNET_H_
#define _YSNET_H_

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif

#include "drv_typedef.h"
#include "drv_video.h"
#include "drv_audio.h"
#include "stnet.h"
#include "ysav.h"


typedef enum NET_StreamType_e
{
    NET_NOPID   = -1,
    NET_MP1V    =  1,
    NET_MP2V,
    NET_MP4V2=NET_MP2V,
    NET_H264,
    NET_VC1,
    NET_MP1A,
    NET_MP2A,
    NET_MP4A,
    NET_PRIVATE,
    NET_TTXT, 
    NET_SUBT,
    NET_PCR,
    NET_DSMCC,
    NET_CC,
    NET_AC3     = 0x81,
    NET_WMA     = 0xE6,
    NET_OTHER   = 0xFF
} NET_StreamType_t;

typedef struct 
{
    int  nTSVID;
    int  nTSAID;
    int  nTSPcrID;
	NET_StreamType_t VideoType;
	NET_StreamType_t AudioType;
  	U32 uLengthOfStream;
}avpara;

typedef enum
{
    NET0 = 0,
    NET1 = 1,
    NET2 = 2,
    NET3 = 3,
    NET_MAXDEVICE
} NET_DeviceId_t;

/* HandleId for multi-instance access */
typedef U32 NET_HandleId_t;

typedef struct
{
    ST_DeviceName_t  DeviceName;
    STNET_Handle_t  *Handle_p;    /* Handles for multi-instance */
} NET_DeviceHandle_t;

extern NET_DeviceHandle_t NETDeviceHandle[];

#define NET_GET_HANDLE(h,id)  (NETDeviceHandle[(h)].Handle_p[(id)])
#define NET_GET_DEVICENAME(h) (NETDeviceHandle[(h)].DeviceName)
#define NET_HANDLE_NULL       (STNET_Handle_t)NULL


typedef struct DRV_NetVideoParameter_s
{
    DRV_VideoType       VideoType;
    DRV_VideoType StreamType;
    DRV_Pid             VideoPid;
    U32_YS	TotalTimeOfAV;
}DRV_NetVideoParameter_t;
    
    
typedef struct DRV_NetAudioParameter_s
{
    DRV_AudioType       AudioType;
    DRV_AudioType StreamType;
    DRV_Pid             AudioPid;
}DRV_NetAudioParameter_t;
    
typedef void (*DRV_NetCallback)(U32_YS State);

DRV_ErrCode DRV_NetInit(U32_YS Instance);
DRV_ErrCode DRV_NetOpen(U32_YS Instance);
DRV_ErrCode DRV_NetReOpen(U32_YS Instance);
DRV_ErrCode DRV_NetClose(U32_YS Instance);

DRV_ErrCode DRV_NetStart(U32_YS Instance);
DRV_ErrCode DRV_NetStop(U32_YS Instance);

DRV_ErrCode DRV_NetPause(U32_YS Instance);
DRV_ErrCode DRV_NetResume(U32_YS Instance);

DRV_ErrCode DRV_NetSetServerIP(U32_YS Instance, char *ServerIP_p);
DRV_ErrCode DRV_NetSetBoardIP(U32_YS Instance, char *BoardIP_p);
DRV_ErrCode DRV_NetSetPort(U32_YS Instance, U32_YS Port);

DRV_ErrCode DRV_NetSetStreamSource(U32_YS Instance, BOOL_YS IsRTSP);

DRV_ErrCode DRV_NetGetStreamSource(U32_YS Instance, BOOL_YS *IsRTSP);

DRV_ErrCode DRV_NetGetVideoParameter(U32_YS Instance,DRV_NetVideoParameter_t *Para);
DRV_ErrCode DRV_NetGetAudioParameter(U32_YS Instance,DRV_NetAudioParameter_t *Para);

DRV_ErrCode DRV_NetSetRtspUrl(U32_YS Instance, char *RtspUrl_p);
DRV_ErrCode DRV_NetJump(U32_YS Instance,char* time);

DRV_ErrCode DRV_NetPrintIPInfo(void);

DRV_ErrCode DRV_NetEnableCallback(DRV_NetCallback Callback_f);
DRV_ErrCode DRV_NetDisableCallback(DRV_NetCallback Callback_f);
#ifdef __cplusplus
}
#endif
#endif

