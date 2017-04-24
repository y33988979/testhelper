#include <string.h>
#include <stdio.h>
#ifndef STTBX_PRINT
#define STTBX_PRINT
#endif
#include "drv_basic.h"
#include "drv_snd.h"
#include "drv_avplay.h"
#include "ysav.h"
#include "drv_os.h"
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

/* Flag to inject using DMAs */
/* ------------------------- */
DRV_PlayRecCallback mAudioInjectback_user = NULL;
static HI_HANDLE    mAudioInjectAvplay;
static HI_BOOL      mAudioInjectStopThread;
static HI_BOOL      mAudioInjectPauseThread;
static HI_BOOL      mAudioInjectOk;
static DRV_AudioInjectSource mAudioInjectSource;
static DRVOS_Task_t *pAudioInjectTaskid;
static DRV_Handle mAudioInjectFile;
static DRV_AudioInjectStatus mAudioInjectStatus;

    

 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_AudioInjectEnableCallback
  Description     :注册回调函数
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AudioInjectEnableCallback(DRV_PlayRecCallback Callback_f)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
    mAudioInjectback_user = Callback_f;
    return DRV_NO_ERROR;
}
    

 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_AudioInjectDisableCallback
  Description     :反注册回调函数
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AudioInjectDisableCallback(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
    mAudioInjectback_user = NULL;
    return DRV_NO_ERROR;
}
    

 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : drv_AudioInjectTask
  Description     :启动线程注入音频数据
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void * drv_AudioInjectTask(void *args)
{
    HI_UNF_STREAM_BUF_S   StreamBuf;
    HI_U32            Readlen;
    HI_U32            Readlenamount;
    DRV_ErrCode       Ret = DRV_NO_ERROR;
    HI_BOOL           bIsEmpty;
    HI_BOOL           bIsCallbackUp = FALSE;
    HI_BOOL           bAudBufAvail = HI_TRUE;
    bAudBufAvail = HI_FALSE;
	Readlenamount = 0;
	
    while (!mAudioInjectStopThread)
    {
    	if(mAudioInjectPauseThread)
    	{
    		DRV_OS_TaskDelay_MS(50);
    		continue;
    	}
    	if(mAudioInjectOk)
    	{
    		DRV_OS_TaskDelay_MS(50);
			Ret = HI_UNF_AVPLAY_IsBuffEmpty(mAudioInjectAvplay, &bIsEmpty);
            if (Ret != DRV_NO_ERROR )
            {
                printf("call drv_AudioInjectTask HI_UNF_AVPLAY_IsBuffEmpty failed.Ret 0x%08x\n",Ret);
            }
            if(bIsEmpty)
            {
                printf("------call drv_AudioInjectTask Buffer is Empty now!\n");
            }
     		if(bIsEmpty&&(bIsCallbackUp == FALSE)&&(mAudioInjectback_user != NULL))
     		{
    			mAudioInjectback_user(DRV_PLAYREC_REACH_END_OF_PLAYBACK_EVT,NULL);
    			bIsCallbackUp = TRUE;
    		}	
   			continue;
    	}
    	
        Ret = HI_UNF_AVPLAY_GetBuf(mAudioInjectAvplay, HI_UNF_AVPLAY_BUF_ID_ES_AUD, 0x1000, &StreamBuf, 0);
        if (DRV_NO_ERROR == Ret)
        {
            bAudBufAvail = HI_TRUE;
            if(mAudioInjectSource.FileName != NULL)//基于文件
            {
	            Readlen = DRV_VfsReadFile(mAudioInjectFile,StreamBuf.pu8Data, 0x1000);
	            if (Readlen > 0)
	            {
	                Ret = HI_UNF_AVPLAY_PutBuf(mAudioInjectAvplay, HI_UNF_AVPLAY_BUF_ID_ES_AUD, Readlen, 0);
	                if (Ret != DRV_NO_ERROR )
	                {
	                    printf("call drv_AudioInjectTask HI_UNF_AVPLAY_PutBuf failed.\n");
	                }
	            }
	            else if(Readlen <= 0)
	            {
 	                printf("The File is the end!\n");
	                //rewind(mAudioInjectFile);
	                //Readlenamount = 0;
	                mAudioInjectOk = TRUE;
	            }
	            else
	            {
	            	Readlenamount += Readlen;
	            }
            }
            else//基于buffer
            {
            	if(Readlenamount >= mAudioInjectSource.DataLength)//已经播放完了
            	{
 	                printf("The buffer is the end! Readlenamount %d mAudioInjectSource.DataLength %d\n",Readlenamount,mAudioInjectSource.DataLength);
 	                //Readlenamount = 0;
	                mAudioInjectOk = TRUE;
           		}
           		else
           		{
           			Readlen = 0x1000;
           			if(Readlenamount+0x1000 > mAudioInjectSource.DataLength)
           				Readlen = mAudioInjectSource.DataLength - Readlenamount;
	            	memcpy(StreamBuf.pu8Data,mAudioInjectSource.AudioData_p + Readlenamount, Readlen);
	            	Readlenamount += Readlen;
	                Ret = HI_UNF_AVPLAY_PutBuf(mAudioInjectAvplay, HI_UNF_AVPLAY_BUF_ID_ES_AUD, Readlen, 0);
	                if (Ret != DRV_NO_ERROR )
	                {
	                    printf("call drv_AudioInjectTask HI_UNF_AVPLAY_PutBuf failed.\n");
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
  Function Name   : DRV_AudioInjectStart
  Description     :开启音频播放器，并注入数据
  Input           : DeviceId，AUD_StartParams，NbLoops无效参数
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AudioInjectStart(U32 DeviceId,DRV_AudioInjectSource *DataSource_p,STAUD_StartParams_t *AUD_StartParams,U32 NbLoops)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
    HI_U32                      AdecType = 0;
    HI_UNF_AVPLAY_ATTR_S        AvplayAttr;
    HI_UNF_SYNC_ATTR_S          AvSyncAttr;
    HI_HA_DECODEMODE_E          enAudioDecMode = HD_DEC_MODE_RAWPCM;
    HI_S32                      s32DtsDtsCoreOnly = 0;
	if(mAudioInjectStatus == DRV_AUDIO_INJECT_STOP)
	{
		if(DataSource_p->FileName != NULL)//文件形式
		{
			mAudioInjectSource.FileName = DataSource_p->FileName;
			mAudioInjectSource.AudioData_p = NULL;
		}
		else
		{
			mAudioInjectSource.FileName = NULL;
			mAudioInjectSource.AudioData_p = DataSource_p->AudioData_p;
			mAudioInjectSource.DataLength = DataSource_p->DataLength;
		}	
#ifdef USE_AVPLAYER_BUT_CREATE
#else
		ErrCode = DRV_AVAPI_AVstop(0);
	    if (DRV_NO_ERROR != ErrCode)
	    {
	        printf("DRV_AudioInjectStart DRV_AVAPI_AVstop ErrCode 0x%08x \n", ErrCode);
	        return ErrCode;
	    }
		ErrCode = DRV_AvplayerTerm(mAvPlayHandle,HI_UNF_AVPLAY_MEDIA_CHAN_AUD|HI_UNF_AVPLAY_MEDIA_CHAN_VID);
	    if (DRV_NO_ERROR != ErrCode)
	    {
	        printf("DRV_AudioInjectStart DRV_AvplayerTerm ErrCode 0x%08x \n", ErrCode);
	        return ErrCode;
	    }
#endif
		AdecType = HA_AUDIO_ID_MP3;
		ErrCode = DRV_AvplayerSetup(HI_UNF_AVPLAY_STREAM_TYPE_ES,HI_UNF_AVPLAY_MEDIA_CHAN_AUD,&mAudioInjectAvplay);
	    if (DRV_NO_ERROR != ErrCode)
	    {
	        printf("DRV_AudioInjectStart DRV_AvplayerSetup ErrCode 0x%08x \n", ErrCode);
	        return ErrCode;
	    }
	    ErrCode = HI_UNF_AVPLAY_GetAttr(mAudioInjectAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC,&AvSyncAttr);
	    AvSyncAttr.enSyncRef = HI_UNF_SYNC_REF_NONE;
	    ErrCode |= HI_UNF_AVPLAY_SetAttr(mAudioInjectAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC,&AvSyncAttr);
	    if (DRV_NO_ERROR != ErrCode)
	    {
	        printf("DRV_AudioInjectStart HI_UNF_AVPLAY_SetAttr failed.ErrCode 0x%08x\n",ErrCode);
	        return ErrCode;
	    }
	    
	    ErrCode = HIADP_AVPlay_SetAdecAttr(mAudioInjectAvplay, AdecType, enAudioDecMode, s32DtsDtsCoreOnly);
	    if (ErrCode != DRV_NO_ERROR)
	    {
	        printf("DRV_AudioInjectStart HIADP_AVPlay_SetAdecAttr failed.ErrCode 0x%08x\n",ErrCode);
	        return ErrCode;
	    }
	    
	    ErrCode = HI_UNF_AVPLAY_Start(mAudioInjectAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
	    if (ErrCode != DRV_NO_ERROR)
	    {
	        printf("DRV_AudioInjectStart HI_UNF_AVPLAY_Start failed.ErrCode 0x%08x\n",ErrCode);
	        return ErrCode;
	    }
	    DRV_AV_UnMuteAudio();
	    mAudioInjectStopThread = HI_FALSE;
	    mAudioInjectPauseThread = HI_FALSE;
	    mAudioInjectOk = HI_FALSE;
		if(mAudioInjectSource.FileName != NULL)//基于文件
		{
			mAudioInjectFile = DRV_VfsOpenFile(mAudioInjectSource.FileName,"r");
		}
	    /*create a thread for receive*/
		pAudioInjectTaskid = DRV_OS_TaskCreate("drv_KeyReceiveTask",drv_AudioInjectTask,3,8096*2,NULL,0);
	    if (pAudioInjectTaskid == NULL)
	    {
	        printf("DRV_AudioInjectStart DRV_OS_TaskCreate failed.\n");
	        return ErrCode;
	    }
	    mAudioInjectStatus = DRV_AUDIO_INJECT_START;
	}
	return ErrCode;
}

 
  /*****************************************************************************
   Date&Time       : 2012-12-07
   Author          : whyu modify
   Function Name   : DRV_AudioInjectStop
   Description     :关闭播放器，停止注入数据
   Input           : DeviceId，无效参数
   Outut           :  
   Calls           : 
   Called by       :
   Global Variables: 
   Others          : 
 *****************************************************************************/
DRV_ErrCode DRV_AudioInjectStop(U32 DeviceId)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	S32_YS      result;
    
	if(mAudioInjectStatus != DRV_AUDIO_INJECT_STOP)
	{
		mAudioInjectStopThread = HI_TRUE;
        
		/* Delete the inject task */
		while(1)
		{
			result = DRV_OS_TaskWait(pAudioInjectTaskid, DRVOS_WAIT_FINITY, 500); 
			if(result == 0)
			break;
			DRV_OS_TaskKill(pAudioInjectTaskid, 0, 0);
			DRV_OS_TaskDelay_MS(10); 
		}
		DRV_OSTaskDelete(pAudioInjectTaskid);
		if(mAudioInjectSource.FileName != NULL)//基于文件
		{
			DRV_VfsCloseFile(mAudioInjectFile);
			mAudioInjectFile = NULL;
		}

#ifdef USE_AVPLAYER_BUT_CREATE
#else
		ErrCode = DRV_AvplayerTerm(mAudioInjectAvplay,HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
	    if (DRV_NO_ERROR != ErrCode)
	    {
	        printf("DRV_AudioInjectStop DRV_AvplayerTerm ErrCode 0x%08x \n", ErrCode);
	        return ErrCode;
	    }
		ErrCode = DRV_AvplayerSetup(HI_UNF_AVPLAY_STREAM_TYPE_TS,HI_UNF_AVPLAY_MEDIA_CHAN_AUD|HI_UNF_AVPLAY_MEDIA_CHAN_VID,&mAvPlayHandle);
	    if (DRV_NO_ERROR != ErrCode)
	    {
	        printf("DRV_AudioInjectStop DRV_AvplayerSetup ErrCode 0x%08x \n", ErrCode);
	        return ErrCode;
	    }
#endif
		mAudioInjectAvplay = NULL;
	    mAudioInjectStatus = DRV_AUDIO_INJECT_STOP;
	}
	return ErrCode;
}
  
  
   /*****************************************************************************
    Date&Time       : 2012-12-07
    Author          : whyu modify
    Function Name   : DRV_AudioInjectPause
    Description     :暂停播放
    Input           : DeviceId，无效参数
    Outut           :  
    Calls           : 
    Called by       :
    Global Variables: 
    Others          : 
  *****************************************************************************/
DRV_ErrCode DRV_AudioInjectPause(U32 DeviceId)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	if(mAudioInjectStatus == DRV_AUDIO_INJECT_START)
	{
		mAudioInjectPauseThread = TRUE;
		ErrCode = HI_UNF_AVPLAY_Pause(mAudioInjectAvplay,NULL);
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
     Function Name   : DRV_AudioInjectResume
     Description     :恢复播放
     Input           : DeviceId，无效参数
     Outut           :  
     Calls           : 
     Called by       :
     Global Variables: 
     Others          : 
   *****************************************************************************/
DRV_ErrCode DRV_AudioInjectResume(U32 DeviceId)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	if(mAudioInjectStatus == DRV_AUDIO_INJECT_PAUSE)
	{
		mAudioInjectPauseThread = FALSE;
		ErrCode = HI_UNF_AVPLAY_Resume(mAudioInjectAvplay,NULL);
	    if (ErrCode != DRV_NO_ERROR)
	    {
	        printf("DRV_AudioInjectResume HI_UNF_AVPLAY_Resume failed.ErrCode 0x%08x\n",ErrCode);
	        return ErrCode;
	    }
	    mAudioInjectStatus = DRV_AUDIO_INJECT_START;
    }
	return ErrCode;
}



