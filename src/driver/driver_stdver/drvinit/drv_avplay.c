/*****************************************************************************

File name: vid.c

Description: STVID interface

COPYRIGHT (C) 2004 STMicroelectronics

*****************************************************************************/

/* Includes --------------------------------------------------------------- */

#include <string.h>
#include <stdio.h>
#include <hi_type.h>
#include <hi_unf_avplay.h>
#include <hi_unf_sound.h>
#include <hi_error_mpi.h>
#include "HA.AUDIO.G711.codec.h"
#include "HA.AUDIO.MP3.decode.h"
#include "HA.AUDIO.MP2.decode.h"
#include "HA.AUDIO.AAC.decode.h"
#include "HA.AUDIO.DRA.decode.h"
#include "HA.AUDIO.PCM.decode.h"
#include "HA.AUDIO.WMA9STD.decode.h"
#include "HA.AUDIO.AMRNB.codec.h"
#include "HA.AUDIO.AMRWB.codec.h"
#include "HA.AUDIO.TRUEHDPASSTHROUGH.decode.h"
#include "HA.AUDIO.DTSHD.decode.h"
#include "HA.AUDIO.DOLBYPLUS.decode.h"
#include "HA.AUDIO.AC3PASSTHROUGH.decode.h"
#include "HA.AUDIO.FFMPEG_DECODE.decode.h"
#include "HA.AUDIO.AAC.encode.h"

#ifndef STTBX_PRINT
#define STTBX_PRINT
#endif
#include "drv_basic.h"
#include "drv_avplay.h"
#include "drv_vo.h"
#include "drv_os.h"

static BOOL m_IsVidAspectRatioConversionAutoForHd;
HI_U8 u8DecOpenBuf[1024];
HI_U8 u8EncOpenBuf[1024];
static DRVOS_Sem_t*  m_YSVidLinkProtectSem_p;
static DRVOS_Sem_t*  m_YSVidInjectProtectSem_p;
HI_HANDLE  mAvPlayHandle;

DRV_ErrCode drv_videoLinkClaim()
{
 DRV_OS_SemWait(m_YSVidLinkProtectSem_p,DRVOS_WAIT_INFINITY,0);   
 return 0;
}
DRV_ErrCode drv_videoLinkRelease()
{
    DRV_OSSemSignal(m_YSVidLinkProtectSem_p);
    return 0;
}

DRV_ErrCode drv_videoInjectClaim()
{
  DRV_OS_SemWait(m_YSVidInjectProtectSem_p,DRVOS_WAIT_INFINITY,0);   
    return 0;
}
DRV_ErrCode drv_videoInjectRelease()
{
    DRV_OSSemSignal(m_YSVidInjectProtectSem_p);
    return 0;
}

/* Functions ---------------------------------------------------------- */

DRV_ErrCode GetWritePtrFct(void * const Handle_p, void ** const Address_p)
{
    DRV_ErrCode Err = DRV_NO_ERROR;
    return(Err);
}

void SetReadPtrFct(void * const Handle_p, void * const Address_p)
{
    DRV_ErrCode Err;
    return;
}


HI_S32 DRV_AvplayerRegADecLib()
{
    HI_S32 Ret = HI_SUCCESS;

	Ret = HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.AMRWB.codec.so");
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.MP3.decode.so");
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.MP2.decode.so");
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.AAC.decode.so");
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.DRA.decode.so");
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.TRUEHDPASSTHROUGH.decode.so");
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.AMRNB.codec.so");
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.WMA.decode.so");
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.COOK.decode.so");
	Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.DOLBYPLUS.decode.so");
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.DTSHD.decode.so");
	Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.DTSPASSTHROUGH.decode.so");
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.AC3PASSTHROUGH.decode.so");
    Ret |= HI_UNF_AVPLAY_RegisterAcodecLib("libHA.AUDIO.PCM.decode.so");

    if (Ret != HI_SUCCESS)
    {
        printf("\n\n!!! some audio codec NOT found. you may NOT able to decode some audio type.\n\n");
    }
    
    return HI_SUCCESS;
}

DRV_ErrCode AVPLAY_Setup(void)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
    HI_UNF_AVPLAY_ATTR_S        AvplayAttr;
    HI_UNF_SYNC_ATTR_S          SyncAttr;
    HI_UNF_AVPLAY_STOP_OPT_S    Stop;
    DRV_AvplayerRegADecLib();
    ErrCode = HI_UNF_AVPLAY_Init();
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("AVPLAY_Setup HI_UNF_AVPLAY_Init failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }

    ErrCode = HI_UNF_SO_Init();//ffmpegµÄ×ÖÄ»
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("AVPLAY_Setup HI_UNF_SO_Init failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }

    m_YSVidLinkProtectSem_p = DRV_OS_SemCreate(NULL,1,DRVOS_INIT_FIFO_WAIT);
    m_YSVidInjectProtectSem_p = DRV_OS_SemCreate(NULL,1,DRVOS_INIT_FIFO_WAIT);
    
	ErrCode = DRV_AvplayerSetup(HI_UNF_AVPLAY_STREAM_TYPE_TS,HI_UNF_AVPLAY_MEDIA_CHAN_AUD|HI_UNF_AVPLAY_MEDIA_CHAN_VID,&mAvPlayHandle);
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("AVPLAY_Setup DRV_AvplayerSetup failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }
	/* Return no errors */
 /* ================ */
 return(ErrCode);
}
DRV_ErrCode DRV_AvplayerSetup(HI_UNF_AVPLAY_STREAM_TYPE_E SteamType,HI_UNF_AVPLAY_MEDIA_CHAN_E Channel,HI_HANDLE *PlayerHandle_p)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
    HI_UNF_AVPLAY_ATTR_S        AvplayAttr;
    HI_UNF_SYNC_ATTR_S          SyncAttr;
    HI_UNF_AVPLAY_STOP_OPT_S    Stop;
    ErrCode = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, SteamType);
    ErrCode |= HI_UNF_AVPLAY_Create(&AvplayAttr, PlayerHandle_p);
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("AVPLAY_Setup HI_UNF_AVPLAY_Create failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }

    if((Channel&HI_UNF_AVPLAY_MEDIA_CHAN_AUD) == HI_UNF_AVPLAY_MEDIA_CHAN_AUD)
    {
	    ErrCode = HI_UNF_AVPLAY_ChnOpen(*PlayerHandle_p, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
	    if (ErrCode != DRV_NO_ERROR)
	    {
	        printf("AVPLAY_Setup HI_UNF_AVPLAY_ChnOpen failed.ErrCode 0x%08x\n",ErrCode);
	        return ErrCode;
	    }
	    ErrCode = HI_UNF_SND_Attach(HI_UNF_SND_0, *PlayerHandle_p, HI_UNF_SND_MIX_TYPE_MASTER, 100);
	    if (ErrCode != DRV_NO_ERROR)
	    {
	        printf("AVPLAY_Setup HI_SND_Attach failed.ErrCode 0x%08x\n",ErrCode);
	        return ErrCode;
	    }
        ErrCode = HI_UNF_SND_SetTrackMode(HI_UNF_SND_0, HI_UNF_TRACK_MODE_DOUBLE_MONO);
        if (ErrCode != HI_SUCCESS)
        {
            printf("call HI_UNF_SND_SetTrackMode failed.\n");
            return HI_FAILURE;
        }
    }
    if((Channel&HI_UNF_AVPLAY_MEDIA_CHAN_VID) == HI_UNF_AVPLAY_MEDIA_CHAN_VID)
    {
	    ErrCode = HI_UNF_AVPLAY_ChnOpen(*PlayerHandle_p, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
	    if (ErrCode != DRV_NO_ERROR)
	    {
	        printf("AVPLAY_Setup HI_UNF_AVPLAY_ChnOpen failed.ErrCode 0x%08x\n",ErrCode);
	        return ErrCode;
	    }
	    ErrCode = HI_UNF_VO_AttachWindow(mAvPlayWinHandle, *PlayerHandle_p);
	    if (ErrCode != DRV_NO_ERROR)
	    {
	        printf("AVPLAY_Setup HI_UNF_VO_AttachWindow failed.ErrCode 0x%08x\n",ErrCode);
	        return ErrCode;
	    }
	    ErrCode = HI_UNF_VO_SetWindowEnable(mAvPlayWinHandle, HI_TRUE);
	    if (ErrCode != DRV_NO_ERROR)
	    {
	        printf("AVPLAY_Setup HI_UNF_VO_SetWindowEnable failed.ErrCode 0x%08x\n",ErrCode);
	        return ErrCode;
	    }
    }
    ErrCode = HI_UNF_AVPLAY_GetAttr(*PlayerHandle_p, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    SyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
    SyncAttr.stSyncStartRegion.s32VidPlusTime = 100;
    SyncAttr.stSyncStartRegion.s32VidNegativeTime = -100;
    SyncAttr.stSyncStartRegion.bSmoothPlay = HI_TRUE;
    SyncAttr.bQuickOutput = HI_TRUE;
    SyncAttr.u32PreSyncTimeoutMs = 600;
    ErrCode = HI_UNF_AVPLAY_SetAttr(*PlayerHandle_p, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("AVPLAY_Setup HI_UNF_AVPLAY_SetAttr failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }
	return ErrCode;
}
DRV_ErrCode DRV_AvplayerTerm(HI_HANDLE PlayerHandle,HI_UNF_AVPLAY_MEDIA_CHAN_E Channel)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
    HI_UNF_AVPLAY_ATTR_S        AvplayAttr;
    HI_UNF_SYNC_ATTR_S          SyncAttr;
    HI_UNF_AVPLAY_STOP_OPT_S    Stop;
    
    if((Channel&HI_UNF_AVPLAY_MEDIA_CHAN_AUD) == HI_UNF_AVPLAY_MEDIA_CHAN_AUD)
    {
	    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_STILL;
	    Stop.u32TimeoutMs = 0;
	   	ErrCode = HI_UNF_AVPLAY_Stop(PlayerHandle, HI_UNF_AVPLAY_MEDIA_CHAN_AUD,  &Stop);
	    if (ErrCode != DRV_NO_ERROR)
	    {
	        printf("DRV_AvplayerTerm HI_UNF_AVPLAY_Stop failed.ErrCode 0x%08x\n",ErrCode);
	        return ErrCode;
	    }
		ErrCode = HI_UNF_SND_Detach(HI_UNF_SND_0, PlayerHandle);
	    if (HI_SUCCESS != ErrCode)
	    {
	        printf("DRV_AvplayerTerm HI_UNF_SND_Detach ErrCode 0x%08x \n", ErrCode);
	        return ErrCode;
	    }
	    ErrCode = HI_UNF_AVPLAY_ChnClose(PlayerHandle, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
	    if (ErrCode != HI_SUCCESS)
	    {
	        printf("DRV_AvplayerTerm HI_UNF_AVPLAY_ChnClose failed.ErrCode 0x%08x\n",ErrCode);
	        return ErrCode;
	    }
   	}
    if((Channel&HI_UNF_AVPLAY_MEDIA_CHAN_VID) == HI_UNF_AVPLAY_MEDIA_CHAN_VID)
    {
	    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_STILL;
	    Stop.u32TimeoutMs = 0;
	    ErrCode |= HI_UNF_AVPLAY_Stop(PlayerHandle, HI_UNF_AVPLAY_MEDIA_CHAN_VID,  &Stop);
	    if (ErrCode != DRV_NO_ERROR)
	    {
	        printf("DRV_AvplayerTerm HI_UNF_AVPLAY_Stop failed.ErrCode 0x%08x\n",ErrCode);
	        return ErrCode;
	    }
	    ErrCode = HI_UNF_VO_DetachWindow(mAvPlayWinHandle,PlayerHandle);
	    if (ErrCode != DRV_NO_ERROR)
	    {
	        printf("DRV_AvplayerTerm HI_UNF_VO_DetachWindow failed.ErrCode 0x%08x\n",ErrCode);
	        return ErrCode;
	    }
	    ErrCode = HI_UNF_AVPLAY_ChnClose(PlayerHandle, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
	    if (ErrCode != HI_SUCCESS)
	    {
	        printf("DRV_AvplayerTerm HI_UNF_AVPLAY_ChnClose failed.ErrCode 0x%08x\n",ErrCode);
	        return ErrCode;
	    }
    }
	ErrCode = HI_UNF_AVPLAY_Destroy(PlayerHandle);
    if (ErrCode != HI_SUCCESS)
    {
        printf("DRV_AvplayerTerm HI_UNF_AVPLAY_ChnClose failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }
	return ErrCode;
}

HI_S32 HIADP_AVPlay_SetVdecAttr(HI_HANDLE hAvplay,HI_UNF_VCODEC_TYPE_E enType,HI_UNF_VCODEC_MODE_E enMode)
{
    HI_S32 ErrCode;
    HI_UNF_VCODEC_ATTR_S        VdecAttr;
    
    ErrCode = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &VdecAttr);
    if (HI_SUCCESS != ErrCode)
    {
        printf("HI_UNF_AVPLAY_GetAttr failed:%#x\n",ErrCode);        
        return ErrCode;
    }

    VdecAttr.enType = enType;
    VdecAttr.enMode = enMode;
    VdecAttr.u32ErrCover = 100;
    VdecAttr.u32Priority = 3;

    ErrCode = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &VdecAttr);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
        return ErrCode;
    }

    return ErrCode;
}


DOLBYPLUS_STREAM_INFO_S g_stDDpStreamInfo;
/*dolby Dual Mono type control*/
HI_U32  g_u32DolbyAcmod = 0;
HI_BOOL g_bDrawChnBar = HI_TRUE;

static HI_VOID DDPlusCallBack(DOLBYPLUS_EVENT_E Event, HI_VOID *pUserData)
{
    DOLBYPLUS_STREAM_INFO_S *pstInfo = (DOLBYPLUS_STREAM_INFO_S *)pUserData;
#if 0
	printf( "DDPlusCallBack show info:\n \
                s16StreamType          = %d\n \
                s16Acmod               = %d\n \
                s32BitRate             = %d\n \
                s32SampleRateRate      = %d\n \
                Event                  = %d\n", 
                pstInfo->s16StreamType, pstInfo->s16Acmod, pstInfo->s32BitRate, pstInfo->s32SampleRateRate,Event);
#endif                
	g_u32DolbyAcmod = pstInfo->s16Acmod;

	if (HA_DOLBYPLUS_EVENT_SOURCE_CHANGE == Event)
    {
		g_bDrawChnBar = HI_TRUE;
		//printf("DDPlusCallBack enent !\n");
	}
    return;
}



HI_S32 HIADP_AVPlay_SetAdecAttr(HI_HANDLE hAvplay, HI_U32 enADecType, HI_HA_DECODEMODE_E enMode, HI_S32 isCoreOnly)
{
    HI_UNF_ACODEC_ATTR_S AdecAttr;
    WAV_FORMAT_S stWavFormat;

    HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_ADEC, &AdecAttr);
    AdecAttr.enType = enADecType;

    if (HA_AUDIO_ID_PCM == AdecAttr.enType)
    {
        HI_BOOL isBigEndian;
        /* set pcm wav format here base on pcm file */
        stWavFormat.nChannels = 1;
        stWavFormat.nSamplesPerSec = 48000;
        stWavFormat.wBitsPerSample = 16;
        /* if big-endian pcm */
        isBigEndian = HI_FALSE;
        if(HI_TRUE == isBigEndian)
        {
        	stWavFormat.cbSize =4;
        	stWavFormat.cbExtWord[0] = 1;
        }
        HA_PCM_DecGetDefalutOpenParam(&(AdecAttr.stDecodeParam),&stWavFormat);
        printf("please make sure the attributes of PCM stream is tme same as defined in function of \"HIADP_AVPlay_SetAdecAttr\"? \n");
        printf("(nChannels = 1, wBitsPerSample = 16, nSamplesPerSec = 48000, isBigEndian = HI_FALSE) \n");
    }
#if 0    
    else if (HA_AUDIO_ID_G711 == AdecAttr.enType)
    {
         HA_G711_GetDecDefalutOpenParam(&(AdecAttr.stDecodeParam));
    }
#endif    
    else if (HA_AUDIO_ID_MP2 == AdecAttr.enType)
    {
    	 HA_MP2_DecGetDefalutOpenParam(&(AdecAttr.stDecodeParam));
    }
    else if (HA_AUDIO_ID_AAC == AdecAttr.enType)
    {
    	 HA_AAC_DecGetDefalutOpenParam(&(AdecAttr.stDecodeParam));
    }
    else if (HA_AUDIO_ID_MP3 == AdecAttr.enType)
    {
    	 HA_MP3_DecGetDefalutOpenParam(&(AdecAttr.stDecodeParam));
    }
    else if (HA_AUDIO_ID_AMRNB== AdecAttr.enType)
    {
        AMRNB_DECODE_OPENCONFIG_S *pstConfig = (AMRNB_DECODE_OPENCONFIG_S *)u8DecOpenBuf;        
        HA_AMRNB_GetDecDefalutOpenParam(&(AdecAttr.stDecodeParam), pstConfig);
        pstConfig->enFormat = AMRNB_MIME;
    }
    else if (HA_AUDIO_ID_AMRWB== AdecAttr.enType)
    {
        AMRWB_DECODE_OPENCONFIG_S *pstConfig = (AMRWB_DECODE_OPENCONFIG_S *)u8DecOpenBuf;
        HA_AMRWB_GetDecDefalutOpenParam(&(AdecAttr.stDecodeParam), pstConfig);
        pstConfig->enFormat = AMRWB_FORMAT_MIME;
    }
    else if (HA_AUDIO_ID_AC3PASSTHROUGH== AdecAttr.enType)
    {
        HA_AC3PASSTHROUGH_DecGetDefalutOpenParam(&(AdecAttr.stDecodeParam));
        AdecAttr.stDecodeParam.enDecMode = HD_DEC_MODE_THRU;
    }
    else if (HA_AUDIO_ID_TRUEHD == AdecAttr.enType)
    {
        HA_TRUEHD_DecGetDefalutOpenParam(&(AdecAttr.stDecodeParam));
        if (HD_DEC_MODE_THRU != enMode)
        {
            printf(" MLP decoder enMode(%d) error (mlp only support hbr Pass-through only).\n", enMode);
            return -1;
        }

        AdecAttr.stDecodeParam.enDecMode = HD_DEC_MODE_THRU;        /* truehd just support pass-through */
        printf(" TrueHD decoder(HBR Pass-through only).\n");
    }
    else if (HA_AUDIO_ID_DTSHD == AdecAttr.enType)
    {
        DTSHD_DECODE_OPENCONFIG_S *pstConfig = (DTSHD_DECODE_OPENCONFIG_S *)u8DecOpenBuf;        
    	HA_DTSHD_DecGetDefalutOpenConfig(pstConfig);
        HA_DTSHD_DecGetDefalutOpenParam(&(AdecAttr.stDecodeParam), pstConfig);
		AdecAttr.stDecodeParam.enDecMode = HD_DEC_MODE_SIMUL;

    }
	else if (HA_AUDIO_ID_DOLBY_PLUS == AdecAttr.enType)
	{
		DOLBYPLUS_DECODE_OPENCONFIG_S *pstConfig = (DOLBYPLUS_DECODE_OPENCONFIG_S *)u8DecOpenBuf;
		HA_DOLBYPLUS_DecGetDefalutOpenConfig(pstConfig);
		pstConfig->pfnEvtCbFunc[HA_DOLBYPLUS_EVENT_SOURCE_CHANGE] = DDPlusCallBack;
		pstConfig->pAppData[HA_DOLBYPLUS_EVENT_SOURCE_CHANGE] = &g_stDDpStreamInfo;
		/* Dolby DVB Broadcast default settings */
		pstConfig->enDrcMode = DOLBYPLUS_DRC_RF; 
		pstConfig->enDmxMode = DOLBYPLUS_DMX_SRND; 
		HA_DOLBYPLUS_DecGetDefalutOpenParam(&(AdecAttr.stDecodeParam), pstConfig);
		AdecAttr.stDecodeParam.enDecMode = HD_DEC_MODE_SIMUL;
	}
    else
    {
	     HA_DRA_DecGetDefalutOpenParam(&(AdecAttr.stDecodeParam));
    }

    HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_ADEC, &AdecAttr);

    return DRV_NO_ERROR;
}

/* EOF --------------------------------------------------------------------- */
