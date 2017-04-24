#include "ysplayrec.h"
#include "hi_svr_player.h"
#include "hi_unf_demux.h"
#include "hi_adp_pvr.h"
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

#define MAX_TIMESHIFT_REC_FILE_SIZE       (3500*1024*1024LLU)
#define MAX_REC_FILE_SIZE          (100*1024*1024*1024LLU)
static HI_BOOL bRewind = HI_TRUE;
static HI_BOOL bDoCipher = HI_FALSE;
HI_U32 mPlayRecRecordHandle;
HI_U32 mPvrPlayPlaybackHandle = 0;//时移和播放共用一个handle
DRV_PlayRecMode_t mPlayRecMode = DRV_PLAYREC_MODE_NONE;
DRV_PlayRecPlaybackStatus_t mPlayRecPlaybackStatus = DRV_PLAYREC_PLAYBACK_NONE;
DRV_PlayRecRecordStatus_t   mPlayRecRecordStatus   = DRV_PLAYREC_RECORD_NONE;
DRV_PlayRecCallback mPlayRecCallback = NULL;
BOOL mIsTimeShiftStart = FALSE;
static S8 PlayRecFullName[512];   
static BOOL mRecTaskStatus;
static DRVOS_Task_t *pRecThread;
static HI_U32 mRecLimitTimeInMs;
static PMT_COMPACT_PROG RecPstProgInfo;
DRV_ErrCode DRV_PlayRecStartPlay(U32 PlaybackId,DRV_PlayRecStartPlayParams_t *Params_p)
{
	DRV_ErrCode ErrCode;
	if(mPlayRecPlaybackStatus != DRV_PLAYREC_PLAYBACK_RUNNING\
	 &&mPlayRecPlaybackStatus != DRV_PLAYREC_PLAYBACK_BACKWARD\
	 &&mPlayRecPlaybackStatus != DRV_PLAYREC_PLAYBACK_SLOWFORWARD\
	 &&mPlayRecPlaybackStatus != DRV_PLAYREC_PLAYBACK_FORWARD)
	{
		DRV_AV_UnMuteAudio();
		if(PlaybackId == 1)
		{
		    if(mPlayRecMode == DRV_PLAYREC_MODE_PVRTIMESHIFT)
		    {
                ErrCode = HI_UNF_PVR_PlayResumeChn(mPvrPlayPlaybackHandle);
                if (ErrCode != HI_SUCCESS)
                {
                    printf("call HI_UNF_PVR_PlayPauseChn failed.\n");
                    return ErrCode;
                }
		    }
		    else
		    {
			    ErrCode = HI_UNF_PVR_PlayInit();
			    if (ErrCode != HI_SUCCESS)
			    {
			        printf("call HI_UNF_PVR_RecInit failed.\n");
			        return ErrCode;
			    }

			    ErrCode = PVR_RegisterCallBacks();
			    if (ErrCode != HI_SUCCESS)
			    {
			        printf("call PVR_RegisterCallBacks failed.\n");
			        return ErrCode;
			    }
			    memset(PlayRecFullName,0,512);
				DRV_VfsPathUserToDrv(Params_p->SrcPath,PlayRecFullName,FALSE);
			    ErrCode = PVR_StartPlayBack(PlayRecFullName, &mPvrPlayPlaybackHandle, mAvPlayHandle);
			    if (ErrCode != HI_SUCCESS)
			    {
			        printf("call PVR_StartPlayBack failed.\n");
			        PVR_StopPlayBack(mPvrPlayPlaybackHandle);
			        PVR_UnRegisterCallBacks();
			        HI_UNF_PVR_PlayDeInit();
			        mPvrPlayPlaybackHandle = 0;
			        return ErrCode;
			    }
			    if(Params_p->Callback != NULL)
			    	mPlayRecCallback = Params_p->Callback;
			    mPlayRecMode = DRV_PLAYREC_MODE_PVRPLAY;
		    }
		}
		else
		{
			ErrCode=DRV_ffmpeg_setup();
			ErrCode|=DRV_ffmpeg_start(Params_p->SrcPath);
		    if (HI_SUCCESS != ErrCode)
		    {
		        printf("---->>>>[DRV]DRV_PlayRecStartPlay DRV_ffmpeg_setup ErrCode 0x%08x \n", ErrCode);
		        return ErrCode;
		    }
            DRV_AV_UnMuteAudio();
			if(Params_p->Callback != NULL)
				DRV_ffmpegEnableCallback(Params_p->Callback);
			mPlayRecMode = DRV_PLAYREC_MODE_LOCALPLAY;	
		}	
	}
	if(ErrCode==0)
	{
		mPlayRecPlaybackStatus = DRV_PLAYREC_PLAYBACK_RUNNING;
		return DRV_NO_ERROR;
	}
	else
	{
		mPlayRecPlaybackStatus = DRV_PLAYREC_PLAYBACK_NONE;
		return -1;
	}
	mIsTimeShiftStart = TRUE;
}

DRV_ErrCode DRV_PlayRecStopPlay(U32 PlaybackId)
{
	DRV_ErrCode ErrCode;
	if(mPlayRecPlaybackStatus != DRV_PLAYREC_PLAYBACK_STOPPED)
	{
		if(PlaybackId == 1)
		{
			if(mPlayRecMode == DRV_PLAYREC_MODE_PVRTIMESHIFT)
			{
#if 0			
				while(1)
				{
					ErrCode = DRV_OS_TaskWait(pRecThread, DRVOS_WAIT_FINITY, 500); 
					if(ErrCode == 0)
					break;
					DRV_OS_TaskKill(pRecThread, 0, 0);
					DRV_OS_TaskDelay_MS(10); 
				}
				DRV_OSTaskDelete(pRecThread);
			    pRecThread = NULL;
#endif			    
				PVR_RecStop(mPlayRecRecordHandle);	
			}
    		PVR_StopPlayBack(mPvrPlayPlaybackHandle);
    		PVR_UnRegisterCallBacks();
    		HI_UNF_PVR_PlayDeInit();
    		mPvrPlayPlaybackHandle = 0;
			if(mPlayRecMode == DRV_PLAYREC_MODE_PVRTIMESHIFT)
			{
			    ErrCode = HI_UNF_PVR_RecDeInit();
			    if (ErrCode != HI_SUCCESS)
			    {
			        printf("call HI_UNF_DMX_DetachTSPort failed.\n");
			        return ErrCode;
			    }
			}
		    ErrCode = HI_UNF_DMX_DetachTSPort(PVR_DMX_ID_REC);
		    if (ErrCode != HI_SUCCESS)
		    {
		        printf("call HI_UNF_DMX_DetachTSPort failed.\n");
		        return ErrCode;
		    }
		    mPlayRecRecordHandle = 0;
		    mPvrPlayPlaybackHandle = 0;
		}
		else
		{
			DRV_ffmpeg_stop();
			DRV_ffmpeg_term();
		}
		mPlayRecPlaybackStatus = DRV_PLAYREC_PLAYBACK_STOPPED;
		mPlayRecMode = DRV_PLAYREC_MODE_NONE;
		return DRV_NO_ERROR;
	}
	return -1;
}

DRV_ErrCode DRV_PlayRecPausePlay(U32 PlaybackId)
{
	DRV_ErrCode ErrCode;
	if(mPlayRecPlaybackStatus   == DRV_PLAYREC_PLAYBACK_RUNNING\
	   ||mPlayRecPlaybackStatus == DRV_PLAYREC_PLAYBACK_BACKWARD\
	   ||mPlayRecPlaybackStatus == DRV_PLAYREC_PLAYBACK_FORWARD)
	{
		if(PlaybackId == 1)
		{
            printf("PVR pause now.\n");
            ErrCode = HI_UNF_PVR_PlayPauseChn(mPvrPlayPlaybackHandle);
            if (ErrCode != HI_SUCCESS)
            {
                printf("call HI_UNF_PVR_PlayPauseChn failed.\n");
                return ErrCode;
            }
		}
		else
		{
			DRV_ffmpeg_Pause();
		}
		mPlayRecPlaybackStatus = DRV_PLAYREC_PLAYBACK_PAUSED;
		return DRV_NO_ERROR;
	}
	mIsTimeShiftStart = TRUE;
	return -1;
}

DRV_ErrCode DRV_PlayRecResumePlay(U32 PlaybackId)
{
	DRV_ErrCode ErrCode;
	if(mPlayRecPlaybackStatus != DRV_PLAYREC_PLAYBACK_STOPPED)
	{
		if(PlaybackId == 1)
		{
            printf("PVR normal play now.\n");
            ErrCode = HI_UNF_PVR_PlayResumeChn(mPvrPlayPlaybackHandle);
            if (ErrCode != HI_SUCCESS)
            {
                printf("call HI_UNF_PVR_PlayResumeChn failed.\n");
                return ErrCode;
            }
		}
		else
		{
			DRV_ffmpeg_Resume();
		}
		mPlayRecPlaybackStatus = DRV_PLAYREC_PLAYBACK_RUNNING;
		return DRV_NO_ERROR;
	}
	return -1;
}

DRV_ErrCode DRV_PlayRecSetPlaySpeed(U32 PlaybackId,S32 Speed)
{
	DRV_ErrCode ErrCode;
	S32 TrueSpeed;
	TrueSpeed = Speed*HI_SVR_PLAYER_PLAY_SPEED_NORMAL;
	if(TrueSpeed > HI_SVR_PLAYER_PLAY_SPEED_NORMAL)
		mPlayRecPlaybackStatus = DRV_PLAYREC_PLAYBACK_FORWARD;
	if(TrueSpeed < HI_SVR_PLAYER_PLAY_SPEED_NORMAL)
		mPlayRecPlaybackStatus = DRV_PLAYREC_PLAYBACK_BACKWARD;
	if(TrueSpeed == HI_SVR_PLAYER_PLAY_SPEED_NORMAL)
		mPlayRecPlaybackStatus = DRV_PLAYREC_PLAYBACK_RUNNING;
    if(TrueSpeed == -1024)	
    	TrueSpeed = -2048;
	printf("set speed [%d]\n",TrueSpeed);
	
	if(PlaybackId == 1)
	{
        HI_UNF_PVR_PLAY_MODE_S stTrickMode;
        stTrickMode.enSpeed = TrueSpeed;
        printf("trick mod:%d\n", stTrickMode.enSpeed);
        ErrCode = HI_UNF_PVR_PlayTPlay(mPvrPlayPlaybackHandle, &stTrickMode);
        if (ErrCode != HI_SUCCESS)
        {
            printf("call HI_UNF_PVR_PlayTPlay failed.\n");
            return ErrCode;
        }
	}
	else
	{
		DRV_ffmpeg_SetSpeed(TrueSpeed);
	}
	return DRV_NO_ERROR;
}


DRV_ErrCode DRV_PlayRecGetPlaySpeed(U32 PlaybackId,S32* Speed)
{
	DRV_ErrCode ErrCode;
    HI_UNF_PVR_PLAY_STATUS_S stStatus;
	S32 TrueSpeed;
	if(PlaybackId == 1)
	{
        ErrCode = HI_UNF_PVR_PlayGetStatus(mPvrPlayPlaybackHandle, &stStatus);
        if (HI_SUCCESS != ErrCode)
        {
        	printf("call HI_UNF_PVR_RecGetStatus failed.\n");
            return (ErrCode);
        }
        TrueSpeed = stStatus.enSpeed;
	}
	else
	{
		DRV_ffmpeg_GetSpeed(&TrueSpeed);
	}
	*Speed = TrueSpeed/HI_SVR_PLAYER_PLAY_SPEED_NORMAL;
    if(mPlayRecPlaybackStatus == DRV_PLAYREC_PLAYBACK_PAUSED)
           *Speed = 0;
	return DRV_NO_ERROR;
}

DRV_ErrCode DRV_PlayRecSeekPlay(U32 PlaybackId,S32 TimeInms)
{
	DRV_ErrCode ErrCode;
	if(PlaybackId == 1)
	{
        HI_UNF_PVR_PLAY_POSITION_S stPos;
        stPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
        stPos.s64Offset = TimeInms;
        stPos.s32Whence = SEEK_SET;
        printf("seek to %d ms\n",TimeInms);

        ErrCode = HI_UNF_PVR_PlaySeek(mPvrPlayPlaybackHandle, &stPos);
        if (ErrCode != HI_SUCCESS)
        {
            printf("call HI_UNF_PVR_PlayStep failed.\n");
            return ErrCode;
        }
	}
	else
	{
		DRV_ffmpeg_seek(TimeInms);
	}
	return DRV_NO_ERROR;
}

DRV_ErrCode DRV_PlayRecGetPlayTime(U32 PlaybackId,U32* TatalTimeInms_p,U32* BeginTimeInms_p,U32* EndTimeInms_p,U32* CurrentTimeInms_p)
{
	DRV_ErrCode ErrCode;
    HI_UNF_PVR_PLAY_STATUS_S stStatus;
    HI_UNF_PVR_FILE_ATTR_S fileAttr;
	int time,time1;
	time = 0;
	if(PlaybackId == 1)
	{
        ErrCode = HI_UNF_PVR_PlayGetStatus(mPvrPlayPlaybackHandle, &stStatus);
        if (HI_SUCCESS != ErrCode)
        {
        	printf("call HI_UNF_PVR_RecGetStatus failed.\n");
            return (ErrCode);
        }
    	ErrCode = HI_UNF_PVR_PlayGetFileAttr(mPvrPlayPlaybackHandle, &fileAttr);
        if (HI_SUCCESS != ErrCode)
        {
        	printf("call HI_UNF_PVR_PlayGetFileAttr failed.\n");
            return (ErrCode);
        }
        * EndTimeInms_p     = fileAttr.u32EndTimeInMs;
        * TatalTimeInms_p   = fileAttr.u32EndTimeInMs;
        * BeginTimeInms_p   = fileAttr.u32StartTimeInMs;
        * CurrentTimeInms_p = stStatus.u32CurPlayTimeInMs;
	}
	else
	{
		DRV_ffmpeg_Get_DurationTime(&time);
		* EndTimeInms_p =* TatalTimeInms_p=time;
		* BeginTimeInms_p=0;
		DRV_ffmpeg_Get_NowTime(&time1);
		* CurrentTimeInms_p=time1;
	}
	return DRV_NO_ERROR;
}

DRV_ErrCode DRV_PlayRecGetPlayStatus(U32 PlaybackId,DRV_PlayRecPlaybackStatus_t* Status)
{
	DRV_ErrCode ErrCode;
    HI_UNF_PVR_PLAY_STATUS_S stStatus;

	if(PlaybackId == 1)
	{
        ErrCode = HI_UNF_PVR_PlayGetStatus(mPvrPlayPlaybackHandle, &stStatus);
        if (HI_SUCCESS != ErrCode)
        {
        	printf("call HI_UNF_PVR_RecGetStatus failed.\n");
            return (ErrCode);
        }
	    printf("== pvr play status: \n");
	    if (stStatus.u64CurPlayPos < 1024*1024)
	    {
	        printf("      size:%lld K\n", stStatus.u64CurPlayPos/1024);
	    }
	    else
	    {
	        printf("      size:%lld M\n", stStatus.u64CurPlayPos/1024/1024);
	    }
	    printf("      time:CurPlay %d s\n", stStatus.u32CurPlayTimeInMs/1000);
	    printf("      pvr play u32CurPlayFrame:%d  enSpeed:%d\n", stStatus.u32CurPlayFrame, stStatus.enSpeed);
	
	}
	*Status = mPlayRecPlaybackStatus;
	return DRV_NO_ERROR;
}
DRV_ErrCode DRV_PlayRecPlaybackEnableCallBack(DRV_PlayRecCallback Callback_f)
{
	DRV_ErrCode ErrCode;
	//if(mPlayRecMode == DRV_PLAYREC_MODE_LOCALPLAY)
	{
    	DRV_ffmpegEnableCallback(Callback_f);
    }	
    //else
    {
    	mPlayRecCallback = Callback_f;
    }
    return DRV_NO_ERROR;
}
DRV_ErrCode DRV_PlayRecPlaybackDisableCallBack(void)
{
	DRV_ErrCode ErrCode;
	if(mPlayRecMode == DRV_PLAYREC_MODE_LOCALPLAY)
	{
		DRV_ffmpegDisableCallback();
	}	
    else
    {
    	mPlayRecCallback = NULL;
    }
	
	return DRV_NO_ERROR;
}

HI_S32 drv_SwitchToShiftPlay(const HI_CHAR *pszFileName, HI_U32 *pu32PlayChn,
             HI_HANDLE hAvplay)
{

    PVR_SwitchDmxSource(PVR_DMX_ID_LIVE, PVR_DMX_PORT_ID_PLAYBACK);
    PVR_StartPlayBack(pszFileName, pu32PlayChn, hAvplay);

    return HI_SUCCESS;
}


HI_S32 drv_SwitchToLivePlay(HI_U32 u32PlayChn, HI_HANDLE hAvplay, const PMT_COMPACT_PROG *pProgInfo)
{
    PVR_StopPlayBack(u32PlayChn);


    PVR_SwitchDmxSource(PVR_DMX_ID_LIVE, PVR_DMX_PORT_ID_DVB);
    PVR_StartLivePlay(hAvplay, pProgInfo);
    return HI_SUCCESS;
}
void drv_RecCheckTask()
{
	DRV_ErrCode ErrCode;
    HI_UNF_PVR_REC_STATUS_S stRecStatus;
    while(mRecTaskStatus)
    {
        DRV_OS_TaskDelay_MS(1000);
        ErrCode = HI_UNF_PVR_RecGetStatus(mPlayRecRecordHandle, &stRecStatus);
        if (ErrCode != HI_SUCCESS)
        {
            printf("call HI_UNF_PVR_RecGetStatus failed.\n");
            continue;
        }
        printf("== record status: \n");
        if (stRecStatus.u64CurWritePos < 1024*1024)
        {
            printf("      size:%lld K\n", stRecStatus.u64CurWritePos/1024);
        }
        else
        {
            printf("      size:%lld M\n", stRecStatus.u64CurWritePos/1024/1024);
        }
        printf("      time:from %d to %d, length:%d s\n", stRecStatus.u32StartTimeInMs/1000, stRecStatus.u32EndTimeInMs/1000, stRecStatus.u32CurTimeInMs/1000);
        printf("      record buff:%d/%d \n", stRecStatus.stRecBufStatus.u32UsedSize, stRecStatus.stRecBufStatus.u32BufSize);
        if((mRecLimitTimeInMs != 0)&&(stRecStatus.u32CurTimeInMs >= mRecLimitTimeInMs))
        {
        	
        }
    }
	printf("drv_RecCheckTask has quit!\n");
}

/*这里不处理回调，DRV_PlayRecStartPlay的时候才处理*/
DRV_ErrCode DRV_PlayRecPausePlay2Record(U32 PlaybackId,U32 RecordId,DRV_PlayRecStartRecordParams_t* Params_p,DRV_PlayRecCallback	PlaybackCallback)
{
	DRV_ErrCode ErrCode;
	PMT_COMPACT_PROG pstProgInfo;
    HI_UNF_PVR_REC_ATTR_S 		RecAttr;
	U32 index;
	memset(&pstProgInfo,0,sizeof(PMT_COMPACT_PROG));
    ErrCode = HI_UNF_PVR_RecInit();
	ErrCode |= HI_UNF_PVR_PlayInit();
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_PVR_RecInit failed.\n");
        return ErrCode;
    }
    ErrCode = PVR_RegisterCallBacks();
    if (ErrCode != HI_SUCCESS)
    {
        printf("call PVR_RegisterCallBacks failed.\n");
        return ErrCode;
    }
	mIsTimeShiftStart = FALSE;
	for(index = 0;index < Params_p->PidsNum;index++)
	{
		switch(Params_p->Pids[index].Type)
		{
            case DRV_PLAYREC_STREAMTYPE_MP1V:
    		case DRV_PLAYREC_STREAMTYPE_MP2V:
    		{
	    		pstProgInfo.VideoType = HI_UNF_VCODEC_TYPE_MPEG2;
	    		pstProgInfo.VElementNum = 1;
	    		pstProgInfo.VElementPid = Params_p->Pids[index].Pid;
	    		break;
	    	}	
    		case DRV_PLAYREC_STREAMTYPE_MP4V:
    		{
	    		pstProgInfo.VideoType = HI_UNF_VCODEC_TYPE_MPEG4;
	    		pstProgInfo.VElementNum = 1;
	    		pstProgInfo.VElementPid = Params_p->Pids[index].Pid;
	    		break;
	    	}	
    		case DRV_PLAYREC_STREAMTYPE_H264V:
    		{
	    		pstProgInfo.VideoType = HI_UNF_VCODEC_TYPE_H264;
	    		pstProgInfo.VElementNum = 1;
	    		pstProgInfo.VElementPid = Params_p->Pids[index].Pid;
	    		break;
	    	}	
    		case DRV_PLAYREC_STREAMTYPE_AVS:
    		{
	    		pstProgInfo.VideoType = HI_UNF_VCODEC_TYPE_AVS;
	    		pstProgInfo.VElementNum = 1;
	    		pstProgInfo.VElementPid = Params_p->Pids[index].Pid;
	    		break;
	    	}
    		case DRV_PLAYREC_STREAMTYPE_MP1A:
    		case DRV_PLAYREC_STREAMTYPE_MP2A:
    		{
	    		pstProgInfo.AudioType = HA_AUDIO_ID_MP2;
	    		pstProgInfo.AElementNum = 1;
	    		pstProgInfo.AElementPid = Params_p->Pids[index].Pid;
	    		break;
	    	}	
    		case DRV_PLAYREC_STREAMTYPE_MP4A:
    		{
	    		pstProgInfo.AudioType = HA_AUDIO_ID_AAC;
	    		pstProgInfo.AElementNum = 1;
	    		pstProgInfo.AElementPid = Params_p->Pids[index].Pid;
	    		break;
	    	}	
            case DRV_PLAYREC_STREAMTYPE_AC3:
    		{
	    		pstProgInfo.AudioType = HA_AUDIO_ID_AC3PASSTHROUGH;
	    		pstProgInfo.AElementNum = 1;
	    		pstProgInfo.AElementPid = Params_p->Pids[index].Pid;
	    		break;
	    	}
    		case DRV_PLAYREC_STREAMTYPE_PMT:
    		{
	    		pstProgInfo.PmtPid = Params_p->Pids[index].Pid;
	    		break;
	    	}	
    		case DRV_PLAYREC_STREAMTYPE_PCR:
    		{
	    		pstProgInfo.PcrPid = Params_p->Pids[index].Pid;
	    		break;
	    	}	
    		default:
    		break;
		}
	}

    ErrCode = HI_UNF_DMX_AttachTSPort(PVR_DMX_ID_REC, PVR_DMX_PORT_ID_DVB);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_DMX_AttachTSPort for REC failed.\n");
        PVR_UnRegisterCallBacks();
        HI_UNF_PVR_RecDeInit();
        HI_UNF_PVR_PlayDeInit();
        mPvrPlayPlaybackHandle = 0;
        return ErrCode;
    }
    memset(PlayRecFullName,0,512);
	DRV_VfsPathUserToDrv(Params_p->DstFilePath,PlayRecFullName,FALSE);

    /* Check the size limit params*/
    if((Params_p->SizeLimitInMbyte> 0)&&(Params_p->SizeLimitInMbyte< MAX_TIMESHIFT_REC_FILE_SIZE))
        Params_p->SizeLimitInMbyte = Params_p->SizeLimitInMbyte*1024*1024;
    else
        Params_p->SizeLimitInMbyte = MAX_TIMESHIFT_REC_FILE_SIZE;
    
    ErrCode = PVR_RecStart(PlayRecFullName, &pstProgInfo,PVR_DMX_ID_REC, bRewind, bDoCipher, Params_p->SizeLimitInMbyte, &mPlayRecRecordHandle);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call PVR_StartPlayBack failed.\n");
    	PVR_RecStop(mPlayRecRecordHandle);
        PVR_UnRegisterCallBacks();
        HI_UNF_PVR_RecDeInit();
        HI_UNF_PVR_PlayDeInit();
        mPvrPlayPlaybackHandle = 0;
    	HI_UNF_DMX_DetachTSPort(PVR_DMX_ID_REC); 
        return ErrCode;
    }

    ErrCode = HI_UNF_PVR_RecGetChn(mPlayRecRecordHandle,&RecAttr);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_PVR_PlayPauseChn failed.\n");
    	PVR_RecStop(mPlayRecRecordHandle);
        PVR_UnRegisterCallBacks();
        HI_UNF_PVR_RecDeInit();
        HI_UNF_PVR_PlayDeInit();
        mPvrPlayPlaybackHandle = 0;
    	HI_UNF_DMX_DetachTSPort(PVR_DMX_ID_REC);
        return ErrCode;
    }    
    DRV_OS_TaskDelay_MS(1000);
    ErrCode = drv_SwitchToShiftPlay(RecAttr.szFileName, &mPvrPlayPlaybackHandle, mAvPlayHandle);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_PVR_PlayPauseChn failed.\n");
        PVR_StopPlayBack(mPvrPlayPlaybackHandle);
    	PVR_RecStop(mPlayRecRecordHandle);
        PVR_UnRegisterCallBacks();
        HI_UNF_PVR_RecDeInit();
        HI_UNF_PVR_PlayDeInit();
        mPvrPlayPlaybackHandle = 0;
    	HI_UNF_DMX_DetachTSPort(PVR_DMX_ID_REC);
        return ErrCode;
    }    
    ErrCode = HI_UNF_PVR_PlayPauseChn(mPvrPlayPlaybackHandle);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_PVR_PlayPauseChn failed.\n");
        PVR_StopPlayBack(mPvrPlayPlaybackHandle);
    	PVR_RecStop(mPlayRecRecordHandle);
        PVR_UnRegisterCallBacks();
        HI_UNF_PVR_RecDeInit();
        HI_UNF_PVR_PlayDeInit();
        mPvrPlayPlaybackHandle = 0;
    	HI_UNF_DMX_DetachTSPort(PVR_DMX_ID_REC);        
        return ErrCode;
    }
#if 0    
    mRecLimitTimeInMs = Params_p->TimeLimitInMinute;
    /*create a thread for rec status*/
	pRecThread = DRV_OS_TaskCreate("drv_RecCheckTask",drv_RecCheckTask,3,8096,NULL,0);
    if (pRecThread == NULL)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, ErrCode);
        return ErrCode;
    }
#endif
    mPlayRecMode = DRV_PLAYREC_MODE_PVRTIMESHIFT;
    mPlayRecPlaybackStatus = DRV_PLAYREC_PLAYBACK_PAUSED;
    if(Params_p->Callback != NULL)
    	mPlayRecCallback = Params_p->Callback;
    return(DRV_NO_ERROR);
}
DRV_ErrCode DRV_PlayRecStartRecord(U32 RecordId,DRV_PlayRecStartRecordParams_t *Params_p)
{
	DRV_ErrCode ErrCode;
	PMT_COMPACT_PROG pstProgInfo;
	U32 index;
	memset(&pstProgInfo,0,sizeof(PMT_COMPACT_PROG));
    ErrCode = HI_UNF_PVR_RecInit();
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_PVR_RecInit failed.\n");
        return ErrCode;
    }
    ErrCode = PVR_RegisterCallBacks();
    if (ErrCode != HI_SUCCESS)
    {
        printf("call PVR_RegisterCallBacks failed.\n");
        return ErrCode;
    }
	//DRV_AV_UnMuteAudio();
	for(index = 0;index < Params_p->PidsNum;index++)
	{
		switch(Params_p->Pids[index].Type)
		{
            case DRV_PLAYREC_STREAMTYPE_MP1V:
    		case DRV_PLAYREC_STREAMTYPE_MP2V:
    		{
	    		pstProgInfo.VideoType = HI_UNF_VCODEC_TYPE_MPEG2;
	    		pstProgInfo.VElementNum = 1;
	    		pstProgInfo.VElementPid = Params_p->Pids[index].Pid;
	    		break;
	    	}	
    		case DRV_PLAYREC_STREAMTYPE_MP4V:
    		{
	    		pstProgInfo.VideoType = HI_UNF_VCODEC_TYPE_MPEG4;
	    		pstProgInfo.VElementNum = 1;
	    		pstProgInfo.VElementPid = Params_p->Pids[index].Pid;
	    		break;
	    	}	
    		case DRV_PLAYREC_STREAMTYPE_H264V:
    		{
	    		pstProgInfo.VideoType = HI_UNF_VCODEC_TYPE_H264;
	    		pstProgInfo.VElementNum = 1;
	    		pstProgInfo.VElementPid = Params_p->Pids[index].Pid;
	    		break;
	    	}	
    		case DRV_PLAYREC_STREAMTYPE_AVS:
    		{
	    		pstProgInfo.VideoType = HI_UNF_VCODEC_TYPE_AVS;
	    		pstProgInfo.VElementNum = 1;
	    		pstProgInfo.VElementPid = Params_p->Pids[index].Pid;
	    		break;
	    	}
    		case DRV_PLAYREC_STREAMTYPE_MP1A:
    		case DRV_PLAYREC_STREAMTYPE_MP2A:
    		{
	    		pstProgInfo.AudioType = HA_AUDIO_ID_MP2;
	    		pstProgInfo.AElementNum = 1;
	    		pstProgInfo.AElementPid = Params_p->Pids[index].Pid;
	    		break;
	    	}	
    		case DRV_PLAYREC_STREAMTYPE_MP4A:
    		{
	    		pstProgInfo.AudioType = HA_AUDIO_ID_AAC;
	    		pstProgInfo.AElementNum = 1;
	    		pstProgInfo.AElementPid = Params_p->Pids[index].Pid;
	    		break;
	    	}	
            case DRV_PLAYREC_STREAMTYPE_AC3:
    		{
	    		pstProgInfo.AudioType = HA_AUDIO_ID_AC3PASSTHROUGH;
	    		pstProgInfo.AElementNum = 1;
	    		pstProgInfo.AElementPid = Params_p->Pids[index].Pid;
	    		break;
	    	}
            
    		case DRV_PLAYREC_STREAMTYPE_PMT:
    		{
	    		pstProgInfo.PmtPid = Params_p->Pids[index].Pid;
	    		break;
	    	}	
    		case DRV_PLAYREC_STREAMTYPE_PCR:
    		{
	    		pstProgInfo.PcrPid = Params_p->Pids[index].Pid;
	    		break;
	    	}	
    		default:
    		break;
		}
	}
    ErrCode = HI_UNF_DMX_AttachTSPort(PVR_DMX_ID_REC, PVR_DMX_PORT_ID_DVB);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_DMX_AttachTSPort for REC failed.\n");
        PVR_UnRegisterCallBacks();
        HI_UNF_PVR_RecDeInit();
        return ErrCode;
    }
	memcpy(&RecPstProgInfo,&pstProgInfo,sizeof(RecPstProgInfo));
    memset(PlayRecFullName,0,512);
	DRV_VfsPathUserToDrv(Params_p->DstFilePath,PlayRecFullName,FALSE);	
    ErrCode = PVR_RecStart(PlayRecFullName, &pstProgInfo,PVR_DMX_ID_REC, bRewind, bDoCipher, MAX_REC_FILE_SIZE, &mPlayRecRecordHandle);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call PVR_StartPlayBack failed.\n");
    	PVR_RecStop(mPlayRecRecordHandle);
        PVR_UnRegisterCallBacks();
        HI_UNF_PVR_RecDeInit();
        HI_UNF_PVR_PlayDeInit();
        mPvrPlayPlaybackHandle = 0;
    	HI_UNF_DMX_DetachTSPort(PVR_DMX_ID_REC);        
        return ErrCode;
    }
#if 0    
    mRecLimitTimeInMs = Params_p->TimeLimitInMinute;
    /*create a thread for rec status*/
	pRecThread = DRV_OS_TaskCreate("drv_RecCheckTask",drv_RecCheckTask,3,8096,NULL,0);
    if (pRecThread == NULL)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, ErrCode);
        return ErrCode;
    }
#endif
	mPlayRecMode = DRV_PLAYREC_MODE_PVRREC;
	mPlayRecRecordStatus = DRV_PLAYREC_RECORD_RECORDING;
    if(Params_p->Callback != NULL)
		mPlayRecCallback = Params_p->Callback;

    return(DRV_NO_ERROR);
}

DRV_ErrCode DRV_PlayRecStopRecord(U32 RecordId)
{
    DRV_ErrCode ErrCode;
#if 0    
	while(1)
	{
		ErrCode = DRV_OS_TaskWait(pRecThread, DRVOS_WAIT_FINITY, 500); 
		if(ErrCode == 0)
		break;
		DRV_OS_TaskKill(pRecThread, 0, 0);
		DRV_OS_TaskDelay_MS(10); 
	}
	DRV_OSTaskDelete(pRecThread);
    pRecThread = NULL;
#endif
    ErrCode = PVR_RecStop(mPlayRecRecordHandle);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call PVR_StartPlayBack failed.\n");
        return ErrCode;
    }
    ErrCode = PVR_UnRegisterCallBacks();
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_DMX_DetachTSPort failed.\n");
        return ErrCode;
    }
    ErrCode = HI_UNF_PVR_RecDeInit();
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_DMX_DetachTSPort failed.\n");
        return ErrCode;
    }
    ErrCode = HI_UNF_DMX_DetachTSPort(PVR_DMX_ID_REC);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_DMX_DetachTSPort failed.\n");
        return ErrCode;
    }
    mPlayRecRecordHandle = 0;
    mPlayRecMode = DRV_PLAYREC_MODE_NONE;
    mPlayRecRecordStatus = DRV_PLAYREC_RECORD_STOPPED;
    return(DRV_NO_ERROR);
}

DRV_ErrCode DRV_PlayRecGetRecordTime(U32 RecordId,U32* TimeInms_p)
{
    DRV_ErrCode ErrCode;
    HI_UNF_PVR_REC_STATUS_S stRecStatus;

    ErrCode = HI_UNF_PVR_RecGetStatus(mPlayRecRecordHandle, &stRecStatus);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_PVR_RecGetStatus failed.\n");
        return ErrCode;
    }
    else
    {
    	*TimeInms_p = stRecStatus.u32CurTimeInMs;
	    printf("== record status: \n");
	    if (stRecStatus.u64CurWritePos < 1024*1024)
	    {
	        printf("      size:%lld K\n", stRecStatus.u64CurWritePos/1024);
	    }
	    else
	    {
	        printf("      size:%lld M\n", stRecStatus.u64CurWritePos/1024/1024);
	    }
	    printf("      time:from %d to %d, length:%d s\n", stRecStatus.u32StartTimeInMs/1000, stRecStatus.u32EndTimeInMs/1000, stRecStatus.u32CurTimeInMs/1000);
	    printf("      record buff:%d/%d \n", stRecStatus.stRecBufStatus.u32UsedSize, stRecStatus.stRecBufStatus.u32BufSize);
   }
    return(ErrCode);
}


DRV_ErrCode DRV_PlayRecGetRecordStatus(U32 RecordId,DRV_PlayRecRecordStatus_t* Status)
{
    DRV_ErrCode ErrCode;
    HI_UNF_PVR_REC_STATUS_S stRecStatus;
    HI_CHAR timestr[20];

    ErrCode = HI_UNF_PVR_RecGetStatus(mPlayRecRecordHandle, &stRecStatus);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_PVR_RecGetStatus failed.\n");
        return ErrCode;
    }
    printf("== record status: \n");
    if (stRecStatus.u64CurWritePos < 1024*1024)
    {
        printf("      size:%lld K\n", stRecStatus.u64CurWritePos/1024);
    }
    else
    {
        printf("      size:%lld M\n", stRecStatus.u64CurWritePos/1024/1024);
    }
    printf("      time:from %d to %d, length:%d s\n", stRecStatus.u32StartTimeInMs/1000, stRecStatus.u32EndTimeInMs/1000, stRecStatus.u32CurTimeInMs/1000);
    printf("      record buff:%d/%d \n", stRecStatus.stRecBufStatus.u32UsedSize, stRecStatus.stRecBufStatus.u32BufSize);
    switch(stRecStatus.enState)
    {
		case HI_UNF_PVR_REC_STATE_RUNNING:        /**< 录制中。      */
			*Status = DRV_PLAYREC_RECORD_RECORDING;
		case HI_UNF_PVR_REC_STATE_PAUSE:          /**< 暂停中        */
			*Status = DRV_PLAYREC_RECORD_PAUSED;
		case HI_UNF_PVR_REC_STATE_STOP:           /**< 已经停止。    */
			*Status = DRV_PLAYREC_RECORD_STOPPED;
		case HI_UNF_PVR_REC_STATE_STOPPING:       /**< 正在停止。    */
		case HI_UNF_PVR_REC_STATE_INVALID:        /**< 未初始化。    */
		case HI_UNF_PVR_REC_STATE_INIT:           /**< 初始化。      */
		case HI_UNF_PVR_REC_STATE_BUTT:           /**< 无效的状态值。*/
		default:
			*Status = DRV_PLAYREC_RECORD_NONE;
    }
    *Status = mPlayRecRecordStatus;
    return(ErrCode);
}

DRV_ErrCode DRV_PlayRecPauseRecord(U32 RecordId)
{
    DRV_ErrCode ErrCode;
    //ErrCode = PVR_RecStop(mPlayRecRecordHandle);
    //if (ErrCode != HI_SUCCESS)
    //{
    //    printf("call PVR_StartPlayBack failed.\n");
    //    return ErrCode;
    //}
	mPlayRecRecordStatus = DRV_PLAYREC_RECORD_PAUSED;
    return DRV_NO_ERROR;
}

DRV_ErrCode DRV_PlayRecResumeRecord(U32 RecordId)
{
    DRV_ErrCode ErrCode;
    //ErrCode = PVR_RecStart("/mnt", &RecPstProgInfo,PVR_DMX_ID_REC, bRewind, bDoCipher, MAX_REC_FILE_SIZE, &mPlayRecRecordHandle);
    //if (ErrCode != HI_SUCCESS)
    //{
    //    printf("call PVR_StartPlayBack failed.\n");
    //    return ErrCode;
    //}
	mPlayRecRecordStatus = DRV_PLAYREC_RECORD_RECORDING;
    return DRV_NO_ERROR;
}


