/*****************************************************************************
*
* Description: 
*    多屏互动
*
* Revision History:
*    Date          Author         Modification
* ---------------------------------------------------------------------
* 1. 2012-09-11    wrwei          创建
* 
*
*****************************************************************************/
#ifndef _YS_TRANSCODER_H_
#define _YS_TRANSCODER_H_

#include "drv_typedef.h"
#include "ys_typedef.h"

#include "HA.AUDIO.MP3.decode.h"
#include "HA.AUDIO.MP2.decode.h"
#include "HA.AUDIO.AAC.decode.h"
#include "HA.AUDIO.DRA.decode.h"
#include "HA.AUDIO.PCM.decode.h"
#include "HA.AUDIO.WMA9STD.decode.h"
#include "HA.AUDIO.AMRNB.codec.h"
#include "HA.AUDIO.AAC.encode.h"
#include "hi_unf_aenc.h"
#include "hi_unf_venc.h"
#ifdef __cplusplus
extern "C" {
#endif

DRV_ErrCode DRV_Transcoder_Init(void);

DRV_ErrCode DRV_Transcoder_DeInit(void);

DRV_ErrCode DRV_Transcoder_Open(void);

DRV_ErrCode DRV_Transcoder_Close(void);

DRV_ErrCode DRV_Transcoder_Start(void);

DRV_ErrCode DRV_Transcoder_Stop(void);

DRV_ErrCode DRV_Transcoder_Reset(void);

DRV_ErrCode DRV_Transcoder_VideoEncInit(void);

DRV_ErrCode DRV_Transcoder_VideoEncDeInit(void);

DRV_ErrCode DRV_Transcoder_VideoEncStart(void);

DRV_ErrCode DRV_Transcoder_VideoEncStop(void);

DRV_ErrCode DRV_Transcoder_AudioEncInit(void);

DRV_ErrCode DRV_Transcoder_AudioEncDeInit(void);

DRV_ErrCode DRV_Transcoder_AudioEncStart(void);

DRV_ErrCode DRV_Transcoder_AudioEncStop(void);

#ifdef __cplusplus
}
#endif
#endif

