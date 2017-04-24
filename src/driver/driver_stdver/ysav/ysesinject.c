/******************************************************************************
 *
 * 文件名 : ysesinject.c
 *
 * 描述 : es注入
 *
 * 创建人 :whyu
 *
 * 版本 :2012.12.07
 *
 *
 *
 *****************************************************************************/

#include <string.h>
#include <stdio.h>
#include <hi_go_surface.h>
#include <hi_go_bliter.h>
#include <hi_go_encoder.h>
#include <hi_unf_common.h>
#include <hi_type.h>

#include "drv_basic.h"
#include "drv_snd.h"
#include "drv_avplay.h"
#include "ysav.h"
#include "drv_os.h"
#include "osddrv.h"
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

typedef enum DRV_AudioInjectStatus_e
{
	DRV_AUDIO_INJECT_STOP,
	DRV_AUDIO_INJECT_START,
	DRV_AUDIO_INJECT_PAUSE,
	DRV_AUDIO_INJECT_RESUME
}DRV_AudioInjectStatus;

DRV_PlayRecCallback mAudioESInjectback_user = NULL;
static HI_HANDLE    mESInjectAvplay;
static HI_BOOL      mAudioInjectStopThread;
static HI_BOOL      mAudioInjectPauseThread;
static HI_BOOL      mAudioInjectOk;
static DRV_Handle   mAudioInjectFile;
static DRV_Handle   mVideoInjectFile;
static DRV_AudioInjectStatus mAudioInjectStatus;
  

 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_AudioESInjectEnableCallback
  Description     :注册回调函数，对事件回报状态
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AudioESInjectEnableCallback(DRV_PlayRecCallback Callback_f)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
    mAudioESInjectback_user = Callback_f;
    return DRV_NO_ERROR;
}
   

 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   :DRV_AudioESInjectDisableCallback
  Description     :反注册回调函数
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AudioESInjectDisableCallback(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
    mAudioESInjectback_user = NULL;
    return DRV_NO_ERROR;
}
   

 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_ESInjectSetup
  Description     :从直播切换到ES注入
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_ESInjectSetup(void)/*从直播切换到ES注入*/
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	ErrCode = DRV_AVAPI_AVstop(0);
    if (DRV_NO_ERROR != ErrCode)
    {
        printf("DRV_ESInjectStart DRV_AVAPI_AVstop ErrCode 0x%08x \n", ErrCode);
        return ErrCode;
    }
	ErrCode = DRV_AvplayerTerm(mAvPlayHandle,HI_UNF_AVPLAY_MEDIA_CHAN_AUD|HI_UNF_AVPLAY_MEDIA_CHAN_VID);
    if (DRV_NO_ERROR != ErrCode)
    {
        printf("DRV_ESInjectStart DRV_AvplayerTerm ErrCode 0x%08x \n", ErrCode);
        return ErrCode;
    }
    return ErrCode;
}
   
 
  /*****************************************************************************
   Date&Time       : 2012-12-07
   Author          : whyu modify
   Function Name   : DRV_ESInjectTerm
   Description     :从ES注入切换到直播
   Input           : 
   Outut           :  
   Calls           : 
   Called by       :
   Global Variables: 
   Others          : 
 *****************************************************************************/
DRV_ErrCode DRV_ESInjectTerm(void)/*从ES注入切换到直播*/
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	ErrCode = DRV_AvplayerSetup(HI_UNF_AVPLAY_STREAM_TYPE_TS,HI_UNF_AVPLAY_MEDIA_CHAN_AUD|HI_UNF_AVPLAY_MEDIA_CHAN_VID,&mAvPlayHandle);
    if (DRV_NO_ERROR != ErrCode)
    {
        printf("DRV_ESInjectTerm DRV_AvplayerSetup ErrCode 0x%08x \n", ErrCode);
        return ErrCode;
    }
    return ErrCode;
}
    

 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_AudioESInjectStart
  Description     :开启音频播放器
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AudioESInjectStart(DRV_AudioType AudStreamContent)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
    HI_U32                      AdecType = 0;
    HI_UNF_SYNC_ATTR_S          AvSyncAttr;
    HI_HA_DECODEMODE_E enAudioDecMode = HD_DEC_MODE_RAWPCM;
    HI_S32 s32DtsDtsCoreOnly = 0;

	switch(AudStreamContent)
	{
		case DRV_AUDIO_TYPE_AC3:
			AdecType = HA_AUDIO_ID_DOLBY_PLUS;
			break;
		case DRV_AUDIO_TYPE_MPEG1:
            AdecType = HA_AUDIO_ID_MP2;
			break;
		case DRV_AUDIO_TYPE_MPEG2:
			AdecType = HA_AUDIO_ID_MP2;
			break;
		case DRV_AUDIO_TYPE_MP3:
			AdecType = HA_AUDIO_ID_MP3;
			break;
		case DRV_AUDIO_TYPE_MPEG_AAC:
			AdecType = HA_AUDIO_ID_AAC;
			break;
		case DRV_AUDIO_TYPE_MPEG_HE_AAC:
			AdecType = HA_AUDIO_ID_AAC;
			break;
		case DRV_AUDIO_TYPE_DOLBY_PLUS:
            AdecType = HA_AUDIO_ID_DOLBY_PLUS;
			break;
		default:
			return -1;
	}
    
	ErrCode = DRV_AvplayerSetup(HI_UNF_AVPLAY_STREAM_TYPE_ES,HI_UNF_AVPLAY_MEDIA_CHAN_AUD,&mESInjectAvplay);
    if (DRV_NO_ERROR != ErrCode)
    {
        printf("DRV_AudioESInjectStart DRV_AvplayerSetup ErrCode 0x%08x \n", ErrCode);
        return ErrCode;
    }
    ErrCode = HI_UNF_AVPLAY_GetAttr(mESInjectAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC,&AvSyncAttr);
    AvSyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
    AvSyncAttr.stSyncStartRegion.s32VidPlusTime = 100;
    AvSyncAttr.stSyncStartRegion.s32VidNegativeTime = -100;
    AvSyncAttr.stSyncStartRegion.bSmoothPlay = HI_TRUE;
    AvSyncAttr.bQuickOutput = HI_TRUE;
    AvSyncAttr.u32PreSyncTimeoutMs = 600;
    ErrCode |= HI_UNF_AVPLAY_SetAttr(mESInjectAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC,&AvSyncAttr);
    if (DRV_NO_ERROR != ErrCode)
    {
        printf("DRV_AudioESInjectStart HI_UNF_AVPLAY_SetAttr failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }
    
    ErrCode = HIADP_AVPlay_SetAdecAttr(mESInjectAvplay, AdecType, enAudioDecMode, s32DtsDtsCoreOnly);
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("DRV_AudioESInjectStart HIADP_AVPlay_SetAdecAttr failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }
    
    ErrCode = HI_UNF_AVPLAY_Start(mESInjectAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("DRV_AudioESInjectStart HI_UNF_AVPLAY_Start failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }
    
    ErrCode = DRV_AV_UnMuteAudio();
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("DRV_AudioESInjectStart DRV_AV_UnMuteAudio failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }
    
    mAudioInjectStopThread = HI_FALSE;
    mAudioInjectPauseThread = HI_FALSE;
    mAudioInjectOk = HI_FALSE;

	return ErrCode;
}
   

 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_AudioESInjectBuffer
  Description     :注入音频ES数据
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AudioESInjectBuffer(DRV_AudioESInjectSource *DataSource_p)
{
    HI_UNF_STREAM_BUF_S   StreamBuf;
    HI_U32            Readlen;
    HI_U32            Readlenamount;
    HI_S32            Ret;
    HI_BOOL           bIsEmpty;
    HI_BOOL           bIsCallbackUp = FALSE;
    HI_BOOL           bAudBufAvail = HI_TRUE;
    bAudBufAvail = HI_FALSE;
	Readlenamount = 0;

    //mAudioInjectFile = DRV_VfsOpenFile(mAudioInjectSource.FileName,"rw");
    mAudioInjectFile = fopen(DataSource_p->FileName,"rw");

    while (!mAudioInjectStopThread)
    {
    	if(mAudioInjectPauseThread)
    	{
    		DRV_OS_TaskDelay_MS(50);
    	}
    	if(mAudioInjectOk)
    	{
    		DRV_OS_TaskDelay_MS(50);
    		Ret = HI_UNF_AVPLAY_IsBuffEmpty(mESInjectAvplay, &bIsEmpty);
            if (Ret != DRV_NO_ERROR )
            {
                printf("call DRV_AudioESInjectBuffer HI_UNF_AVPLAY_IsBuffEmpty failed.Ret 0x%08x\n",Ret);
            }
            if(bIsEmpty)
            {
                printf("------call DRV_AudioESInjectBuffer Buffer is Empty now!\n");
            }
     		if(bIsEmpty&&(bIsCallbackUp == FALSE)&&(mAudioESInjectback_user != NULL))
     		{
    			mAudioESInjectback_user(DRV_PLAYREC_REACH_END_OF_PLAYBACK_EVT,NULL);
    			bIsCallbackUp = TRUE;
    		}	
    	}

        
        Ret = HI_UNF_AVPLAY_GetBuf(mESInjectAvplay, HI_UNF_AVPLAY_BUF_ID_ES_AUD, 0x1000, &StreamBuf, 0);
        if (DRV_NO_ERROR == Ret)
        {
            bAudBufAvail = HI_TRUE;
            if(DataSource_p->FileName != NULL)//基于文件
            {
                //Readlen = DRV_VfsReadFile(mAudioInjectFile,StreamBuf.pu8Data, 0x1000);
                Readlen = fread(StreamBuf.pu8Data,1, 0x1000,mAudioInjectFile);
                if (Readlen > 0)
                {
                    Ret = HI_UNF_AVPLAY_PutBuf(mESInjectAvplay, HI_UNF_AVPLAY_BUF_ID_ES_AUD, Readlen, DataSource_p->PtsMs);
                    if (Ret != DRV_NO_ERROR )
                    {
                        printf("call DRV_AudioESInjectBuffer HI_UNF_AVPLAY_PutBuf failed.\n");
                    }
                }
                else if(Readlen <= 0)
                {
    	                printf("The File is the end!\n");
                    rewind(mAudioInjectFile);
                    Readlenamount = 0;
                    mAudioInjectOk = TRUE;
                }
            }
            else//基于buffer
            {
            	if(Readlenamount >= DataSource_p->DataLength)//已经播放完了
            	{
    	                printf("The buffer is the end! Readlenamount %d DataSource_p->DataLength %d\n",Readlenamount,DataSource_p->DataLength);
                    mAudioInjectOk = TRUE;
           		}
           		else
           		{
           			Readlen = 0x1000;
           			if(Readlenamount+0x1000 > DataSource_p->DataLength)
           				Readlen = DataSource_p->DataLength - Readlenamount;
                	memcpy(StreamBuf.pu8Data, DataSource_p->AudioData_p + Readlenamount, Readlen);
                	Readlenamount += Readlen;
                    Ret = HI_UNF_AVPLAY_PutBuf(mESInjectAvplay, HI_UNF_AVPLAY_BUF_ID_ES_AUD, Readlen, DataSource_p->PtsMs);
                    if (Ret != DRV_NO_ERROR )
                    {
                        printf("call DRV_AudioESInjectBuffer HI_UNF_AVPLAY_PutBuf failed.\n");
                    }
           		}
            }
        }
        else if (Ret != DRV_NO_ERROR )
        {
            bAudBufAvail = HI_FALSE;
        }

        /* wait for buffer */
        if (HI_FALSE == bAudBufAvail)
        {
        	//printf("------drv_AudioInjectTask full,So slow down!\n");
            DRV_OS_TaskDelay_MS(50);
        }
    }
    return 0;
}
   

 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_AudioESInjectStop
  Description     :停止音频的播放和注入
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AudioESInjectStop(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;

	ErrCode = DRV_AvplayerTerm(mESInjectAvplay,HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
    if (DRV_NO_ERROR != ErrCode)
    {
        printf("DRV_AudioInjectStop HI_UNF_SND_Detach ErrCode 0x%08x \n", ErrCode);
        return ErrCode;
    }

	mESInjectAvplay = NULL;
    mAudioInjectStatus = DRV_AUDIO_INJECT_STOP;
	
	return ErrCode;
}
   

 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   :DRV_AudioESInjectPause
  Description     :暂停音频播放
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AudioESInjectPause(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	if(mAudioInjectStatus == DRV_AUDIO_INJECT_START)
	{
		mAudioInjectPauseThread = TRUE;
		ErrCode = HI_UNF_AVPLAY_Pause(mESInjectAvplay,NULL);
	    if (ErrCode != DRV_NO_ERROR)
	    {
	        printf("DRV_AudioInjectPause HI_UNF_AVPLAY_Pause failed.ErrCode 0x%08x\n",ErrCode);
	        return ErrCode;
	    }
	    mAudioInjectStatus = DRV_AUDIO_INJECT_PAUSE;
    }
	return ErrCode;
}
   

 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   :DRV_AudioESInjectResume
  Description     :恢复音频播放
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AudioESInjectResume(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	if(mAudioInjectStatus == DRV_AUDIO_INJECT_PAUSE)
	{
		mAudioInjectPauseThread = FALSE;
		ErrCode = HI_UNF_AVPLAY_Resume(mESInjectAvplay,NULL);
	    if (ErrCode != DRV_NO_ERROR)
	    {
	        printf("DRV_AudioInjectResume HI_UNF_AVPLAY_Resume failed.ErrCode 0x%08x\n",ErrCode);
	        return ErrCode;
	    }
	    mAudioInjectStatus = DRV_AUDIO_INJECT_START;
    }
	return ErrCode;
}
   

 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_VideoESInjectStart
  Description     :开启视频播放器
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_VideoESInjectStart(DRV_VideoType VidStreamContent)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
    HI_UNF_VCODEC_TYPE_E        VdecType;
    HI_UNF_SYNC_ATTR_S          AvSyncAttr;

	switch(VidStreamContent)
	{
        case DRV_VIDEO_TYPE_MP1V:
		case DRV_VIDEO_TYPE_MP2V:
			VdecType = HI_UNF_VCODEC_TYPE_MPEG2;
			break;
		case DRV_VIDEO_TYPE_MPEG4P2:
			VdecType = HI_UNF_VCODEC_TYPE_MPEG4;
			break;
		case DRV_VIDEO_TYPE_H264:
			VdecType = HI_UNF_VCODEC_TYPE_H264;
			break;	
		case DRV_VIDEO_TYPE_AVS:
			VdecType = HI_UNF_VCODEC_TYPE_AVS;
			break;	
		default:
			printf("Stream type is not supported.\n");
			return -1;
	}
    ErrCode = DRV_AvplayerSetup(HI_UNF_AVPLAY_STREAM_TYPE_ES,HI_UNF_AVPLAY_MEDIA_CHAN_VID,&mESInjectAvplay);
    if (DRV_NO_ERROR != ErrCode)
    {
        printf("DRV_VideoESInjectStart DRV_AvplayerSetup ErrCode 0x%08x \n", ErrCode);
        return ErrCode;
    }

    ErrCode = HI_UNF_AVPLAY_GetAttr(mESInjectAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC,&AvSyncAttr);
    AvSyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
    AvSyncAttr.stSyncStartRegion.s32VidPlusTime = 100;
    AvSyncAttr.stSyncStartRegion.s32VidNegativeTime = -100;
    AvSyncAttr.stSyncStartRegion.bSmoothPlay = HI_TRUE;
    AvSyncAttr.bQuickOutput = HI_TRUE;
    AvSyncAttr.u32PreSyncTimeoutMs = 600;
    ErrCode |= HI_UNF_AVPLAY_SetAttr(mESInjectAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC,&AvSyncAttr);
    if (DRV_NO_ERROR != ErrCode)
    {
        printf("DRV_VideoESInjectStart HI_UNF_AVPLAY_SetAttr failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }
    
    ErrCode = HIADP_AVPlay_SetVdecAttr(mESInjectAvplay, VdecType, HI_UNF_VCODEC_MODE_NORMAL);
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("DRV_VideoESInjectStart HIADP_AVPlay_SetVdecAttr failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }
    
    ErrCode = HI_UNF_AVPLAY_Start(mESInjectAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("DRV_VideoESInjectStart HI_UNF_AVPLAY_Start failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }

    mAudioInjectStopThread = HI_FALSE;
    mAudioInjectPauseThread = HI_FALSE;
    mAudioInjectOk = HI_FALSE;

	return ErrCode;
}
   

 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   :DRV_VideoESInjectBuffer
  Description     :开始注入视频数据
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_VideoESInjectBuffer(DRV_VideoESInjectSource *DataSource_p)
{
    HI_UNF_STREAM_BUF_S   StreamBuf;
    HI_U32            Readlen;
    HI_U32            Readlenamount;
    HI_S32            Ret;
    HI_BOOL           bIsEmpty;
    HI_BOOL           bIsCallbackUp = FALSE;    
    HI_BOOL           bVidBufAvail = HI_TRUE;
    bVidBufAvail = HI_FALSE;
	Readlenamount = 0;

    //mVideoInjectFile = DRV_VfsOpenFile(mAudioInjectSource.FileName,"rw");
    mVideoInjectFile = fopen(DataSource_p->FileName,"rw");

    while (!mAudioInjectStopThread)
    {
    	if(mAudioInjectPauseThread)
    	{
    		DRV_OS_TaskDelay_MS(50);
    	}
    	if(mAudioInjectOk)
    	{
    		DRV_OS_TaskDelay_MS(50);
    		Ret = HI_UNF_AVPLAY_IsBuffEmpty(mESInjectAvplay, &bIsEmpty);
            if (Ret != DRV_NO_ERROR )
            {
                printf("call drv_AudioInjectTask HI_UNF_AVPLAY_IsBuffEmpty failed.Ret 0x%08x\n",Ret);
            }
            if(bIsEmpty)
            {
                printf("------call drv_AudioInjectTask Buffer is Empty now!\n");
            }
     		if(bIsEmpty&&(bIsCallbackUp == FALSE)&&(mAudioESInjectback_user != NULL))
     		{
    			mAudioESInjectback_user(DRV_PLAYREC_REACH_END_OF_PLAYBACK_EVT,NULL);
    			bIsCallbackUp = TRUE;
    		}	
    	}
    	
        Ret = HI_UNF_AVPLAY_GetBuf(mESInjectAvplay, HI_UNF_AVPLAY_BUF_ID_ES_VID, 0x4000, &StreamBuf, 0);
        if (DRV_NO_ERROR == Ret)
        {
            bVidBufAvail = HI_TRUE;
            if(DataSource_p->FileName != NULL)//基于文件
            {
                //Readlen = DRV_VfsReadFile(mVideoInjectFile,StreamBuf.pu8Data, 0x4000);
                Readlen = fread(StreamBuf.pu8Data,1, 0x4000,mVideoInjectFile);
                if (Readlen > 0)
                {
                    Ret = HI_UNF_AVPLAY_PutBuf(mESInjectAvplay, HI_UNF_AVPLAY_BUF_ID_ES_VID, Readlen, DataSource_p->PtsMs);
                    if (Ret != DRV_NO_ERROR )
                    {
                        printf("call drv_AudioInjectTask HI_UNF_AVPLAY_PutBuf failed.\n");
                    }
                }
                else if(Readlen <= 0)
                {
    	                printf("The File is the end!\n");
                    rewind(mVideoInjectFile);
                    Readlenamount = 0;
                    mAudioInjectOk = TRUE;
                }
            }
            else//基于buffer
            {
            	if(Readlenamount >= DataSource_p->DataLength)//已经播放完了
            	{
    	                printf("The buffer is the end! Readlenamount %d mAudioInjectSource.DataLength %d\n",Readlenamount,DataSource_p->DataLength);
                    mAudioInjectOk = TRUE;
           		}
           		else
           		{
           			Readlen = 0x4000;
           			if(Readlenamount+0x4000 > DataSource_p->DataLength)
           				Readlen = DataSource_p->DataLength - Readlenamount;
                	memcpy(StreamBuf.pu8Data,DataSource_p->VideoData_p + Readlenamount, Readlen);
                	Readlenamount += Readlen;
                    Ret = HI_UNF_AVPLAY_PutBuf(mESInjectAvplay, HI_UNF_AVPLAY_BUF_ID_ES_VID, Readlen, DataSource_p->PtsMs);
                    if (Ret != DRV_NO_ERROR )
                    {
                        printf("call drv_AudioInjectTask HI_UNF_AVPLAY_PutBuf failed.\n");
                    }
           		}
            }
        }
        else if (Ret != DRV_NO_ERROR )
        {
            bVidBufAvail = HI_FALSE;
        }

        /* wait for buffer */
        if (HI_FALSE == bVidBufAvail)
        {
        	//printf("------drv_AudioInjectTask full,So slow down!\n");
            DRV_OS_TaskDelay_MS(50);
        }
    }
    return 0;
}
   

 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_VideoESInjectStop
  Description     :停止播放
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_VideoESInjectStop(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;

	ErrCode = DRV_AvplayerTerm(mESInjectAvplay,HI_UNF_AVPLAY_MEDIA_CHAN_VID);
    if (DRV_NO_ERROR != ErrCode)
    {
        printf("DRV_VideoESInjectStop DRV_AvplayerTerm ErrCode 0x%08x \n", ErrCode);
        return ErrCode;
    }

	mESInjectAvplay = NULL;
    mAudioInjectStatus = DRV_AUDIO_INJECT_STOP;
	
	return ErrCode;
}
   

 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_VideoESInjectPause
  Description     :暂停播放
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_VideoESInjectPause(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	if(mAudioInjectStatus == DRV_AUDIO_INJECT_START)
	{
		mAudioInjectPauseThread = TRUE;
		ErrCode = HI_UNF_AVPLAY_Pause(mESInjectAvplay,NULL);
	    if (ErrCode != DRV_NO_ERROR)
	    {
	        printf("DRV_VideoESInjectPause HI_UNF_AVPLAY_Pause failed.ErrCode 0x%08x\n",ErrCode);
	        return ErrCode;
	    }
	    mAudioInjectStatus = DRV_AUDIO_INJECT_PAUSE;
    }
	return ErrCode;
}
   

 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_VideoESInjectResume
  Description     :恢复播放
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_VideoESInjectResume(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	if(mAudioInjectStatus == DRV_AUDIO_INJECT_PAUSE)
	{
		mAudioInjectPauseThread = FALSE;
		ErrCode = HI_UNF_AVPLAY_Resume(mESInjectAvplay,NULL);
	    if (ErrCode != DRV_NO_ERROR)
	    {
	        printf("DRV_VideoESInjectResume HI_UNF_AVPLAY_Resume failed.ErrCode 0x%08x\n",ErrCode);
	        return ErrCode;
	    }
	    mAudioInjectStatus = DRV_AUDIO_INJECT_START;
    }
	return ErrCode;
}

