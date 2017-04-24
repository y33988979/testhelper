/*----------------------------------------------------------------------------
 *  Include
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 在此包含其他模块的头文件，自己的头文件放在函数定义之前
*----------------------------------------------------------------------------*/

#include <string.h>
#ifndef STTBX_PRINT
#define STTBX_PRINT
#endif
#include "drv_basic.h"
#include "ysav.h"
#include "drv_sys.h"
#include "drv_avplay.h"
#include "drv_snd.h"
#include "drv_disp.h"
#include "drv_vo.h"
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
#include "osddrv.h"
#include "hi_svr_player.h"
#include "ystranscoder.h"
#include "hi_unf_hdmi.h"
#include "hi_go_gdev.h"

/*----------------------------------------------------------------------------
 *  Debug
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 *  External Function Prototype
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 引用的外部函数原型
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 *  External variable
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *  Struct
 *----------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------
 *  Private Constant & Enum & Macro
 *----------------------------------------------------------------------------*/
#define MAX_VOL_LEVEL 63
#define MIN_VOL_LEVEL 0
#define NTSC_X_OFFSET 0
#define NTSC_Y_OFFSET 0

/*----------------------------------------------------------------------------
 *  Global variable
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *  Private Module Variable
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 必须定义为static变量，防止外部引用
*----------------------------------------------------------------------------*/

#define  DENC_DELAY_DEFAULT_VALUE        (0x35)
#define  DENC_BRIGHTNESS_DEFAULT_VALUE   (50)
#define  DENC_CONTRAST_DEFAULT_VALUE     (50)
#define  DENC_SATURATION_DEFAULT_VALUE   (50)
#define DENC_OUT_ADJUST_RANGE (50)
S8_YS DENCBrightnessDefaultValue  = DENC_BRIGHTNESS_DEFAULT_VALUE;
S8_YS DENCContrastDefaultValue     = DENC_CONTRAST_DEFAULT_VALUE;
S8_YS DENCSaturationDefaultValue  = DENC_SATURATION_DEFAULT_VALUE;
S8_YS DENCDelayDefaultValue         = DENC_DELAY_DEFAULT_VALUE;


DRV_Pid m_curVidPid;/*video当前的PID*/
DRV_Pid m_curAudPid;/*audio当前的PID*/
DRV_Pid m_curPcrPid;/*pcr当前的PID*/
DRV_Pid m_curTtxPid;/*txt当前的PID*/
DRV_AudioType m_curAudStreamContent;/*audio当前type*/
DRV_AvSynMode m_DecodeCurAvSynMode[5] = {DRV_AVSYN_NONE};/*当前的同步模式*/
DRV_AvSynMode m_DecodeLastAvSynMode[5] = {DRV_AVSYN_NONE};/*解码器初始化时的同步模式*/
BOOL_YS m_bEnableVideo =FALSE_YS;/*video当前的状态，TRUE为start，FALSE为stop*/
BOOL_YS m_bEnableAudio =FALSE_YS;/*audio当前的状态，TRUE为start，FALSE为stop*/
BOOL_YS m_bEnablePCR =FALSE_YS;/*pcr当前的状态，TRUE为start，FALSE为stop*/
DRV_VideoType m_curVidStreamContent = DRV_VIDEO_TYPE_MP2V;
DRV_VideoType m_LastVidStreamContent = DRV_VIDEO_TYPE_MP2V;
VID_DeviceId_t m_curVidSDecodeChannel = 0;
VID_DeviceId_t m_LastVidSDecodeChannel = 0;
BOOL m_IsEnableDolbyOutput = TRUE;
BOOL m_DisableOutPutWhenVideoStop = FALSE;
BOOL m_IsNPSwitchAuto = FALSE;

U32_YS DBG_VidEvtCounter;
U32_YS DBG_AudEvtCounter;
U32 m_VidNewEvtCounter = 0;
BOOL m_VidNewEvtCallBackInit[3] = {FALSE};

BOOL_YS m_OutputAlreadyEnabled=FALSE_YS;
//static BOOL m_AutoMode = TRUE;
static DRV_AudioType m_AutoMode;
static DRV_RectWh m_RectOut = {0,0,720,576};
/*----------------------------------------------------------------------------
 *  Private Function Prototype
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 必须定义为static变量，防止外部引用
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
* 在此包含自己的头文件，以确保头文件中所有类型都已声明
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 *  Function
 *----------------------------------------------------------------------------*/
extern HI_S32 HI_MPI_HIAO_SetSpdifCompatible(HI_BOOL bEnable);

 
 /*****************************************************************************
  Date&Time       : 2012-12-03
  Author          : whyu modify
  Function Name   : DRV_AvSynModeSet
  Description     : 同步的属性设置
  Input           : SynMode 同步的方式
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AvSynModeSet(DRV_AvSynMode SynMode)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
    HI_UNF_SYNC_ATTR_S          SyncAttr;
	switch(SynMode)
    {
    	case DRV_AVSYN_AUD_PTS:
		    ErrCode = HI_UNF_AVPLAY_GetAttr(mAvPlayHandle, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
		    if(ErrCode != ST_NO_ERROR)
		    {
		        STTBX_Print(("DRV_AvSynModeSet : HI_UNF_AVPLAY_GetAttr ErrCode 0x%08x\n",ErrCode));
		        return ErrCode;
		    }
		    SyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
		    SyncAttr.stSyncStartRegion.s32VidPlusTime = 100;
		    SyncAttr.stSyncStartRegion.s32VidNegativeTime = -100;
		    SyncAttr.stSyncStartRegion.bSmoothPlay = HI_TRUE;
		    SyncAttr.bQuickOutput = HI_TRUE;
		    SyncAttr.u32PreSyncTimeoutMs = 500;
		    ErrCode = HI_UNF_AVPLAY_SetAttr(mAvPlayHandle, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
		    if(ErrCode != ST_NO_ERROR)
		    {
		        STTBX_Print(("DRV_AvSynModeSet : HI_UNF_AVPLAY_GetAttr ErrCode 0x%08x\n",ErrCode));
		        return ErrCode;
		    }
			break;
    	case DRV_AVSYN_VID_PTS:
		    ErrCode = HI_UNF_AVPLAY_GetAttr(mAvPlayHandle, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
		    if(ErrCode != ST_NO_ERROR)
		    {
		        STTBX_Print(("DRV_AvSynModeSet : HI_UNF_AVPLAY_GetAttr ErrCode 0x%08x\n",ErrCode));
		        return ErrCode;
		    }
		    SyncAttr.enSyncRef = HI_UNF_SYNC_REF_VIDEO;
		    SyncAttr.stSyncStartRegion.s32VidPlusTime = 100;
		    SyncAttr.stSyncStartRegion.s32VidNegativeTime = -100;
		    SyncAttr.stSyncStartRegion.bSmoothPlay = HI_TRUE;
		    SyncAttr.bQuickOutput = HI_TRUE;
		    SyncAttr.u32PreSyncTimeoutMs = 500;
		    ErrCode = HI_UNF_AVPLAY_SetAttr(mAvPlayHandle, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
		    if(ErrCode != ST_NO_ERROR)
		    {
		        STTBX_Print(("DRV_AvSynModeSet : HI_UNF_AVPLAY_SetAttr ErrCode 0x%08x\n",ErrCode));
		        return ErrCode;
		    }
			break;
    	case DRV_AVSYN_PCR:
		    ErrCode = HI_UNF_AVPLAY_GetAttr(mAvPlayHandle, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
		    if(ErrCode != ST_NO_ERROR)
		    {
		        STTBX_Print(("DRV_AvSynModeSet : HI_UNF_AVPLAY_GetAttr ErrCode 0x%08x\n",ErrCode));
		        return ErrCode;
		    }
		    SyncAttr.enSyncRef = HI_UNF_SYNC_REF_PCR;
		    SyncAttr.stSyncStartRegion.s32VidPlusTime = 100;
		    SyncAttr.stSyncStartRegion.s32VidNegativeTime = -100;
		    SyncAttr.stSyncStartRegion.bSmoothPlay = HI_TRUE;
		    SyncAttr.bQuickOutput = HI_TRUE;
		    SyncAttr.u32PreSyncTimeoutMs = 500;
		    ErrCode = HI_UNF_AVPLAY_SetAttr(mAvPlayHandle, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
		    if(ErrCode != ST_NO_ERROR)
		    {
		        STTBX_Print(("DRV_AvSynModeSet : HI_UNF_AVPLAY_SetAttr ErrCode 0x%08x\n",ErrCode));
		        return ErrCode;
		    }
			break;
    	case DRV_AVSYN_NONE_WITH_PCR:
    	case DRV_AVSYN_NONE:
    	default:
		    ErrCode = HI_UNF_AVPLAY_GetAttr(mAvPlayHandle, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
		    if(ErrCode != ST_NO_ERROR)
		    {
		        STTBX_Print(("DRV_AvSynModeSet : HI_UNF_AVPLAY_GetAttr ErrCode 0x%08x\n",ErrCode));
		        return ErrCode;
		    }
		    SyncAttr.enSyncRef = HI_UNF_SYNC_REF_NONE;
		    SyncAttr.stSyncStartRegion.s32VidPlusTime = 100;
		    SyncAttr.stSyncStartRegion.s32VidNegativeTime = -100;
		    SyncAttr.bQuickOutput = HI_FALSE;
		    SyncAttr.u32PreSyncTimeoutMs = 500;
		    ErrCode = HI_UNF_AVPLAY_SetAttr(mAvPlayHandle, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
		    if(ErrCode != ST_NO_ERROR)
		    {
		        STTBX_Print(("DRV_AvSynModeSet : HI_UNF_AVPLAY_SetAttr ErrCode 0x%08x\n",ErrCode));
		        return ErrCode;
		    }
			break;
	}
	m_DecodeCurAvSynMode[m_curVidSDecodeChannel] = SynMode;
	STTBX_Print(( "DRV_AvSynModeSet %d,m_curVidSDecodeChannel %d,ErrCode is 0x%x\n", SynMode,m_curVidSDecodeChannel,ErrCode));
	return ErrCode;
}


/*****************************************************************************
  Date&Time       : 2012-12-03
  Author          : whyu modify
  Function Name   : DRV_AvSynModeSetWithParam
  Description     :和DRV_AvSynModeSet接口相同作用
  Input           : VidStreamContent无效参数，设为0即可。
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AvSynModeSetWithParam(DRV_AvSynMode SynMode,DRV_VideoType VidStreamContent)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
    
	ErrCode = DRV_AvSynModeSet(SynMode);
	return ErrCode;
}


/*****************************************************************************
  Date&Time       : 2012-12-03
  Author          : whyu modify
  Function Name   : drv_AvSynModeChangeCheck
  Description     :检查同步模式是否转换了
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL drv_AvSynModeChangeCheck(DRV_AvSynMode LastSynMode,DRV_AvSynMode CurSynMode)
{
    BOOL LastStatus,CurStatus;
    switch(LastSynMode)
    {
    	case DRV_AVSYN_PCR:
    	case DRV_AVSYN_NONE_WITH_PCR:
    	case DRV_AVSYN_NONE:
    		LastStatus = TRUE;
			break;
    	default:
    		LastStatus = FALSE;    	
			break;
	}
    switch(CurSynMode)
    {
    	case DRV_AVSYN_PCR:
    	case DRV_AVSYN_NONE_WITH_PCR:
    	case DRV_AVSYN_NONE:
    		CurStatus = TRUE;
			break;
    	default:
    		CurStatus = FALSE;    	
			break;
	}
	if(LastStatus != CurStatus)
		return TRUE;
	else
		return FALSE;
}


 /*****************************************************************************
  Date&Time       : 2012-12-04
  Author          : whyu modify
  Function Name   : DRV_ClkRVEnable
  Description     : PCR同步
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void DRV_ClkRVEnable(void)
{
	DRV_AvSynModeSet(DRV_AVSYN_PCR);
}

 
  /*****************************************************************************
   Date&Time       : 2012-12-04
   Author          : whyu modify
   Function Name   : DRV_ClkRVEnable
   Description     : 不同步
   Input           :
   Outut           :  
   Calls           : 
   Called by       :
   Global Variables: 
   Others          : 
 *****************************************************************************/
void DRV_ClkRVDisable(void)
{
	DRV_AvSynModeSet(DRV_AVSYN_NONE);
}


/*-------------------------------------------------------------------------
 * Function : drv_SetDigitalOutputMode
 * Descript : 设置杜比输出模式
 * Input    : Mode，音频格式
 * Output   :
 * Return   : Error Code
 * ----------------------------------------------------------------------*/
DRV_ErrCode drv_SetDigitalOutputMode(DRV_DigitalMode Mode)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	switch(Mode)
	{
		case DRV_AUDIO_DIGITAL_MODE_AC3:
			ErrCode = HI_UNF_SND_SetHdmiPassThrough(HI_UNF_SND_0,HI_TRUE);
			ErrCode |= HI_UNF_SND_SetSpdifPassThrough(HI_UNF_SND_0, HI_TRUE);
		    if(ErrCode != DRV_NO_ERROR)
		    {
		        STTBX_Print(("drv_SetDigitalOutputMode : HI_UNF_SND_SetSpdifPassThrough ErrCode 0x%08x\n",ErrCode));
		        return ErrCode;
		    }
			STTBX_Print(("STAUD_SetDigitalOutput(STAUD_DIGITAL_MODE_COMPRESSED)=%s\n", GetErrorText(ErrCode) ));
			break;
		case DRV_AUDIO_DIGITAL_MODE_PCM:
		default:
			ErrCode = HI_UNF_SND_SetHdmiPassThrough(HI_UNF_SND_0,HI_FALSE);
			ErrCode |= HI_UNF_SND_SetSpdifPassThrough(HI_UNF_SND_0, HI_FALSE);
		    if(ErrCode != DRV_NO_ERROR)
		    {
		        STTBX_Print(("drv_SetDigitalOutputMode : HI_UNF_SND_SetSpdifPassThrough ErrCode 0x%08x\n",ErrCode));
		        return ErrCode;
		    }
			STTBX_Print(("STAUD_SetDigitalOutput(STAUD_DIGITAL_MODE_NONCOMPRESSED)=%s\n", GetErrorText(ErrCode) ));
			break;
	}
	return ErrCode;
} 


 /*****************************************************************************
  Date&Time       : 2012-12-04
  Author          : whyu modify
  Function Name   : DRV_DENC_DisableVideo
  Description     : 关闭视频及图形层的显示输出，待机时使用
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode  DRV_DENC_DisableVideo(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
    HI_UNF_DISP_INTERFACE_S     DacMode;
    DacMode.bScartEnable = HI_FALSE;
    DacMode.bBt1120Enable = HI_FALSE;
    DacMode.bBt656Enable = HI_FALSE;
    DacMode.enDacMode[0] = HI_UNF_DISP_DAC_MODE_SILENCE;
    DacMode.enDacMode[1] = HI_UNF_DISP_DAC_MODE_SILENCE;
    DacMode.enDacMode[2] = HI_UNF_DISP_DAC_MODE_SILENCE;
    DacMode.enDacMode[3] = HI_UNF_DISP_DAC_MODE_SILENCE;
    DacMode.enDacMode[4] = HI_UNF_DISP_DAC_MODE_SILENCE;
    DacMode.enDacMode[5] = HI_UNF_DISP_DAC_MODE_SILENCE;
    ErrCode = HI_UNF_DISP_SetDacMode(&DacMode);
    if(ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_DENC_DisableVideo : HI_UNF_DISP_SetDacMode ErrCode 0x%08x\n",ErrCode));
        return ErrCode;
    }
    DRV_HdmiTerm();
	STTBX_Print(("DRV_DENC_DisableVideo : HI_UNF_DISP_SetDacMode ok!\n"));
	return ErrCode;
}


 /*****************************************************************************
  Date&Time       : 2012-12-04
  Author          : whyu modify
  Function Name   : DRV_DENC_EnableVideo
  Description     :打开视频及图形层的显示输出
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode  DRV_DENC_EnableVideo(void)
{
    DRV_ErrCode ErrCode=DRV_NO_ERROR;
    HI_UNF_DISP_INTERFACE_S     DacMode;
    DacMode.bScartEnable = HI_FALSE;
    DacMode.bBt1120Enable = HI_FALSE;
    DacMode.bBt656Enable = HI_FALSE;
    DacMode.enDacMode[0] = DACMODE0;
    DacMode.enDacMode[1] = DACMODE1;
    DacMode.enDacMode[2] = DACMODE2;
    DacMode.enDacMode[3] = DACMODE3;
    DacMode.enDacMode[4] = DACMODE4;
    DacMode.enDacMode[5] = DACMODE5;
    ErrCode = HI_UNF_DISP_SetDacMode(&DacMode);
    if(ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_DENC_EnableVideo : HI_UNF_DISP_SetDacMode ErrCode 0x%08x\n",ErrCode));
        return ErrCode;
    }
    DRV_HdmiInit();
	STTBX_Print(("DRV_DENC_EnableVideo : HI_UNF_DISP_SetDacMode ok!\n"));
    return ErrCode;
}


/*****************************************************************************
  Date&Time       : 2004-03-16
  Author          : yzb
  Function Name   : DRV_ErrCode DRV_VideoEnable(void)
  Description     : 打开视频层的显示输出 
  Input           : 
  Outut           : none 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_VideoEnable(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	HI_UNF_WINDOW_FREEZE_MODE_E  enWinFreezeMode;
	enWinFreezeMode = HI_UNF_WINDOW_FREEZE_MODE_LAST;
	m_OutputAlreadyEnabled=TRUE;
	ErrCode = HI_UNF_VO_SetWindowEnable(mAvPlayWinHandle,TRUE);
	if(ErrCode != DRV_NO_ERROR)
	{
        STTBX_Print(("DRV_VideoEnable : HI_UNF_VO_SetWindowEnable ErrCode 0x%08x\n",ErrCode));
	}
	else
	{
		STTBX_Print(("[DRV]: DRV_VideoEnable() Success!\n"));
	}

	return ErrCode;
	
}

BOOL DRV_VideoGetVisiable(void)
{
    if(m_OutputAlreadyEnabled == TRUE)
        return TRUE;
    else
        return FALSE;
}


/*****************************************************************************
  Date&Time       : 2004-03-16
  Author          : yzb
  Function Name   : DRV_ErrCode DRV_VideoDisable(void)
  Description     : 关闭视频层的显示输出 
  Input           : 
  Outut           : none 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_VideoDisable(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	HI_UNF_WINDOW_FREEZE_MODE_E  enWinFreezeMode;
	enWinFreezeMode = HI_UNF_WINDOW_FREEZE_MODE_BLACK;

	m_OutputAlreadyEnabled=FALSE_YS;
	
	ErrCode = HI_UNF_AVPLAY_Reset(mAvPlayHandle,NULL);
	ErrCode |= HI_UNF_VO_ResetWindow(mAvPlayWinHandle,enWinFreezeMode);
	ErrCode |= HI_UNF_VO_SetWindowEnable(mAvPlayWinHandle,FALSE);
	if(ErrCode != DRV_NO_ERROR)
	{
        STTBX_Print(("DRV_VideoDisable : HI_UNF_VO_SetWindowEnable ErrCode 0x%08x\n",ErrCode));
	}
	else
	{
		STTBX_Print(("[DRV]: DRV_VideoDisable() Success!\n"));
	}

	return ErrCode;

}


/*****************************************************************************
  Date&Time       : 2004-03-16
  Author          : yzb
  Function Name   : DRV_ErrCode DRV_AV_MuteAudio(void)
  Description     : Mute Audio 
  Input           : 
  Outut           : none 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AV_MuteAudio(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	ErrCode = HI_UNF_SND_SetMute(HI_UNF_SND_0,HI_TRUE);	
	if(ErrCode != DRV_NO_ERROR)
	{
        STTBX_Print(("DRV_AV_MuteAudio : HI_UNF_SND_SetMute ErrCode 0x%08x\n",ErrCode));
	}
	else
	{
		STTBX_Print(("[DRV]:DRV_AV_MuteAudio() Success\n"));
	}
	return ErrCode;
}


/*****************************************************************************
  Date&Time       : 2004-03-16
  Author          : yzb
  Function Name   : DRV_AV_UnMuteAudio
  Description     : UnMute Audio 
  Input           : 
  Outut           : none 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AV_UnMuteAudio(void)
{
	DRV_ErrCode	ErrCode = DRV_NO_ERROR;
	ErrCode = HI_UNF_SND_SetMute(HI_UNF_SND_0,HI_FALSE);	
	if(ErrCode != DRV_NO_ERROR)
	{
        STTBX_Print(("DRV_AV_UnMuteAudio : HI_UNF_SND_SetMute ErrCode 0x%08x\n",ErrCode));
	}	
	else
	{
		STTBX_Print(("[DRV]:DRV_AV_UnMuteAudio() Success\n"));
	}
	return ErrCode;	
}


/*****************************************************************************
  Date&Time       : 2004-03-16
  Author          : yzb
  Function Name   : DRV_DTV_ClearSlot
  Description     : 清除DTV设置的PID
  Input           : 
  Outut           : none 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_DTV_ClearSlot(U32 PTIInst)
{
	DRV_ErrCode ErrCode;
	return ErrCode;
}


/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_AV_SetAudio
  Description     : 
  Input           : volume
  Outut           : none 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AV_SetAudio ( U8_YS left, U8_YS right)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	U32 audioVolume;
	extern HI_HANDLE  mFfmpegPlayHandle;
	if (left > MAX_VOL_LEVEL)
		left = MAX_VOL_LEVEL;
	audioVolume = left;
	if (right > MAX_VOL_LEVEL)
		right = MAX_VOL_LEVEL;
	audioVolume = (MAX_VOL_LEVEL-right)*100/64;
	if(mFfmpegPlayHandle == 0)
	{
		ErrCode = HI_UNF_SND_SetVolume(HI_UNF_SND_0,audioVolume);
	}
	else
	{
		ErrCode = DRV_ffmpegSetAudio(audioVolume);
	}	
	if(ErrCode != DRV_NO_ERROR)
	{
        STTBX_Print(("DRV_AV_SetAudio : HI_UNF_SND_SetVolume ErrCode 0x%08x\n",ErrCode));
	}
	return ErrCode;
}



/*****************************************************************************
设置换台不黑屏,换台时保留上一频道的静祯
******************************************************************************/
void DRV_SetScreenNoBlack(void)
{
	m_DisableOutPutWhenVideoStop = FALSE;
}
void DRV_SetScreenBlack(void)
{
	m_DisableOutPutWhenVideoStop = TRUE;
}

BOOL DRV_IsScreenBlack(void)
{
	return m_DisableOutPutWhenVideoStop;
}


 /*****************************************************************************
  Date&Time       : 2012-12-04
  Author          : whyu modify
  Description     :设置是否NP制

*****************************************************************************/
void DRV_SetNPSwitchNotAuto(void)
{
	m_IsNPSwitchAuto = FALSE;
}
void DRV_SetNPSwitchAuto(void)
{
	m_IsNPSwitchAuto = TRUE;
}

/*****************************************************************************
  Date&Time       : 2004-03-16
  Author          : yzb
  Function Name   : DRV_AV_GetVideoWin
  Description     :   获取 Video 输入输出窗口位置和大小
  Input           : 
  Outut           : none 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AV_GetVideoWin(DRV_RectWh *pstRectIn,DRV_RectWh *pstRectOut)
{
	HI_UNF_WINDOW_ATTR_S pWinAttr;
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	DRV_OSDScreenMode_t Mode;
	DRV_RectWh RectIn;
	DRV_RectWh RectOut;
	extern HI_HANDLE  mFfmpegPlayHandle;
	HI_HANDLE PlayWinHandle;
	DRV_OSDGetScreenMode(&Mode);
	DRV_OSDMode SdMode;
	DRV_VtgTimingMode_t HdMode;
	DRV_OSDGetMode(&SdMode,&HdMode);
	PlayWinHandle = mAvPlayWinHandle;
	if(mFfmpegPlayHandle != 0)
	{
		PlayWinHandle = mFfmpegPlayHandle;
        ErrCode = HI_SVR_PLAYER_GetParam(PlayWinHandle, HI_SVR_PLAYER_ATTR_WINDOW_HDL, &PlayWinHandle);
		if (ErrCode != DRV_NO_ERROR)
		{
	        STTBX_Print(("DRV_AV_GetVideoWin : HI_SVR_PLAYER_GetParam ErrCode 0x%08x\n",ErrCode));
		}
    }    

	ErrCode = HI_UNF_VO_GetWindowAttr(PlayWinHandle,&pWinAttr);
	if (ErrCode != DRV_NO_ERROR)
	{
        STTBX_Print(("DRV_AV_GetVideoWin : HI_UNF_VO_GetWindowAttr ErrCode 0x%08x\n",ErrCode));
		return ErrCode;
	}
	
	if(pstRectIn != NULL)
	{
		RectIn.x = pWinAttr.stInputRect.s32X;
		RectIn.y = pWinAttr.stInputRect.s32Y;
		RectIn.nWidth  = pWinAttr.stInputRect.s32Width;
		RectIn.nHeight = pWinAttr.stInputRect.s32Height;
		if(Mode == OSD_SCREEN_MODE_1920_1080)
		{
			pstRectIn->x = RectIn.x;
			pstRectIn->y = RectIn.y;
			pstRectIn->nWidth  = RectIn.nWidth;
			pstRectIn->nHeight = RectIn.nHeight;
		}
		else if(Mode == OSD_SCREEN_MODE_1280_720)
		{
			pstRectIn->x = RectIn.x*1280/1920;
			pstRectIn->y = RectIn.y*720/1080;
			pstRectIn->nWidth  = RectIn.nWidth*1280/1920;
			pstRectIn->nHeight = RectIn.nHeight*720/1080;
		}
		else if(Mode == OSD_SCREEN_MODE_720_576)
		{
			pstRectIn->x = RectIn.x*720/1920;
			pstRectIn->y = RectIn.y*576/1080;
			pstRectIn->nWidth  = RectIn.nWidth*720/1920;
			pstRectIn->nHeight = RectIn.nHeight*576/1080;
		}
	}

	if(pstRectOut != NULL)
	{
		RectOut.x = pWinAttr.stOutputRect.s32X;
		RectOut.y = pWinAttr.stOutputRect.s32Y;
		RectOut.nWidth  = pWinAttr.stOutputRect.s32Width;
		RectOut.nHeight = pWinAttr.stOutputRect.s32Height;
		if(Mode == OSD_SCREEN_MODE_1920_1080)
		{
			pstRectOut->x = RectOut.x;
			pstRectOut->y = RectOut.y;
			pstRectOut->nWidth  = RectOut.nWidth;
			pstRectOut->nHeight = RectOut.nHeight;
		}
		else if(Mode == OSD_SCREEN_MODE_1280_720)
		{
			pstRectOut->x = RectOut.x*1280/1920;
			pstRectOut->y = RectOut.y*720/1080;
			pstRectOut->nWidth  = RectOut.nWidth*1280/1920;
			pstRectOut->nHeight = RectOut.nHeight*720/1080;
		}
		else if(Mode == OSD_SCREEN_MODE_720_576)
		{
			pstRectOut->x = RectOut.x*720/1920;
			pstRectOut->y = RectOut.y*576/1080;
			pstRectOut->nWidth  = RectOut.nWidth*720/1920;
			pstRectOut->nHeight = RectOut.nHeight*576/1080;
		}
	}

	return (DRV_NO_ERROR);
}


/*****************************************************************************
  Date&Time    :     2012-12-04
  Author          :    whyu modify
  Function Name   : DRV_AV_SetVideoWin
  Description     :  设置 Video 输入输出窗口位置和大小
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AV_SetVideoWin(DRV_RectWh *pstRectIn, DRV_RectWh *pstRectOut, BOOL AutoMode)
{
	HI_UNF_WINDOW_ATTR_S pWinAttr;
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	DRV_OSDScreenMode_t Mode;
	DRV_RectWh RectIn;
	DRV_RectWh RectOut;
	extern HI_HANDLE  mFfmpegPlayHandle;
	HI_HANDLE PlayWinHandle;
	DRV_OSDMode SdMode;
	DRV_VtgTimingMode_t HdMode;
	DRV_OSDGetScreenMode(&Mode);
	DRV_OSDGetMode(&SdMode,&HdMode);
	PlayWinHandle = mAvPlayWinHandle;
	if(mFfmpegPlayHandle != 0)
	{
		PlayWinHandle = mFfmpegPlayHandle;
		ErrCode = HI_SVR_PLAYER_GetParam(PlayWinHandle, HI_SVR_PLAYER_ATTR_WINDOW_HDL, &PlayWinHandle);
		if (ErrCode != DRV_NO_ERROR)
		{
	        STTBX_Print(("DRV_AV_GetVideoWin : HI_SVR_PLAYER_GetParam ErrCode 0x%08x\n",ErrCode));
		}
    }    

	ErrCode = HI_UNF_VO_GetWindowAttr(PlayWinHandle,&pWinAttr);
	if (ErrCode != DRV_NO_ERROR)
	{
        STTBX_Print(("DRV_AV_SetVideoWin : HI_UNF_VO_GetWindowAttr ErrCode 0x%08x\n",ErrCode));
		return ErrCode;
	}

	if(pstRectIn != NULL)
	{
		if(Mode == OSD_SCREEN_MODE_1920_1080)
		{
			RectIn.x       = pstRectIn->x;
			RectIn.y       = pstRectIn->y;
			RectIn.nWidth  = pstRectIn->nWidth;
			RectIn.nHeight = pstRectIn->nHeight;
		}
		else if(Mode == OSD_SCREEN_MODE_1280_720)
		{
			RectIn.x       = pstRectIn->x*1920/1280;
			RectIn.y       = pstRectIn->y*1080/720;
			RectIn.nWidth  = pstRectIn->nWidth*1920/1280;
			RectIn.nHeight = pstRectIn->nHeight*1080/720;
		}
		else if(Mode == OSD_SCREEN_MODE_720_576)
		{
			RectIn.x       = pstRectIn->x*1920/720;
			RectIn.y       = pstRectIn->y*1080/576;
			RectIn.nWidth  = pstRectIn->nWidth*1920/720;
			RectIn.nHeight = pstRectIn->nHeight*1080/576;
		}
		pWinAttr.stInputRect.s32X = RectIn.x;
		pWinAttr.stInputRect.s32Y = RectIn.y;
		pWinAttr.stInputRect.s32Width  = RectIn.nWidth;
		pWinAttr.stInputRect.s32Height = RectIn.nHeight;
	}

	if(pstRectOut != NULL)
	{
		if(Mode == OSD_SCREEN_MODE_1920_1080)
		{
			if((HdMode == DRV_DISPLAY_TIMING_MODE_720P_50)||(HdMode == DRV_DISPLAY_TIMING_MODE_720P_60))		
			{
				RectOut.x       = pstRectOut->x*1280/1920;
				RectOut.y       = pstRectOut->y*720/1080;
				RectOut.nWidth  = pstRectOut->nWidth*1280/1920;
				RectOut.nHeight = pstRectOut->nHeight*720/1080;
			}
			else
			{
			RectOut.x       = pstRectOut->x;
			RectOut.y       = pstRectOut->y;
			RectOut.nWidth  = pstRectOut->nWidth;
			RectOut.nHeight = pstRectOut->nHeight;
		}
		}
		else if(Mode == OSD_SCREEN_MODE_1280_720)
		{
			RectOut.x       = pstRectOut->x*1920/1280;
			RectOut.y       = pstRectOut->y*1080/720;
			RectOut.nWidth  = pstRectOut->nWidth*1920/1280;
			RectOut.nHeight = pstRectOut->nHeight*1080/720;
		}
		else if(Mode == OSD_SCREEN_MODE_720_576)
		{
			if((HdMode == DRV_DISPLAY_TIMING_MODE_720P_50)||(HdMode == DRV_DISPLAY_TIMING_MODE_720P_60))		
			{
				RectOut.x       = pstRectOut->x*1280/720;
				RectOut.y       = pstRectOut->y*720/576;
				RectOut.nWidth  = pstRectOut->nWidth*1280/720;
				RectOut.nHeight = pstRectOut->nHeight*720/576;
			}
			else
			{
				RectOut.x       = pstRectOut->x*1920/720;
				RectOut.y       = pstRectOut->y*1080/576;
				RectOut.nWidth  = pstRectOut->nWidth*1920/720;
				RectOut.nHeight = pstRectOut->nHeight*1080/576;
			}
		}
		pWinAttr.stOutputRect.s32X = RectOut.x;
		pWinAttr.stOutputRect.s32Y = RectOut.y;
		pWinAttr.stOutputRect.s32Width  = RectOut.nWidth;
		pWinAttr.stOutputRect.s32Height = RectOut.nHeight;
	}
	if(AutoMode)
	{
		RectOut.x       = 0;
		RectOut.y       = 0;
		RectOut.nWidth  = 1920;
		RectOut.nHeight = 1080;
		pWinAttr.stOutputRect.s32X = RectOut.x;
		pWinAttr.stOutputRect.s32Y = RectOut.y;
		pWinAttr.stOutputRect.s32Width  = RectOut.nWidth;
		pWinAttr.stOutputRect.s32Height = RectOut.nHeight;
	}
	ErrCode = HI_UNF_VO_SetWindowAttr(PlayWinHandle,&pWinAttr);
	if (ErrCode != DRV_NO_ERROR)
	{
        STTBX_Print(("DRV_AV_SetVideoWin : HI_UNF_VO_SetWindowAttr ErrCode 0x%08x\n",ErrCode));
		return ErrCode;
	}

	return (DRV_NO_ERROR);
}


/*****************************************************************************
  Date&Time    :     2012-12-04
  Author          :    whyu modify
  Function Name   : drv_VideoSetMode
  Description     :  设置视频显示制式
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode drv_VideoSetMode(DRV_VtgTimingMode_t mode)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
	switch(mode)
	{
	    case DRV_DISPLAY_TIMING_MODE_NTSC:
	    	ErrCode = HI_UNF_DISP_SetFormat(HI_UNF_DISP_SD0, HI_UNF_ENC_FMT_NTSC);  
	    	break;
	    case DRV_DISPLAY_TIMING_MODE_PAL:
	    	ErrCode = HI_UNF_DISP_SetFormat(HI_UNF_DISP_SD0, HI_UNF_ENC_FMT_PAL);  
	    	break;
	    case DRV_DISPLAY_TIMING_MODE_NTSC_443:
	    	HI_UNF_DISP_SetFormat(HI_UNF_DISP_SD0, HI_UNF_ENC_FMT_NTSC_J);  
	    	break;
	    case DRV_DISPLAY_TIMING_MODE_1080P_50:
	    	mEncFormat = HI_UNF_ENC_FMT_1080P_50;
	    	ErrCode = HI_UNF_DISP_SetFormat(HI_UNF_DISP_HD0, mEncFormat);  
	    	break;
	    case DRV_DISPLAY_TIMING_MODE_1080i_50:
	    	mEncFormat = HI_UNF_ENC_FMT_1080i_50;
	    	ErrCode = HI_UNF_DISP_SetFormat(HI_UNF_DISP_HD0, mEncFormat);  
	    	break;
	    case DRV_DISPLAY_TIMING_MODE_1080i_60:
	    	mEncFormat = HI_UNF_ENC_FMT_1080i_60;
	    	ErrCode = HI_UNF_DISP_SetFormat(HI_UNF_DISP_HD0, mEncFormat);  
	    	break;
	    case DRV_DISPLAY_TIMING_MODE_720P_50:
	    	mEncFormat = HI_UNF_ENC_FMT_720P_50;
	    	ErrCode = HI_UNF_DISP_SetFormat(HI_UNF_DISP_HD0, mEncFormat);  
	    	break;
	    case DRV_DISPLAY_TIMING_MODE_720P_60:
	    	mEncFormat = HI_UNF_ENC_FMT_720P_60;
	    	ErrCode = HI_UNF_DISP_SetFormat(HI_UNF_DISP_HD0, mEncFormat);  
	    	break;
	    case DRV_DISPLAY_TIMING_MODE_1080P_60:
	    default:
	    	mEncFormat = HI_UNF_ENC_FMT_1080P_60;
	    	ErrCode = HI_UNF_DISP_SetFormat(HI_UNF_DISP_HD0, mEncFormat);  
	    	break;
	}
    
	if (ErrCode != DRV_NO_ERROR)
	{
        STTBX_Print(("drv_VideoSetMode : HI_UNF_DISP_SetFormat ErrCode 0x%08x\n",ErrCode));
		return ErrCode;
	}

    return ErrCode;
}

/*****************************************************************************
  Date&Time    :     2012-12-04
  Author          :    whyu modify
  Description     :  设置是否使能杜比5.1输出

*****************************************************************************/
DRV_ErrCode DRV_EnableDolbyOutput(HI_BOOL bbhdmipassthrough,HI_BOOL bspdifpassthrough)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	m_IsEnableDolbyOutput = TRUE;
	
    if (HI_TRUE == bbhdmipassthrough)	 /*HDMI Port*/
	{
        HI_UNF_HDMI_SINK_CAPABILITY_S stSinkAttr;
        
        ErrCode = HI_UNF_HDMI_GetSinkCapability(HI_UNF_HDMI_ID_0, &stSinkAttr);

        if(HI_TRUE == stSinkAttr.bConnected)
        {
            HI_MPI_HIAO_SetSpdifCompatible(HI_FALSE);
            STTBX_Print(("========HDMI DRV_EnableDolbyOutput000 ErrCode 0x%08x\n",ErrCode));
        }
        else
    	{
            HI_MPI_HIAO_SetSpdifCompatible(HI_TRUE);
            STTBX_Print(("========HDMI DRV_EnableDolbyOutput111 ErrCode 0x%08x\n",ErrCode));
    	}
    	
    	if ((stSinkAttr.bAudioFmtSupported[2] == HI_TRUE)||(stSinkAttr.bAudioFmtSupported[10] == HI_TRUE))
    	{
            ErrCode = drv_SetDigitalOutputMode(DRV_AUDIO_DIGITAL_MODE_AC3);
    		if (HI_SUCCESS == ErrCode)
    		{
                STTBX_Print(("========HDMI DRV_EnableDolbyOutput222 ErrCode 0x%08x\n",ErrCode));
    		}
    	}
    	else
    	{
            ErrCode = drv_SetDigitalOutputMode(DRV_AUDIO_DIGITAL_MODE_PCM);
    		if (HI_SUCCESS == ErrCode)
    		{
                STTBX_Print(("========HDMI DRV_EnableDolbyOutput333 ErrCode 0x%08x\n",ErrCode));
    		}
    	}
    	
	}
	else if(HI_TRUE==bspdifpassthrough)	 /*SPDIF Port*/
	{
	ErrCode = drv_SetDigitalOutputMode(DRV_AUDIO_DIGITAL_MODE_AC3);
		if (HI_SUCCESS == ErrCode)
    {
            STTBX_Print(("========SPDIF DRV_EnableDolbyOutput ErrCode 0x%08x\n",ErrCode));
		}
		HI_MPI_HIAO_SetSpdifCompatible(HI_TRUE);
    }
	return ErrCode;
}

DRV_ErrCode DRV_DisableDolbyOutput(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	m_IsEnableDolbyOutput = FALSE;
	ErrCode = drv_SetDigitalOutputMode(DRV_AUDIO_DIGITAL_MODE_PCM);
    if(ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_DisableDolbyOutput : drv_SetDigitalOutputMode ErrCode 0x%08x\n",ErrCode));
        return ErrCode;
    }
	return ErrCode;
}
DRV_ErrCode DRV_GetAudioType(DRV_AudioType *AudStreamContent)
{
    *AudStreamContent = m_curAudStreamContent;

	return DRV_NO_ERROR;
}


/*****************************************************************************
  Date&Time    :     2012-12-04
  Author          :    whyu modify
  Function Name   : DRV_SetVoiceMode
  Description     :  设置声道
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode  DRV_SetVoiceMode(char  mode)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
	HI_UNF_TRACK_MODE_E enMode = HI_UNF_TRACK_MODE_STEREO;

    switch (mode)
    {
        case 0: 
        enMode = HI_UNF_TRACK_MODE_STEREO;
        break;
        case 1:
        enMode = HI_UNF_TRACK_MODE_DOUBLE_LEFT;
        break;
        case 2:
        enMode = HI_UNF_TRACK_MODE_DOUBLE_RIGHT;
        break;
        case 3:
        enMode = HI_UNF_TRACK_MODE_DOUBLE_MONO;
        break;
		default:
		enMode = HI_UNF_TRACK_MODE_STEREO;
		printf("\n**DRV_SetVoiceMode set wrong mode:%d!!!\n", mode);
		break;	
    }
	ErrCode = HI_UNF_SND_SetTrackMode(HI_UNF_SND_0,enMode);
    if(ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_SetVoiceMode : HI_UNF_SND_SetTrackMode ErrCode 0x%08x\n",ErrCode));
        return ErrCode;
    }
    return ErrCode;

}

void DRV_NewAudioFrameEvent(HI_HANDLE pHandle, HI_UNF_AVPLAY_EVENT_E enEvent, HI_U32 u32Para)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	U32 FrameRate = 25;
    HI_UNF_AO_FRAMEINFO_S *ao_stream_info;

	if(enEvent == HI_UNF_AVPLAY_EVENT_NEW_AUD_FRAME)
	{
        DBG_AudEvtCounter++;
	}
	else
	{
		STTBX_Print(("Warning!DRV Audio Event is 0x%x!\n", enEvent));
	}
	return;
}
/*****************************************************************************
  Date&Time       : 2004-03-16
  Author          : yzb
  Function Name   : DRV_AV_StopAudio
  Description     : 关闭音频
  Input           : 
  Outut           : 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode  DRV_AV_StopAudio(U32 PTIInst)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
    HI_UNF_AVPLAY_STOP_OPT_S    Stop;
    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    Stop.u32TimeoutMs = 0;

    ErrCode = HI_UNF_AVPLAY_UnRegisterEvent(mAvPlayHandle,HI_UNF_AVPLAY_EVENT_NEW_AUD_FRAME);
    ErrCode = HI_UNF_AVPLAY_Stop(mAvPlayHandle, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);
    if(ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_AV_StopAudio : HI_UNF_AVPLAY_Stop ErrCode 0x%08x\n",ErrCode));
    }
	else
	{	
		STTBX_Print(("DRV_AV_StopAudio OK!\n"));
	}
	m_bEnableAudio = FALSE_YS;
	return ErrCode;
}


 /*****************************************************************************
  Date&Time       : 2012-12-03
  Author          : whyu modify
  Function Name   : DRV_AV_StartAudio
  Description     : 开启音频播放
  Input           : StreamType，AudPID无效的参数
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AV_StartAudio(U32 PTIInst,STAUD_StreamType_t StreamType, STPTI_Pid_t AudPID,DRV_AudioType AudStreamContent)
{
    ST_ErrorCode_t ErrCode = DRV_NO_ERROR;
	HI_U32 enADecType;    

    if (m_bEnableAudio)
    {
        DRV_AV_StopAudio(PTIInst);
    }    

	switch(AudStreamContent)
	{
		case DRV_AUDIO_TYPE_AC3:
			enADecType = HA_AUDIO_ID_DOLBY_PLUS;
			m_AutoMode = DRV_AUDIO_TYPE_AC3;
			break;
		case DRV_AUDIO_TYPE_MPEG1:
            enADecType = HA_AUDIO_ID_MP2;
			m_AutoMode = DRV_AUDIO_TYPE_MPEG1;
			break;
		case DRV_AUDIO_TYPE_MPEG2:
			enADecType = HA_AUDIO_ID_MP2;
			m_AutoMode = DRV_AUDIO_TYPE_MPEG2;
			break;
		case DRV_AUDIO_TYPE_MP3:
			enADecType = HA_AUDIO_ID_MP3;
			m_AutoMode = DRV_AUDIO_TYPE_MP3;
			break;
		case DRV_AUDIO_TYPE_MPEG_AAC:
			enADecType = HA_AUDIO_ID_AAC;
			m_AutoMode = DRV_AUDIO_TYPE_MPEG_AAC;
			break;
		case DRV_AUDIO_TYPE_MPEG_HE_AAC:
			enADecType = HA_AUDIO_ID_AAC;
			m_AutoMode = DRV_AUDIO_TYPE_MPEG_HE_AAC;
			break;
		case DRV_AUDIO_TYPE_DOLBY_PLUS:
            enADecType = HA_AUDIO_ID_DOLBY_PLUS;
            m_AutoMode = DRV_AUDIO_TYPE_DOLBY_PLUS;
			break;
		default:
			return -1;
	}
    ErrCode = HIADP_AVPlay_SetAdecAttr(mAvPlayHandle,enADecType,HD_DEC_MODE_RAWPCM,1);
    if(ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_AV_StartAudio : HIADP_AVPlay_SetAdecAttr ErrCode 0x%08x\n",ErrCode));
        return ErrCode;
    }
    ErrCode = HI_UNF_AVPLAY_RegisterEvent(mAvPlayHandle, HI_UNF_AVPLAY_EVENT_NEW_AUD_FRAME, DRV_NewAudioFrameEvent);
    if(ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_AV_StartVideo : HI_UNF_AVPLAY_RegisterEvent ErrCode 0x%08x\n",ErrCode));
        return ErrCode;
    }
    DBG_AudEvtCounter = 0;
    ErrCode = HI_UNF_AVPLAY_Start(mAvPlayHandle, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if(ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_AV_StartAudio : HI_UNF_AVPLAY_Start ErrCode 0x%08x\n",ErrCode));
        return ErrCode;
    }

    m_bEnableAudio = TRUE;
    return ErrCode;
}
 

 /*****************************************************************************
  Date&Time       : 2012-12-03
  Author          : whyu modify
  Function Name   : DRV_AV_GetAudioType
  Description     : 获取音频格式
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AV_GetAudioType(DRV_AudioType *AudStreamContent)
{
    ST_ErrorCode_t ErrCode = DRV_NO_ERROR;  

    *AudStreamContent = m_AutoMode;
   
    return ErrCode;
}
 

 /*****************************************************************************
  Date&Time       : 2012-12-03
  Author          : whyu modify
  Function Name   : DRV_AV_StartAudioWithParam
  Description     : 开启音频播放
  Input           :AudStartParams无效的参数
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AV_StartAudioWithParam(U32 PTIInst,STAUD_StartParams_t AudStartParams,DRV_AudioType AudStreamContent)
{
    ST_ErrorCode_t ErrCode = DRV_NO_ERROR;
    HI_UNF_ACODEC_ATTR_S        AdecAttr;
	HI_U32 enADecType;    

    if (m_bEnableAudio)
    {
        DRV_AV_StopAudio(PTIInst);
    }    

	switch(AudStreamContent)
	{
		case DRV_AUDIO_TYPE_AC3:
			enADecType = HA_AUDIO_ID_DOLBY_PLUS;
			m_AutoMode = DRV_AUDIO_TYPE_AC3;
			break;
		case DRV_AUDIO_TYPE_MPEG1:
            enADecType = HA_AUDIO_ID_MP2;
			m_AutoMode = DRV_AUDIO_TYPE_MPEG1;
			break;
		case DRV_AUDIO_TYPE_MPEG2:
			enADecType = HA_AUDIO_ID_MP2;
			m_AutoMode = DRV_AUDIO_TYPE_MPEG2;
			break;
		case DRV_AUDIO_TYPE_MP3:
			enADecType = HA_AUDIO_ID_MP3;
			m_AutoMode = DRV_AUDIO_TYPE_MP3;
			break;
		case DRV_AUDIO_TYPE_MPEG_AAC:
			enADecType = HA_AUDIO_ID_AAC;
			m_AutoMode = DRV_AUDIO_TYPE_MPEG_AAC;
			break;
		case DRV_AUDIO_TYPE_MPEG_HE_AAC:
			enADecType = HA_AUDIO_ID_AAC;
			m_AutoMode = DRV_AUDIO_TYPE_MPEG_HE_AAC;
			break;
		case DRV_AUDIO_TYPE_DOLBY_PLUS:
            enADecType = HA_AUDIO_ID_DOLBY_PLUS;
            m_AutoMode = DRV_AUDIO_TYPE_DOLBY_PLUS;
			break;
		default:
			return -1;
	}
    ErrCode = HIADP_AVPlay_SetAdecAttr(mAvPlayHandle,enADecType,HD_DEC_MODE_RAWPCM,1);
    if(ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_AV_StartAudioWithParam : HIADP_AVPlay_SetAdecAttr ErrCode 0x%08x\n",ErrCode));
        return ErrCode;
    }
    ErrCode = HI_UNF_AVPLAY_Start(mAvPlayHandle, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if(ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_AV_StartAudioWithParam : HI_UNF_AVPLAY_Start ErrCode 0x%08x\n",ErrCode));
        return ErrCode;
    }

    m_bEnableAudio = TRUE;
    return ErrCode;
}
 

 /*****************************************************************************
  Date&Time       : 2012-12-03
  Author          : whyu modify
  Function Name   : DRV_AV_PauseAudio
  Description     :暂停AV播放，进入PAUSE状态
  Input           : PTIInst无效的参数
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AV_PauseAudio(U32 PTIInst)
{
    DRV_ErrCode          ErrCode = DRV_NO_ERROR;
	ErrCode = HI_UNF_AVPLAY_Pause(mAvPlayHandle, NULL);
	if (ErrCode != DRV_NO_ERROR)
	{
        STTBX_Print(("DRV_AV_PauseAudio : HI_UNF_AVPLAY_Pause ErrCode 0x%08x\n",ErrCode));
		return ErrCode;
	}
    return ErrCode;
}
 
 
  /*****************************************************************************
   Date&Time       : 2012-12-03
   Author          : whyu modify
   Function Name   : DRV_AV_ResumeAudio
   Description     : 恢复AV播放，进入PLAY状态
   Input           :PTIInst无效的参数
   Outut           :  
   Calls           : 
   Called by       :
   Global Variables: 
   Others          : 
 *****************************************************************************/
DRV_ErrCode DRV_AV_ResumeAudio(U32 PTIInst)
{
    DRV_ErrCode          ErrCode = DRV_NO_ERROR;
	ErrCode = HI_UNF_AVPLAY_Resume(mAvPlayHandle, NULL);
	if (ErrCode != DRV_NO_ERROR)
	{
        STTBX_Print(("DRV_AV_ResumeAudio : HI_UNF_AVPLAY_Resume ErrCode 0x%08x\n",ErrCode));
		return ErrCode;
	}
    return ErrCode;

}
  

 /*****************************************************************************
  Date&Time       : 2012-12-03
  Author          : whyu modify
  Function Name   : DRV_NewVideoFrameEvent
  Description     :新帧事件
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void DRV_NewVideoFrameEvent(HI_HANDLE pHandle, HI_UNF_AVPLAY_EVENT_E enEvent, HI_U32 u32Para)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	U32 FrameRate = 25;
    HI_UNF_VO_FRAMEINFO_S *vo_stream_info;
	HI_UNF_HDMI_ATTR_S     stHdmiAttr;
	static HI_UNF_VIDEO_FRAME_PACKING_TYPE_E FramePackingType = HI_UNF_FRAME_PACKING_TYPE_NONE;

	if(enEvent == HI_UNF_AVPLAY_EVENT_NEW_VID_FRAME)
	{
		DBG_VidEvtCounter++;
        vo_stream_info = (HI_UNF_VO_FRAMEINFO_S*)u32Para;
        {
			if(m_OutputAlreadyEnabled==FALSE)
			{
		    	m_VidNewEvtCounter++;
				
				if(FrameRate != vo_stream_info->stSeqInfo.u32FrameRate)
				{
					STTBX_Print(("New FRAME_RATE detected = %d\n", vo_stream_info->stSeqInfo.u32FrameRate));
					FrameRate = vo_stream_info->stSeqInfo.u32FrameRate;
				}
				if(m_IsNPSwitchAuto)
				{
					DTV_NewFrameToBeDispWithFrameRateChange(FrameRate);
				}
				if(m_VidNewEvtCounter == 1)
				{
				
					STTBX_Print(("DRV_VideoEnable() by NewFrameEvt, FR=%d\n",vo_stream_info->stSeqInfo.u32FrameRate));
					DRV_VideoEnable();
					m_VidNewEvtCounter = 0;
					m_OutputAlreadyEnabled=TRUE;
				}
				else if(m_VidNewEvtCounter > 1)
				{
					STTBX_Print(("DRV_VideoEnable() by NewFrameEvt, It's impossible happen,m_VidNewEvtCounter is 0x%x\n",m_VidNewEvtCounter));
				}
			}
		}
#if 0   /* 3D视频自动转换 */
        if(enEvent == HI_UNF_AVPLAY_EVENT_FRAMEPACKING_CHANGE)
        {
            vo_stream_info = (HI_UNF_VO_FRAMEINFO_S*)u32Para;
		
            if(vo_stream_info->enFramePackingType != FramePackingType)
            {
                FramePackingType = vo_stream_info->enFramePackingType;
                if(HI_UNF_FRAME_PACKING_TYPE_NONE == vo_stream_info->enFramePackingType)
                {
                    STTBX_Print(("***DRV_NewVideoFrameEvent normal frame, not a 3D frame\n"));
                    HI_UNF_HDMI_Stop(HI_UNF_HDMI_ID_0);
                    ErrCode = HI_UNF_HDMI_GetAttr(HI_UNF_HDMI_ID_0, &stHdmiAttr);
                    stHdmiAttr.b3DEnable = HI_FALSE;
                    stHdmiAttr.u83DParam = HI_FALSE;    //3//非3D格式
                    ErrCode = HI_UNF_HDMI_SetAttr(HI_UNF_HDMI_ID_0, &stHdmiAttr);
                    if (ErrCode != HI_SUCCESS)
                    {
                        STTBX_Print(("=======>>>>DRV_NewVideoFrameEvent : HI_UNF_HDMI_SetAttr NONE ErrCode 0x%08x\n",ErrCode));
                        return ErrCode;
                    }
                    if(OSD_getHDMIopen())
        			{
        				OSD_update_close_hdmi();
        			}
                    HI_UNF_HDMI_Start(HI_UNF_HDMI_ID_0);
                }
                else if(HI_UNF_FRAME_PACKING_TYPE_SIDE_BY_SIDE == vo_stream_info->enFramePackingType)
                {
                    HIGO_STEREO_MODE_E StereoMode;
    			    HIGO_ENCPICFRM_E EncPicFrm;
                    STTBX_Print(("***DRV_NewVideoFrameEvent is 3D side by side\n"));
                    HI_UNF_HDMI_Stop(HI_UNF_HDMI_ID_0);
                    ErrCode = HI_UNF_HDMI_GetAttr(HI_UNF_HDMI_ID_0, &stHdmiAttr);
                    stHdmiAttr.b3DEnable           = HI_TRUE;
                    stHdmiAttr.u83DParam             = 8;    //3//3D左右帧格式显示
                    ErrCode = HI_UNF_HDMI_SetAttr(HI_UNF_HDMI_ID_0, &stHdmiAttr);
                    if (ErrCode != HI_SUCCESS)
                    {
                        STTBX_Print(("=======>>>>DRV_NewVideoFrameEvent : HI_UNF_HDMI_SetAttr SIDE_BY_SIDE ErrCode 0x%08x\n",ErrCode));
                        return ErrCode;
                    }
                    StereoMode = HIGO_STEREO_MODE_HW_HALF;
    				EncPicFrm = HIGO_ENCPICFRM_STEREO_SIDEBYSIDE_HALF;
    				if(!OSD_getHDMIopen())
    					OSD_update_open_hdmi(StereoMode, EncPicFrm);
                    HI_UNF_HDMI_Start(HI_UNF_HDMI_ID_0);
                }
                else if(HI_UNF_FRAME_PACKING_TYPE_TOP_BOTTOM == vo_stream_info->enFramePackingType)
                {
                    HIGO_STEREO_MODE_E StereoMode;
        			HIGO_ENCPICFRM_E EncPicFrm;
                    STTBX_Print(("***DRV_NewVideoFrameEvent is 3D top bottom\n"));
                    HI_UNF_HDMI_Stop(HI_UNF_HDMI_ID_0);
                    ErrCode = HI_UNF_HDMI_GetAttr(HI_UNF_HDMI_ID_0, &stHdmiAttr);
                    stHdmiAttr.b3DEnable           = HI_TRUE;
                    stHdmiAttr.u83DParam             = 6;    //3//3D上下帧格式显示
                    ErrCode = HI_UNF_HDMI_SetAttr(HI_UNF_HDMI_ID_0, &stHdmiAttr);
                    if (ErrCode != HI_SUCCESS)
                    {
                        STTBX_Print(("=======>>>>DRV_NewVideoFrameEvent : HI_UNF_HDMI_SetAttr TOP_BOTTOM ErrCode 0x%08x\n",ErrCode));
                        return ErrCode;
                    }
                    StereoMode = HIGO_STEREO_MODE_HW_HALF;
    				EncPicFrm = HIGO_ENCPICFRM_STEREO_TOPANDBOTTOM;
    				if(!OSD_getHDMIopen())
    					OSD_update_open_hdmi(StereoMode, EncPicFrm);
                    HI_UNF_HDMI_Start(HI_UNF_HDMI_ID_0);
                }
                else
                {
                    STTBX_Print(("=======>>>>DRV_NewVideoFrameEvent 2D&3D event none!\n"));
                }
            }
            else
            {
                STTBX_Print(("[DRV]Video FramePackingType is 0x%x!\n", FramePackingType));
            }
        }
#endif
	}
	else
	{
		STTBX_Print(("Warning!DRV Video Event is 0x%x!\n", enEvent));
	}
	return;
}
   

 /*****************************************************************************
  Date&Time       : 2012-12-03
  Author          : whyu modify
  Function Name   : DRV_AV_StopVideo
  Description     :关闭视频
  Input           : PTIInst，DisableOutputWindow无效参数
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AV_StopVideo(U32 PTIInst, BOOL DisableOutputWindow)
{
    DRV_ErrCode	ErrCode = DRV_NO_ERROR;
    HI_UNF_AVPLAY_STOP_OPT_S    Stop;
    if(m_DisableOutPutWhenVideoStop == TRUE)
    {
    	Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    }
    else
    {
    	Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_STILL;
    }	
    Stop.u32TimeoutMs = 0;
	if (m_VidNewEvtCallBackInit[m_curVidSDecodeChannel])
	{
		ErrCode = HI_UNF_AVPLAY_UnRegisterEvent(mAvPlayHandle,HI_UNF_AVPLAY_EVENT_NEW_VID_FRAME);
		m_VidNewEvtCallBackInit[m_curVidSDecodeChannel] = FALSE;
	}

    ErrCode = HI_UNF_AVPLAY_Stop(mAvPlayHandle, HI_UNF_AVPLAY_MEDIA_CHAN_VID, &Stop);
	if (ErrCode != DRV_NO_ERROR)
	{
        STTBX_Print(("DRV_AV_StopVideo : HI_UNF_AVPLAY_Stop ErrCode 0x%08x\n",ErrCode));
		return ErrCode;
	}
	m_OutputAlreadyEnabled=FALSE_YS;
	m_bEnableVideo = FALSE;
    return ErrCode;
}
    
 
  /*****************************************************************************
   Date&Time       : 2012-12-03
   Author          : whyu modify
   Function Name   : DRV_AV_StartVideo
   Description     :打开视频
   Input           : PTIInst，StreamType无效参数
   Outut           :  
   Calls           : 
   Called by       :
   Global Variables: 
   Others          : 
 *****************************************************************************/
DRV_ErrCode DRV_AV_StartVideo(U32 PTIInst, STVID_StreamType_t StreamType,DRV_VideoType VidStreamContent)
{
    DRV_ErrCode	ErrCode = DRV_NO_ERROR;
	HI_UNF_VCODEC_TYPE_E enType;

	switch(VidStreamContent)
	{
        case DRV_VIDEO_TYPE_MP1V:
		case DRV_VIDEO_TYPE_MP2V:
			enType = HI_UNF_VCODEC_TYPE_MPEG2;
			break;
		case DRV_VIDEO_TYPE_MPEG4P2:
			enType = HI_UNF_VCODEC_TYPE_MPEG4;
			break;
		case DRV_VIDEO_TYPE_H264:
			enType = HI_UNF_VCODEC_TYPE_H264;
			break;	
		case DRV_VIDEO_TYPE_AVS:
			enType = HI_UNF_VCODEC_TYPE_AVS;
			break;	
		default:
			printf("Stream type is not supported.\n");
			return -1;
	}
	ErrCode = HIADP_AVPlay_SetVdecAttr(mAvPlayHandle,enType,HI_UNF_VCODEC_MODE_NORMAL); 	
	if (ErrCode != DRV_NO_ERROR)
	{
        STTBX_Print(("DRV_AV_StartVideo : HIADP_AVPlay_SetVdecAttr ErrCode 0x%08x\n",ErrCode));
		return ErrCode;
	}

	if (m_VidNewEvtCallBackInit[m_curVidSDecodeChannel])
	{
		ErrCode = HI_UNF_AVPLAY_UnRegisterEvent(mAvPlayHandle,HI_UNF_AVPLAY_EVENT_NEW_VID_FRAME);
		m_VidNewEvtCallBackInit[m_curVidSDecodeChannel] = FALSE;
	}
    ErrCode = HI_UNF_AVPLAY_RegisterEvent(mAvPlayHandle, HI_UNF_AVPLAY_EVENT_NEW_VID_FRAME, DRV_NewVideoFrameEvent);
    if(ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_AV_StartVideo : HI_UNF_AVPLAY_RegisterEvent ErrCode 0x%08x\n",ErrCode));
        return ErrCode;
    }
	m_VidNewEvtCallBackInit[m_curVidSDecodeChannel] = TRUE;
	DBG_VidEvtCounter=0;
	m_VidNewEvtCounter=0;
	ErrCode = HI_UNF_AVPLAY_Start(mAvPlayHandle, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
	if (ErrCode != DRV_NO_ERROR)
	{
        STTBX_Print(("DRV_AV_StartVideo : HI_UNF_AVPLAY_Start ErrCode 0x%08x\n",ErrCode));
		return ErrCode;
	}
    m_OutputAlreadyEnabled=FALSE;
    m_bEnableVideo = TRUE;
    return ErrCode;
}


 /*****************************************************************************
                   无效的接口
*****************************************************************************/
DRV_ErrCode DRV_AV_SetDecodedPictures(DRV_DecodedPictures_t type)
{
	DRV_ErrCode	ErrCode;
	return 0;
}
    
 
  /*****************************************************************************
   Date&Time       : 2012-12-03
   Author          : whyu modify
   Function Name   : DRV_AV_StartVideoWithParam
   Description     :打开视频
   Input           : PTIInst，VidStartParams无效参数
   Outut           :  
   Calls           : 
   Called by       :
   Global Variables: 
   Others          : 
 *****************************************************************************/
DRV_ErrCode DRV_AV_StartVideoWithParam(U32 PTIInst, STVID_StartParams_t VidStartParams,DRV_VideoType VidStreamContent)
{
    DRV_ErrCode	ErrCode = DRV_NO_ERROR;
	HI_UNF_VCODEC_TYPE_E enType;

	switch(VidStreamContent)
	{
        case DRV_VIDEO_TYPE_MP1V:
		case DRV_VIDEO_TYPE_MP2V:
			enType = HI_UNF_VCODEC_TYPE_MPEG2;
			break;
		case DRV_VIDEO_TYPE_MPEG4P2:
			enType = HI_UNF_VCODEC_TYPE_MPEG4;
			break;
		case DRV_VIDEO_TYPE_H264:
			enType = HI_UNF_VCODEC_TYPE_H264;
			break;	
		case DRV_VIDEO_TYPE_AVS:
			enType = HI_UNF_VCODEC_TYPE_AVS;
			break;			
		default:
			printf("Stream type is not supported.\n");
			return -1;
	}
	ErrCode = HIADP_AVPlay_SetVdecAttr(mAvPlayHandle,enType,HI_UNF_VCODEC_MODE_NORMAL); 	
	if (ErrCode != DRV_NO_ERROR)
	{
        STTBX_Print(("DRV_AV_StartVideoWithParam : HIADP_AVPlay_SetVdecAttr ErrCode 0x%08x\n",ErrCode));
		return ErrCode;
	}
	if (m_VidNewEvtCallBackInit[m_curVidSDecodeChannel])
	{
		ErrCode = HI_UNF_AVPLAY_UnRegisterEvent(mAvPlayHandle,HI_UNF_AVPLAY_EVENT_NEW_VID_FRAME);
		m_VidNewEvtCallBackInit[m_curVidSDecodeChannel] = FALSE;
	}
    ErrCode = HI_UNF_AVPLAY_RegisterEvent(mAvPlayHandle, HI_UNF_AVPLAY_EVENT_NEW_VID_FRAME, DRV_NewVideoFrameEvent);
    if(ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_AV_StartVideo : HI_UNF_AVPLAY_RegisterEvent ErrCode 0x%08x\n",ErrCode));
        return ErrCode;
    }
	m_VidNewEvtCallBackInit[m_curVidSDecodeChannel] = TRUE;
	DBG_VidEvtCounter=0;
	m_VidNewEvtCounter=0;
	ErrCode = HI_UNF_AVPLAY_Start(mAvPlayHandle, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
	if (ErrCode != DRV_NO_ERROR)
	{
        STTBX_Print(("DRV_AV_StartVideoWithParam : HI_UNF_AVPLAY_Start ErrCode 0x%08x\n",ErrCode));
		return ErrCode;
	}
    m_OutputAlreadyEnabled=FALSE;
    m_bEnableVideo = TRUE;
    return ErrCode;
}


/*****************************************************************************
  Date&Time       : 2004-03-16
  Author          : yzb
  Function Name   : DRV_Pause_Video
  Description     :  暂停视频输出
  Input           : 
  Outut           : none 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AV_PauseVideo(VID_DeviceId_t VidDeviceId)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;

	ErrCode = HI_UNF_AVPLAY_Pause(mAvPlayHandle, NULL);
	if (ErrCode != DRV_NO_ERROR)
	{
        STTBX_Print(("DRV_AV_PauseVideo : HI_UNF_AVPLAY_Pause ErrCode 0x%08x\n",ErrCode));
		return ErrCode;
	}
	return ErrCode;
}
   

 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_AV_ResumeVideo
  Description     :恢复AV播放，进入PLAY状态
  Input           : VidDeviceId无效参数,设为0即可
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AV_ResumeVideo(VID_DeviceId_t VidDeviceId)
{
    DRV_ErrCode  ErrCode = DRV_NO_ERROR;
	ErrCode = HI_UNF_AVPLAY_Resume(mAvPlayHandle,NULL);
	if (ErrCode != DRV_NO_ERROR)
	{
        STTBX_Print(("DRV_AV_ResumeVideo : HI_UNF_AVPLAY_Resume ErrCode 0x%08x\n",ErrCode));
		return ErrCode;
	}
    return ErrCode;
}


 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_AV_GetDmxStatus
  Description     :获取DMX通道的状态
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AV_GetDmxStatus(BOOL *VidValid,BOOL *AudValid,U32 TimeMs)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	HI_HANDLE phDmxAudChn,phDmxVidChn;
	HI_HANDLE phWatchChannel[2] ={0};
	HI_HANDLE phDataChannel[2] ={0};
	HI_U32    pu32ChNum = 0;
	HI_U32    u32WatchNum = 0;
	HI_U32    index;
	ErrCode = HI_UNF_AVPLAY_GetDmxAudChnHandle(mAvPlayHandle, &phDmxAudChn); 
 	if (ErrCode != DRV_NO_ERROR)
	{
        STTBX_Print(("DRV_AV_GetTsPackageStatus : HI_UNF_AVPLAY_GetDmxAudChnHandle ErrCode 0x%08x\n",ErrCode));
	}
	else
	{
		phWatchChannel[u32WatchNum] = phDmxAudChn;
		u32WatchNum ++;
	}

	ErrCode = HI_UNF_AVPLAY_GetDmxVidChnHandle(mAvPlayHandle, &phDmxVidChn);
	if (ErrCode != DRV_NO_ERROR)
	{
        STTBX_Print(("DRV_AV_GetTsPackageStatus : HI_UNF_AVPLAY_GetDmxVidChnHandle ErrCode 0x%08x\n",ErrCode));
	}
	else
	{
		phWatchChannel[u32WatchNum] = phDmxVidChn;
		u32WatchNum ++;
	}
	*VidValid = FALSE;
	*AudValid = FALSE;
	if(u32WatchNum > 0)
	{
		ErrCode = HI_UNF_DMX_SelectDataHandle(phWatchChannel,u32WatchNum,phDataChannel,&pu32ChNum,TimeMs);
		if (ErrCode != DRV_NO_ERROR)
		{
	        STTBX_Print(("DRV_AV_GetTsPackageStatus : HI_UNF_DMX_SelectDataHandle ErrCode 0x%08x\n",ErrCode));
		}
		if(pu32ChNum > 0)
		{
			for(index=0;index<pu32ChNum;index++)
			{
				if(phDataChannel[index] == phDmxAudChn)
				{
					*AudValid = TRUE;
				}
				else if(phDataChannel[index] == phDmxVidChn)
				{
					*VidValid = TRUE;
				}
				else
				{
	        		STTBX_Print(("DRV_AV_GetTsPackageStatus : should not happen\n"));
				}
			}
		}
	}

	return DRV_NO_ERROR;
}
 
 
  /*****************************************************************************
   Date&Time       : 2012-12-07
   Author          : whyu modify
   Function Name   : DRV_AVAPI_DTVResume
   Description     :无效的接口
 *****************************************************************************/
void DRV_AVAPI_DTVResume(STVID_StreamType_t StreamType,STPTI_Pid_t VidPID)
{
	DRV_ErrCode ErrCode;
    return;
}


 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_AVAPI_AVstart
  Description     :标清节目播放接口，音视频同步
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AVAPI_AVstart(U32 decode_channel,
								   U32 ptiToUse, 
	 							   STPTI_Pid_t VidPID, 
								   STPTI_Pid_t AudPID, 
								   STPTI_Pid_t PcrPID, 
								   STPTI_Pid_t TtxPID, 
								   STPTI_Pid_t SubtPID)

{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	DRV_AudioType AudStreamContent = DRV_AUDIO_TYPE_MPEG1;
	DRV_VideoType VidStreamContent = DRV_VIDEO_TYPE_MP2V;   
	/* Start configuring AV for the new channel to be shown */
	STTBX_Print(("[DRV]:AVStart(Vid=%d,Aud=%d,Pcr=%d)\n",VidPID,AudPID,PcrPID));
	ErrCode = DRV_AVAPI_AVstart_HD(decode_channel,
								   ptiToUse, 
	 							   VidPID, 
								   AudPID, 
								   PcrPID, 
								   TtxPID, 
								   SubtPID,
								   AudStreamContent,
								   VidStreamContent);
    if(ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_AVAPI_AVstart Error %d\n",ErrCode));
        return ErrCode;
    }
	return ErrCode;

}
 
 
  /*****************************************************************************
   Date&Time       : 2012-12-07
   Author          : whyu modify
   Function Name   : DRV_AVAPI_AVstart_HD
   Description     :高清节目播放接口，音视频同步
   Input           : 
   Outut           :  
   Calls           : 
   Called by       :
   Global Variables: 
   Others          : 
 *****************************************************************************/
DRV_ErrCode DRV_AVAPI_AVstart_HD(U32 decode_channel,
								   U32 ptiToUse, 
	 							   STPTI_Pid_t VidPID, 
								   STPTI_Pid_t AudPID, 
								   STPTI_Pid_t PcrPID, 
								   STPTI_Pid_t TtxPID, 
								   STPTI_Pid_t SubtPID,
								   DRV_AudioType AudStreamContent,
								   DRV_VideoType VidStreamContent)

{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
    
	STTBX_Print(("DRV_AVAPI_AVstart_HD(decode_channel=%d,ptiToUse=%d,Vid=%d,Aud=%d,Pcr=%d,AudStreamContent=%d,VidStreamContent=%d)\n",decode_channel,ptiToUse,VidPID,AudPID,PcrPID,AudStreamContent,VidStreamContent));
    
	drv_videoInjectClaim();
	DRV_AvSynModeSet(DRV_AVSYN_AUD_PTS);	

	m_curAudStreamContent = AudStreamContent;

	/*Pcr start*/
	if((PcrPID == STPTI_InvalidPid())||(PcrPID == STPTI_NullPid()))
	{
        STTBX_Print(("DRV_AVAPI_AVstart_HD Pcr pid invalid\n"));
    	m_curPcrPid = STPTI_InvalidPid();
	}
	else
	{
		if((m_DecodeCurAvSynMode[m_curVidSDecodeChannel] != DRV_AVSYN_PCR)&&(m_DecodeCurAvSynMode[m_curVidSDecodeChannel] != DRV_AVSYN_NONE_WITH_PCR))/*AUD,VID PTS同步是否需要收?*/
		{
			//PcrPID = STPTI_InvalidPid();
			//ErrCode = drv_PtiPcrStop(ptiToUse);
			//if(ErrCode != ST_NO_ERROR)
			//{
			//	STTBX_Print(("Error in drv_PtiPcrStop\n"));
			//}
			STTBX_Print(("DRV_AVAPI_AVstart_HD Syn mode not use PCR\n"));
		}
		else
		{
			ErrCode = drv_PtiPcrStart(ptiToUse,PcrPID);
			if(ErrCode != DRV_NO_ERROR)
			{
				STTBX_Print(("Error in PTI_PcrStart\n"));
			}
			m_bEnablePCR = TRUE;
		}	
	}
    
#ifdef DRV_TRANSCODER_SERVER
    ErrCode = DRV_Transcoder_Reset();
    if(ErrCode != DRV_NO_ERROR)
    {
        printf("DRV_Transcoder_Reset Error!!!");
    }
#endif   

	/*audio start*/
	if((AudPID == STPTI_InvalidPid())||(AudPID == STPTI_NullPid()))
	{
		STTBX_Print(("DRV_AVAPI_AVstart_HD Aud pid invalid\n"));
		m_curAudPid = STPTI_InvalidPid();
	}
	else
	{
		if(AudPID != m_curAudPid)
		{
			ErrCode = drv_PtiAudioStart(ptiToUse, AudPID);
			ErrCode |= DRV_AV_StartAudio(ptiToUse,0,AudPID,AudStreamContent);
			if(ErrCode != DRV_NO_ERROR)
			{
				STTBX_Print(("DRV_AVAPI_AVstart_HD DRV_AV_StartVideo Error %d\n",ErrCode));
	            //drv_videoInjectRelease();
				//return ST_ERROR_BAD_PARAMETER;
			}
		}
	}
    
	/*video start*/
	if((VidPID == STPTI_InvalidPid())||(VidPID == STPTI_NullPid()))
	{
		STTBX_Print(("DRV_AVAPI_AVstart_HD Vid pid invalid\n"));
		m_curVidPid = STPTI_InvalidPid();
	}
	else
	{
		if(VidPID != m_curVidPid)
		{
			ErrCode = drv_PtiVideoStart(ptiToUse, VidPID);
			ErrCode |= DRV_AV_StartVideo(ptiToUse, 0,VidStreamContent);
			if(ErrCode != DRV_NO_ERROR)
			{
				STTBX_Print(("DRV_AVAPI_AVstart_HD DRV_AV_StartVideo Error %d\n",ErrCode));
				return ErrCode;
			}
		}
	}
    
	drv_videoInjectRelease();

	//绑定播放音视频解扰器
	CA_AssociateDescrambleKey(PLAY_VIDEO_DESCRAMBLE_TYPE,VidPID);
	CA_AssociateDescrambleKey(PLAY_AUDIO_DESCRAMBLE_TYPE,AudPID);
	
	return DRV_NO_ERROR;

}
   
 
  /*****************************************************************************
   Date&Time       : 2012-12-07
   Author          : whyu modify
   Function Name   : DRV_AVAPI_AVStatus_HD
   Description     :获取当前节目音视频及PCR的PID
   Input           : 
   Outut           :  
   Calls           : 
   Called by       :
   Global Variables: 
   Others          : 
 *****************************************************************************/
DRV_ErrCode DRV_AVAPI_AVStatus_HD(U32 *decode_channel,
								   U32 *ptiToUse, 
	 							   STPTI_Pid_t *VidPID, 
								   STPTI_Pid_t *AudPID, 
								   STPTI_Pid_t *PcrPID, 
								   STPTI_Pid_t *TtxPID, 
								   STPTI_Pid_t *SubtPID)

{
	*decode_channel = 0;
	*ptiToUse = 0;
	*VidPID = m_curVidPid;
	*AudPID = m_curAudPid;
	*PcrPID = m_curPcrPid;
	*TtxPID = 0;
	*SubtPID = 0;
	return DRV_NO_ERROR;
}


/*******************************************************************************
Name        : S R A V C _ A V s t o p 
Description : <$functionDescription>
Parameters  : 
Assumptions :
Limitations : 
Returns     : (ST_ErrorCode_t) <$returnDescription>
*******************************************************************************/
DRV_ErrCode DRV_AVAPI_AVstop(U32 decode_channel)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	ErrCode = DRV_AVAPI_AVstop_HD(decode_channel,0);
    if(ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_AVAPI_AVstop ErrCode 0x%08x\n",ErrCode));
        return ErrCode;
    }
	return ErrCode;
}


 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_AVAPI_AVstop_HD
  Description     :停止高清节目播放接口
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AVAPI_AVstop_HD(U32 decode_channel,U32 PTIInst)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	DRV_AV_MuteAudio();

	//解绑播放音视频解扰器
	CA_DisAssociateDescrambleKey(PLAY_VIDEO_DESCRAMBLE_TYPE);
	CA_DisAssociateDescrambleKey(PLAY_AUDIO_DESCRAMBLE_TYPE);
	
	drv_videoInjectClaim();
	if(m_bEnableVideo && (m_curVidPid != STPTI_InvalidPid() )) 
	{
		ErrCode = DRV_AV_StopVideo(PTIInst, m_DisableOutPutWhenVideoStop);
		ErrCode |= drv_PtiVideoStop(PTIInst);
	}
    if(m_bEnableAudio && (m_curAudPid != STPTI_InvalidPid() ))
    {
        ErrCode = DRV_AV_StopAudio(PTIInst);
		ErrCode |= drv_PtiAudioStop(PTIInst);
    }
	drv_videoInjectRelease();
	if(m_bEnablePCR && (m_curPcrPid != STPTI_InvalidPid() ))
    {
        ErrCode = drv_PtiPcrStop(PTIInst);
        m_bEnablePCR=FALSE;
    }
	return ErrCode;
}

/*****************************************************************************
The following formula is used to adjust the luminance intensity of the display
video image: YOUT = YIN + B - 128
Where YIN is 8-bit input luminance and YOUT is the result of brightness operation
(still on 8 bits). This value is saturated at 235 (16) or 254 (1) according to 
DEN_CFG6 bit MAXDYN, B: brightness (unsigned value with center at 128, default 128).
可设定值 from  -16 to 16。
*******************************************************************************/
DRV_ErrCode DRV_DENC_SetDefaultValue(void)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
    U32 value1,value2,value3;
	ErrCode = HI_UNF_DISP_GetBrightness(HI_UNF_DISP_SD0,&value1);
	ErrCode |= HI_UNF_DISP_GetContrast(HI_UNF_DISP_SD0,&value2);
	ErrCode |= HI_UNF_DISP_GetSaturation(HI_UNF_DISP_SD0,&value3);
    if(ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_DENC_SetDefaultValue ErrCode 0x%08x\n",ErrCode));
        return ErrCode;
    }
    STTBX_Print(("DENC Dfault Value(Delay:0x%x, Brigthness:%d, Contrast:%d, Saturation:%d)\n",DENCDelayDefaultValue,\
		value1,value2,value3));
	return ErrCode;
}
/*****************************************************************************
The following formula is used to adjust the luminance intensity of the display
video image: YOUT = YIN + B - 128
Where YIN is 8-bit input luminance and YOUT is the result of brightness operation
(still on 8 bits). This value is saturated at 235 (16) or 254 (1) according to 
DEN_CFG6 bit MAXDYN, B: brightness (unsigned value with center at 128, default 128).
可设定值 from  -16 to 16。
*******************************************************************************/
DRV_ErrCode DRV_BrightAdjust(S8_YS Brightness_Offset)
{
    DRV_ErrCode ErrCode=DRV_NO_ERROR;

    if (Brightness_Offset< -DENC_OUT_ADJUST_RANGE)
    {
    	Brightness_Offset = -DENC_OUT_ADJUST_RANGE;
    }
    else if (Brightness_Offset>DENC_OUT_ADJUST_RANGE)
    {
    	Brightness_Offset = DENC_OUT_ADJUST_RANGE;
    }

	ErrCode = HI_UNF_DISP_SetBrightness(HI_UNF_DISP_SD0,Brightness_Offset+DENCBrightnessDefaultValue);
    if(ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_BrightAdjust :HI_UNF_DISP_SetBrightness ErrCode 0x%08x\n",ErrCode));
        return ErrCode;
    }
	STTBX_Print(("Set Bright Adjust is:%d,The Denc Value is:%d\n",Brightness_Offset,(DENCBrightnessDefaultValue+Brightness_Offset)));
	return ErrCode;

}

/*****************************************************************************
The following formula is used to adjust the relative difference between the 
display image higher and lower intensity luminance values:
YOUT = (YIN - 128) (C + 128)/128 + 128
Where, YIN is 8-bit input luminance, YOUT is the result of contrast operation
(still on 8 bits). This value is saturated at 235 (16) or 254 (1) according to 
DEN_CFG6 bit MAXDYN, C: contrast (two’s complement value from -128 to 127, default 0).
可设定值 from  -16 to 16。
*******************************************************************************/
DRV_ErrCode DRV_ContraAdjust(S8_YS Contrast_Offset)
{
    DRV_ErrCode ErrCode=DRV_NO_ERROR;

    if (Contrast_Offset<-DENC_OUT_ADJUST_RANGE)
    {
    	Contrast_Offset = -DENC_OUT_ADJUST_RANGE;
    }
    else if (Contrast_Offset>DENC_OUT_ADJUST_RANGE)
    {
    	Contrast_Offset = DENC_OUT_ADJUST_RANGE;
    }

	ErrCode = HI_UNF_DISP_SetContrast(HI_UNF_DISP_SD0,Contrast_Offset+DENCContrastDefaultValue);
    if(ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_ContraAdjust :HI_UNF_DISP_SetContrast ErrCode 0x%08x\n",ErrCode));
        return ErrCode;
    }
	STTBX_Print(("Set CONTR Adjust is:%d,The Denc Value is:%d\n",Contrast_Offset,DENCContrastDefaultValue+Contrast_Offset));

	return ErrCode;
	
}

/*****************************************************************************
The following formula is used to adjust the color intensity of the displayed 
video image:
Crout = S(Crin-128)/128 + 128
Cbout = S(Cbin-128)/128 + 128
Where Crin and Cbin are the 8-bit input chroma, Crout and Cbout are the result
of saturation operation (still on 8 bits).
This value is saturated at 240 (16) or 254 (1) according to DEN_CFG6 bit MAXDYN,
S: saturation value (unsigned value with centre at 128, default 128).
可设定值 from  -16 to 16。
*******************************************************************************/
DRV_ErrCode DRV_SaturaAdjust(S8_YS Saturation_Offset)
{	
    DRV_ErrCode ErrCode=DRV_NO_ERROR;

    if (Saturation_Offset<-DENC_OUT_ADJUST_RANGE)
    {
    	Saturation_Offset = -DENC_OUT_ADJUST_RANGE;
    }
    else if (Saturation_Offset>DENC_OUT_ADJUST_RANGE)
    {
    	Saturation_Offset = DENC_OUT_ADJUST_RANGE;
    }

	ErrCode = HI_UNF_DISP_SetSaturation(HI_UNF_DISP_SD0,Saturation_Offset+DENCSaturationDefaultValue);
    if(ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_SaturaAdjust :HI_UNF_DISP_SetSaturation ErrCode 0x%08x\n",ErrCode));
        return ErrCode;
    }
	STTBX_Print(("Set Satura Adjust is:%d,The Denc Value is:%d\n",Saturation_Offset,(DENCSaturationDefaultValue+Saturation_Offset)));

	return ErrCode;
	
}

/***************************************************
  Date&Time       : 2012-07-02
  Author          :   wrwei
  Description     : 复位AV播放
****************************************************/
DRV_ErrCode DRV_AV_Reset(void)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;

	ErrCode = HI_UNF_AVPLAY_Reset(mAvPlayHandle, NULL);
    if(ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_AV_Reset :HI_UNF_AVPLAY_Reset ErrCode 0x%08x\n",ErrCode));
        return ErrCode;
    }

    return ErrCode;
}

