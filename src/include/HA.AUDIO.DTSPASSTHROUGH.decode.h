/******************************************************************************
 Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : HA.AUDIO.DTSPASSTHROUGH.decode.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       : 
 Last Modified :
 Description   : 
 Function List :
 History       :
******************************************************************************/
#ifndef __HISI_AUDIO_DECODER_DTSPASSTHROUGH_H__
#define __HISI_AUDIO_DECODER_DTSPASSTHROUGH_H__

#include "hi_type.h"
#include "hi_audio_codec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HA_DTSPASSTHROUGH_ID 0x1025      
#define HA_AUDIO_ID_DTSPASSTHROUGH   HA_BUILD_CODEC_ID(VENDOR_HISI,FORMAT_DTS,HA_DTSPASSTHROUGH_ID)  /**< support FORMAT_PCM */

#define HA_DTSPASSTHROUGH_DecGetDefalutOpenParam(pOpenParam) \
do{ HI_S32 i; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->enDecMode = HD_DEC_MODE_THRU; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->pCodecPrivateData = HI_NULL; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->u32CodecPrivateDataSize = 0; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32DesiredOutChannels = 2; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.bInterleaved  = HI_TRUE; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32BitPerSample = 16; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32DesiredSampleRate = 48000; \
    for (i = 0; i < HA_AUDIO_MAXCHANNELS; i++) \
    { \
        ((HI_HADECODE_OPENPARAM_S *)pOpenParam)->sPcmformat.enChannelMapping[i] = HA_AUDIO_ChannelNone; \
    } \
}while(0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HISI_AUDIO_DECODER_DTSPASSTHROUGH_H__ */

