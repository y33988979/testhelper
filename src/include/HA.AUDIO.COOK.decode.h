/******************************************************************************
 Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : HA.AUDIO.COOK.decode.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       : 
 Last Modified :
 Description   : 
 Function List :
 History       :
******************************************************************************/
#ifndef __HISI_AUDIO_DECODER_COOK_H__
#define __HISI_AUDIO_DECODER_COOK_H__

#include "hi_type.h"
#include "hi_audio_codec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define    HA_COOK_ID 0x0009        
#define    HA_AUDIO_ID_COOK   HA_BUILD_CODEC_ID(VENDOR_HISI,FORMAT_COOK,HA_COOK_ID)   /**< support FORMAT_COOK, RealAudio */

/*
 * ra_format_info struct
 *
 */

typedef struct  hiRA_FORMAT_INFO_S
{
    HI_U32 ulSampleRate;
    HI_U32 ulActualRate;
    HI_U16 usBitsPerSample;
    HI_U16 usNumChannels;
    HI_U16 usAudioQuality;
    HI_U16 usFlavorIndex;
    HI_U32 ulBitsPerFrame;
    HI_U32 ulGranularity;
    HI_U32 ulOpaqueDataSize;
    HI_U8*  pOpaqueData;
} RA_FORMAT_INFO_S;


#define HA_COOK_DecGetDefalutOpenParam(pOpenParam, pstPrivateParams) \
		do { HI_S32 i; \
			 ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->enDecMode = HD_DEC_MODE_RAWPCM; \
			 ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32DesiredOutChannels = 2; \
			 ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.bInterleaved = HI_TRUE; \
			 ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32BitPerSample = 16; \
			 ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32DesiredSampleRate = 48000; \
			 for (i = 0; i < HA_AUDIO_MAXCHANNELS; i++) \
			 { \
				 ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.enChannelMapping[i] = HA_AUDIO_ChannelNone; \
			 } \
			 ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->pCodecPrivateData = (HI_VOID*)pstPrivateParams; \
			 ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->u32CodecPrivateDataSize = sizeof(RA_FORMAT_INFO_S); \
		} while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HISI_AUDIO_DECODER_COOK_H__ */

