/*****************************************************************************

File name: vid.c

Description: STVID interface

COPYRIGHT (C) 2004 STMicroelectronics

*****************************************************************************/
#ifndef _DRV_AVPLAY_H_
#define _DRV_AVPLAY_H_
/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif
#include <hi_type.h>
#include <hi_unf_avplay.h>
#include "drv_typedef.h"

/* Includes --------------------------------------------------------------- */

extern HI_HANDLE  mAvPlayHandle;


DRV_ErrCode AVPLAY_Setup(void);

DRV_ErrCode DRV_AvplayerSetup(HI_UNF_AVPLAY_STREAM_TYPE_E SteamType,HI_UNF_AVPLAY_MEDIA_CHAN_E Channel,HI_HANDLE *PlayerHandle_p);
DRV_ErrCode DRV_AvplayerTerm(HI_HANDLE PlayerHandle,HI_UNF_AVPLAY_MEDIA_CHAN_E Channel);
HI_S32 HIADP_AVPlay_SetVdecAttr(HI_HANDLE hAvplay,HI_UNF_VCODEC_TYPE_E enType,HI_UNF_VCODEC_MODE_E enMode);
HI_S32 HIADP_AVPlay_SetAdecAttr(HI_HANDLE hAvplay, HI_U32 enADecType, HI_HA_DECODEMODE_E enMode, HI_S32 isCoreOnly);
HI_S32 HIADP_AENC_GetAttr(HI_UNF_AENC_ATTR_S *pAencAttr, HI_VOID *pstConfig);

/* C++ support */
/* ----------- */
#ifdef __cplusplus
}
#endif
#endif

/* EOF --------------------------------------------------------------------- */
