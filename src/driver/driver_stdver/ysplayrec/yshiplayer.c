#ifndef STTBX_PRINT
#define STTBX_PRINT
#endif
#include "drv_basic.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "hi_audio_codec.h"
#include "hi_unf_common.h"
#include "hi_unf_avplay.h"
#include "hi_unf_demux.h"
#include "hi_unf_so.h"
#include "hi_unf_sound.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_go.h"
//#include "hi_adp_audio.h"
//#include "hi_adp_mpi.h"
#include "hi_svr_player.h"
#include "hi_svr_format.h"

#include "hi_svr_assa.h"
#include "hi_unf_pvr.h"
#include "ysav.h"
#include "yshiplayer.h"
/*使用avplayer而不是自己创建*/
#define USE_AVPLAYER_BUT_CREATE
#define CMD_LEN            (512)
#define ASTREAM_FILE       "./astream.file"
#define ASTREAM_IDX_FILE   "./astream_idx.file"
#define VSTREAM_FILE       "./vstream.file"
#define VSTREAM_IDX_FILE   "./vstream_idx.file"
#ifndef BOARD_TYPE_fpga
#define DEFAULT_DVB_PORT 0
#else
#define DEFAULT_DVB_PORT 1
#endif

static int UsbFlag = 1;
typedef struct DRV_HiPlayer_s
{
    U8 HiPlayerFileName[256];
	DRV_AvSynMode HiPlayerSyncMode;
	DRVOS_Sem_t   *HiPlayerSemLock;
	HI_FORMAT_STREAM_TYPE_E HiPlayerStreamType;
    U32 HiPlayerDuration;
    U32 HiPlayerCurrentProgress;
	U32 HiPlayerSpeed;
	U32 HiPlayerSeekTime;
	U32 HiPlayerBwSpeed;
	DRV_PlayRecCallback HiPlayerCallback;
	HI_HANDLE HiPlayerHandle;
	HI_HANDLE HiPlayerAvHandle;
	HI_HANDLE HiPlayerWindowHandle;
	DRV_HiPlayerStatus HiPlayerStatus;
}DRV_HiPlayer_t;

static DRV_HiPlayer_t mHiPlayer;

static HI_SVR_PLAYER_PARAM_S s_stParam = {
        0,
        3,
        0,
        0,
        1920,
        1080,
        100
};
HI_HANDLE  mFfmpegPlayHandle;

static HI_SVR_PLAYER_STATE_E mHiPlayerStatus = HI_SVR_PLAYER_STATE_BUTT;

HI_SVR_PLAYER_STATE_E DRV_GetHiplayerStatus()
{
	return mHiPlayerStatus;
}


void DRV_ffmpeg_SetUsbFlag(int flag)
{
    UsbFlag = flag;

    return;
}


static HI_CHAR *getVidFormatStr(HI_U32 u32Format)
{   
    switch (u32Format)
    {
    case HI_UNF_VCODEC_TYPE_MPEG2:
        return "MPEG2";
        break;
        
    case HI_UNF_VCODEC_TYPE_MPEG4:
        return "MPEG4";
        break;
        
    case HI_UNF_VCODEC_TYPE_AVS:
        return "AVS";
        break;
        
    case HI_UNF_VCODEC_TYPE_H263:
        return "H263";
        break;
        
    case HI_UNF_VCODEC_TYPE_H264:
        return "H264";
        break;
        
    case HI_UNF_VCODEC_TYPE_REAL8:
        return "REAL8";
        break;
        
    case HI_UNF_VCODEC_TYPE_REAL9:
        return "REAL9";
        break;
        
    case HI_UNF_VCODEC_TYPE_VC1:
        return "VC1";
        break;
        
    case HI_UNF_VCODEC_TYPE_VP6:
        return "VP6";
        break;
        
    case HI_UNF_VCODEC_TYPE_DIVX3:
        return "DIVX3";
        break;
        
    case HI_UNF_VCODEC_TYPE_JPEG:
        return "JPEG";
        break;

    case HI_UNF_VCODEC_TYPE_SORENSON:
        return "SORENSON";
        break;

    case HI_UNF_VCODEC_TYPE_VP6F:
        return "VP6F";
        break;

    case HI_UNF_VCODEC_TYPE_VP6A:
        return "VP6A";
        break;

    default:
        return "UN-KNOWN";
        break;
    }

    return "UN-KNOWN";
}

static HI_CHAR *getAudFormatStr(HI_U32 u32Format)
{   
    switch (u32Format)
    {
    case FORMAT_MP2:
        return "MPEG2";
        break;
    case FORMAT_MP3:
        return "MPEG3";
        break;
    case FORMAT_AAC:
        return "AAC";
        break;
    case FORMAT_AC3:
        return "AC3";
        break;
    case FORMAT_DTS:
        return "DTS";
        break;
    case FORMAT_VORBIS:
        return "VORBIS";
        break;
    case FORMAT_DVAUDIO:
        return "DVAUDIO";
        break;
    case FORMAT_WMAV1:
        return "WMAV1";
        break;
    case FORMAT_WMAV2:
        return "WMAV2";
        break;
    case FORMAT_MACE3:
        return "MACE3";
        break;
    case FORMAT_MACE6:
        return "MACE6";
        break;
    case FORMAT_VMDAUDIO:
        return "VMDAUDIO";
        break;
    case FORMAT_SONIC:
        return "SONIC";
        break;
    case FORMAT_SONIC_LS:
        return "SONIC_LS";
        break;
    case FORMAT_FLAC:
        return "FLAC";
        break;
    case FORMAT_MP3ADU:
        return "MP3ADU";
        break;
    case FORMAT_MP3ON4:
        return "MP3ON4";
        break;
    case FORMAT_SHORTEN:
        return "SHORTEN";
        break;
    case FORMAT_ALAC:
        return "ALAC";
        break;
    case FORMAT_WESTWOOD_SND1:
        return "WESTWOOD_SND1";
        break;
    case FORMAT_GSM:
        return "GSM";
        break;
    case FORMAT_QDM2:
        return "QDM2";
        break;
    case FORMAT_COOK:
        return "COOK";
        break;
    case FORMAT_TRUESPEECH:
        return "TRUESPEECH";
        break;
    case FORMAT_TTA:
        return "TTA";
        break;
    case FORMAT_SMACKAUDIO:
        return "SMACKAUDIO";
        break;
    case FORMAT_QCELP:
        return "QCELP";
        break;
    case FORMAT_WAVPACK:
        return "WAVPACK";
        break;
    case FORMAT_DSICINAUDIO:
        return "DSICINAUDIO";
        break;
    case FORMAT_IMC:
        return "IMC";
        break;
    case FORMAT_MUSEPACK7:
        return "MUSEPACK7";
        break;
    case FORMAT_MLP:
        return "MLP";
        break;
    case FORMAT_GSM_MS:
        return "GSM_MS";
        break;
    case FORMAT_ATRAC3:
        return "ATRAC3";
        break;
    case FORMAT_VOXWARE:
        return "VOXWARE";
        break;
    case FORMAT_APE:
        return "APE";
        break;
    case FORMAT_NELLYMOSER:
        return "NELLYMOSER";
        break;
    case FORMAT_MUSEPACK8:
        return "MUSEPACK8";
        break;
    case FORMAT_SPEEX:
        return "SPEEX";
        break;
    case FORMAT_WMAVOICE:
        return "WMAVOICE";
        break;
    case FORMAT_WMAPRO:
        return "WMAPRO";
        break;
    case FORMAT_WMALOSSLESS:
        return "WMALOSSLESS";
        break;
    case FORMAT_ATRAC3P:
        return "ATRAC3P";
        break;
    case FORMAT_EAC3:
        return "EAC3";
        break;
    case FORMAT_SIPR:
        return "SIPR";
        break;
    case FORMAT_MP1:
        return "MP1";
        break;
    case FORMAT_TWINVQ:
        return "TWINVQ";
        break;
    case FORMAT_TRUEHD:
        return "TRUEHD";
        break;
    case FORMAT_MP4ALS:
        return "MP4ALS";
        break;
    case FORMAT_ATRAC1:
        return "ATRAC1";
        break;
    case FORMAT_BINKAUDIO_RDFT:
        return "BINKAUDIO_RDFT";
        break;
    case FORMAT_BINKAUDIO_DCT:
        return "BINKAUDIO_DCT";
        break;
    case FORMAT_DRA:
        return "DRA";
        break;

    case FORMAT_PCM: /* various PCM "codecs" */
        return "PCM";
        break;

    case FORMAT_ADPCM: /* various ADPCM codecs */
        return "ADPCM";
        break;

    case FORMAT_AMR_NB: /* AMR */
        return "AMR_NB";
        break;
    case FORMAT_AMR_WB:
        return "AMR_WB";
        break;
    case FORMAT_AMR_AWB:
        return "AMR_AWB";
        break;
    
    case FORMAT_RA_144: /* RealAudio codecs*/
        return "RA_144";
        break;
    case FORMAT_RA_288:
        return "RA_288";
        break;

    case FORMAT_DPCM: /* various DPCM codecs */
        return "DPCM";
        break;

    case FORMAT_G711:  /* various G.7xx codecs */
        return "G711";
        break;
    case FORMAT_G722:
        return "G722";
        break;
    case FORMAT_G7231:
        return "G7231";
        break;
    case FORMAT_G726:
        return "G726";
        break;
    case FORMAT_G728:
        return "G728";
        break;
    case FORMAT_G729AB:
        return "G729AB";
        break;
    default:
        break;
    }

    return "UN-KNOWN";
}

static HI_CHAR *getSubFormatStr(HI_U32 u32Format)
{   
    switch (u32Format)
    {
    case HI_FORMAT_SUBTITLE_DATA_TEXT:
        return "TEXT";
        break;
        
    case HI_FORMAT_SUBTITLE_DATA_BMP:
        return "BMP";
        break;
        
    case HI_FORMAT_SUBTITLE_DATA_ASS:
        return "ASS";
        break;

    case HI_FORMAT_SUBTITLE_DATA_HDMV_PGS:
        return "HDMV_PGS";
        break;

    default:
        return "UN-KNOWN";
        break;
    }

    return "UN-KNOWN";
}
#if 0
static HI_S32 eventCallBack(HI_HANDLE hPlayer, HI_SVR_PLAYER_EVENT_S *pstruEvent)
{
    HI_U32 *pu32Progress = NULL;
	DRV_ErrCode ErrCode = HI_SUCCESS;
    HI_SVR_PLAYER_STATE_E eEventBk = HI_SVR_PLAYER_STATE_BUTT;
    HI_SVR_PLAYER_STREAMID_S *pstSreamId = NULL;
    HI_HANDLE hWindow = (HI_HANDLE)NULL;

    if (0 == hPlayer || NULL == pstruEvent)
    {
        return HI_SUCCESS;
    }

    if (HI_SVR_PLAYER_EVENT_STATE == (HI_SVR_PLAYER_EVENT_E)pstruEvent->eEvent)
    {
        eEventBk = (HI_SVR_PLAYER_STATE_E)*pstruEvent->pu8Data;
        printf("Status change to %d \n", eEventBk);

        if (eEventBk == HI_SVR_PLAYER_STATE_STOP)
        {
            ErrCode = HI_SVR_PLAYER_GetParam(hPlayer, HI_SVR_PLAYER_ATTR_WINDOW_HDL, &hWindow);
		    if (HI_SUCCESS != ErrCode)
		    {
		        printf("eventCallBack HI_SVR_PLAYER_GetParam ErrCode 0x%08x \n", ErrCode);
		    }
            ErrCode = HI_UNF_VO_ResetWindow(hWindow, HI_UNF_WINDOW_FREEZE_MODE_BLACK);
		    if (HI_SUCCESS != ErrCode)
		    {
		        printf("eventCallBack HI_UNF_VO_ResetWindow ErrCode 0x%08x \n", ErrCode);
		    }
        }
    }
    else if (HI_SVR_PLAYER_EVENT_SOF == (HI_SVR_PLAYER_EVENT_E)pstruEvent->eEvent)
    {
        printf("File position is start of file! \n");
        /* 快退到头重新启动播放 */
        eEventBk = (HI_SVR_PLAYER_STATE_E)*pstruEvent->pu8Data;

        if (HI_SVR_PLAYER_STATE_BACKWARD == eEventBk)
        {
            printf("backward to start of file, start play! \n");
            ErrCode = HI_SVR_PLAYER_Play(hPlayer);
 		    if (HI_SUCCESS != ErrCode)
		    {
		        printf("eventCallBack HI_SVR_PLAYER_Play ErrCode 0x%08x \n", ErrCode);
		    }
       }
    }
    else if (HI_SVR_PLAYER_EVENT_EOF == (HI_SVR_PLAYER_EVENT_E)pstruEvent->eEvent)
    {
        printf("File postion is end of file, clear last frame and replay! \n");
        ErrCode = HI_SVR_PLAYER_GetParam(hPlayer, HI_SVR_PLAYER_ATTR_WINDOW_HDL, &hWindow);
	    if (HI_SUCCESS != ErrCode)
	    {
	        printf("eventCallBack HI_SVR_PLAYER_GetParam ErrCode 0x%08x \n", ErrCode);
	    }
        ErrCode = HI_UNF_VO_ResetWindow(hWindow, HI_UNF_WINDOW_FREEZE_MODE_BLACK);
	    if (HI_SUCCESS != ErrCode)
	    {
	        printf("eventCallBack HI_UNF_VO_ResetWindow ErrCode 0x%08x \n", ErrCode);
	    }
        ErrCode = HI_SVR_PLAYER_Play(hPlayer);
	    if (HI_SUCCESS != ErrCode)
	    {
	        printf("eventCallBack HI_SVR_PLAYER_Play ErrCode 0x%08x \n", ErrCode);
	    }
    }
    else if (HI_SVR_PLAYER_EVENT_STREAMID_CHG == (HI_SVR_PLAYER_EVENT_E)pstruEvent->eEvent)
    {
        pstSreamId = (HI_SVR_PLAYER_STREAMID_S*)pstruEvent->pu8Data;

        if (NULL != pstSreamId)
        {
            printf("Stream id change to: ProgramId %d, vid %d, aid %d, sid %d \n",
                pstSreamId->u32ProgramId,
                pstSreamId->u32VStreamId,
                pstSreamId->u32AStreamId,
                pstSreamId->u32SubTitleId);
        }
    }
    else if (HI_SVR_PLAYER_EVENT_PROGRESS == (HI_SVR_PLAYER_EVENT_E)pstruEvent->eEvent)
    {
        pu32Progress = (HI_U32*)pstruEvent->pu8Data;
        printf("Current progress is %d \n", *pu32Progress);
        mHiPlayer.HiPlayerCurrentProgress = *pu32Progress;
    }

    return HI_SUCCESS;
}
#else
static BOOL mIsHiplayerStopNow = FALSE;
DRV_ErrCode drv_ffmpeg_CallBack(HI_HANDLE hPlayer, HI_SVR_PLAYER_EVENT_S *pstruEvent)
{
    HI_U32 *pu32Progress = NULL;
	DRV_ErrCode ErrCode = HI_SUCCESS;
    HI_SVR_PLAYER_STATE_E eEventBk = HI_SVR_PLAYER_STATE_BUTT;
    HI_SVR_PLAYER_STREAMID_S *pstSreamId = NULL;
    HI_HANDLE hWindow = (HI_HANDLE)NULL;

    if (0 == hPlayer || NULL == pstruEvent)
    {
        return HI_SUCCESS;
    }

    if (HI_SVR_PLAYER_EVENT_STATE == (HI_SVR_PLAYER_EVENT_E)pstruEvent->eEvent)
    {
        eEventBk = (HI_SVR_PLAYER_STATE_E)*pstruEvent->pu8Data;
        printf("Status change to %d \n", eEventBk);

		mHiPlayerStatus = eEventBk;
        if (eEventBk == HI_SVR_PLAYER_STATE_STOP)
        {
        	;
        }
    }
    else if (HI_SVR_PLAYER_EVENT_SOF == (HI_SVR_PLAYER_EVENT_E)pstruEvent->eEvent)
    {
        printf("File position is start of file! \n");
        /* 快退到头重新启动播放 */
        eEventBk = (HI_SVR_PLAYER_STATE_E)*pstruEvent->pu8Data;

        if (HI_SVR_PLAYER_STATE_BACKWARD == eEventBk)
        {
            printf("backward to start of file, start play! \n");
            mIsHiplayerStopNow = TRUE;
			if(mHiPlayer.HiPlayerCallback != NULL)
			{
				mHiPlayer.HiPlayerCallback(DRV_PLAYREC_REACH_BEGIN_OF_PLAYBACK_EVT,NULL);
			}       
       	}
    }
    else if (HI_SVR_PLAYER_EVENT_EOF == (HI_SVR_PLAYER_EVENT_E)pstruEvent->eEvent)
    {
        printf("File postion is end of file, clear last frame and replay! \n");
        mIsHiplayerStopNow = TRUE;
		if(mHiPlayer.HiPlayerCallback != NULL)
		{
			mHiPlayer.HiPlayerCallback(DRV_PLAYREC_REACH_END_OF_PLAYBACK_EVT,NULL);
		}
    }
    else if (HI_SVR_PLAYER_EVENT_STREAMID_CHG == (HI_SVR_PLAYER_EVENT_E)pstruEvent->eEvent)
    {
        pstSreamId = (HI_SVR_PLAYER_STREAMID_S*)pstruEvent->pu8Data;

        if (NULL != pstSreamId)
        {
            printf("Stream id change to: ProgramId %d, vid %d, aid %d, sid %d \n",
                pstSreamId->u32ProgramId,
                pstSreamId->u32VStreamId,
                pstSreamId->u32AStreamId,
                pstSreamId->u32SubTitleId);
        }
    }
    else if (HI_SVR_PLAYER_EVENT_PROGRESS == (HI_SVR_PLAYER_EVENT_E)pstruEvent->eEvent)
    {
        pu32Progress = (HI_U32*)pstruEvent->pu8Data;
        //printf("Current progress is %d \n", *pu32Progress);
        mHiPlayer.HiPlayerCurrentProgress = *pu32Progress;
    }

    return HI_SUCCESS;
}
#endif
static HI_S32 outFileInfo(HI_FORMAT_FILE_INFO_S *pstFileInfo)
{
    HI_S32 i = 0, j = 0;
#if 0//add by jyshan
    HI_UNF_PVR_FILE_ATTR_S fileAttr;
	DRV_ErrCode ErrCode = HI_SUCCESS;

	ErrCode = HI_UNF_PVR_PlayGetFileAttr(mFfmpegPlayHandle, &fileAttr);
    if (HI_SUCCESS == ErrCode)
    {
        pstFileInfo->s64Duration   = fileAttr.u32EndTimeInMs;
        pstFileInfo->s64StartTime   = fileAttr.u32StartTimeInMs;
    }
    else
    {
    	printf("call outFileInfo HI_UNF_PVR_PlayGetFileAttr failed.\n");
    }
#endif

    printf("\n\n*************************************************\n");

    printf("File name:   %s \n", pstFileInfo->aszFileName);

    if (HI_FORMAT_STREAM_TS == pstFileInfo->eStreamType)
        printf("Stream type: %s \n", "TS");
    else
        printf("Stream type: %s \n", "ES");   
    mHiPlayer.HiPlayerStreamType = pstFileInfo->eStreamType;
    printf("File size:   %lld bytes \n", pstFileInfo->s64FileSize);
    printf("Start time:  %lld:%lld:%lld \n",
        pstFileInfo->s64StartTime / (1000 * 3600),
        (pstFileInfo->s64StartTime % (1000 * 3600)) / (1000 * 60),
        ((pstFileInfo->s64StartTime % (1000 * 3600)) % (1000 * 60)) / 1000);
    printf("Duration:    %lld:%lld:%lld \n",
        pstFileInfo->s64Duration / (1000 * 3600),
        (pstFileInfo->s64Duration % (1000 * 3600)) / (1000 * 60),
        ((pstFileInfo->s64Duration % (1000 * 3600)) % (1000 * 60)) / 1000);
    mHiPlayer.HiPlayerDuration = (U32)pstFileInfo->s64Duration;   
    printf("bps:         %d bits/s \n", pstFileInfo->u32Bps);
    printf("Album:       %s \n", pstFileInfo->aszAlbum);
    printf("Title:       %s \n", pstFileInfo->aszTitle);
    printf("Artist:      %s \n", pstFileInfo->aszArtist);
    printf("Genre:       %s \n", pstFileInfo->aszGenre);
    printf("Comments:    %s \n", pstFileInfo->aszComments);
    printf("Create time: %s \n", pstFileInfo->aszTime);

    for (i = 0; i < pstFileInfo->u32ProgramNum; i++)
    {
        printf("\nProgram %d: \n", i);
        printf("   video info: \n");

        if (HI_FORMAT_INVALID_STREAM_ID != pstFileInfo->astProgramInfo[i].stVidStream.s32StreamIndex)
        {
            printf("     stream idx:   %d \n", pstFileInfo->astProgramInfo[i].stVidStream.s32StreamIndex);
            printf("     format:       %s \n", getVidFormatStr(pstFileInfo->astProgramInfo[i].stVidStream.u32Format));
            printf("     w * h:        %d * %d \n",
                pstFileInfo->astProgramInfo[i].stVidStream.u16Width,
                pstFileInfo->astProgramInfo[i].stVidStream.u16Height);
            printf("     fps:          %d.%d \n",
                pstFileInfo->astProgramInfo[i].stVidStream.u16FpsInteger,
                pstFileInfo->astProgramInfo[i].stVidStream.u16FpsDecimal);
            printf("     bps:          %d bits/s \n", pstFileInfo->astProgramInfo[i].stVidStream.u32Bps);
        }
        else
        {
            printf("     video stream is null. \n");
        }

        for (j = 0; j < pstFileInfo->astProgramInfo[i].u32AudStreamNum; j++)
        {
            printf("   audio %d info: \n", j);
            printf("     stream idx:   %d \n", pstFileInfo->astProgramInfo[i].astAudStream[j].s32StreamIndex);
            printf("     format:       %s \n", getAudFormatStr(pstFileInfo->astProgramInfo[i].astAudStream[j].u32Format));
            printf("     version:      0x%x \n", pstFileInfo->astProgramInfo[i].astAudStream[j].u32Version);
            printf("     samplerate:   %d Hz \n", pstFileInfo->astProgramInfo[i].astAudStream[j].u32SampleRate);
            printf("     bitpersample: %d \n", pstFileInfo->astProgramInfo[i].astAudStream[j].u16BitPerSample);
            printf("     channels:     %d \n", pstFileInfo->astProgramInfo[i].astAudStream[j].u16Channels);
            printf("     bps:          %d bits/s \n", pstFileInfo->astProgramInfo[i].astAudStream[j].u32Bps);
            printf("     lang:         %s \n", pstFileInfo->astProgramInfo[i].astAudStream[j].aszAudLang);
        }

        for (j = 0; j < pstFileInfo->astProgramInfo[i].u32SubTitleNum; j++)
        {
            printf("   subtitle %d info: \n", j);
            printf("     stream idx:   %d \n", pstFileInfo->astProgramInfo[i].astSubTitle[j].s32StreamIndex);
            printf("     sub type:     %s \n", getSubFormatStr(pstFileInfo->astProgramInfo[i].astSubTitle[j].eSubtitileType));
            printf("     be ext sub:   %d \n", pstFileInfo->astProgramInfo[i].astSubTitle[j].bExtSubTitle);
            printf("     sub lang:     %s \n", pstFileInfo->astProgramInfo[i].astSubTitle[j].aszSubTitleName);
        }
    }

    printf("\n*************************************************\n\n");

    return HI_SUCCESS;
}

HI_HANDLE hAVPlay = HI_SVR_PLAYER_INVALID_HDL;
HI_HANDLE hWindow = HI_SVR_PLAYER_INVALID_HDL;
DRV_ErrCode DRV_ffmpeg_setup(void)
{
	DRV_ErrCode ErrCode = HI_SUCCESS;
    HI_UNF_AVPLAY_ATTR_S stAVPlayAttr;
    HI_UNF_WINDOW_ATTR_S stWinAttr;

	if((mHiPlayer.HiPlayerStatus == DRV_HIPLAYER_CLOSE) || (mHiPlayer.HiPlayerStatus == DRV_HIPLAYER_STOP))
	{
        printf("DRV_ffmpeg_setup entry\n");
	}
	else
	{
        printf("DRV_ffmpeg_setup mHiPlayer.HiPlayerStatus %d Error\n", mHiPlayer.HiPlayerStatus);
        return -1;
	}

	//DRV_SetScreenBlack();
	ErrCode = DRV_AVAPI_AVstop(0);
	ErrCode |= DRV_AvplayerTerm(mAvPlayHandle,HI_UNF_AVPLAY_MEDIA_CHAN_AUD|HI_UNF_AVPLAY_MEDIA_CHAN_VID);
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_ffmpeg_setup DRV_AvplayerTerm ErrCode 0x%08x \n", ErrCode);
        return ErrCode;
    }
    ErrCode = HI_SVR_PLAYER_Init();
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_ffmpeg_setup HI_SVR_PLAYER_Init ErrCode 0x%08x \n", ErrCode);
        return ErrCode;
    }

	mHiPlayerStatus = HI_SVR_PLAYER_STATE_INI;

    HI_SVR_PLAYER_EnableDbg(HI_FALSE);

    /* 注册文件解析器 */
    ErrCode = HI_SVR_PLAYER_RegisterDynamic(HI_SVR_PLAYER_DLL_PARSER, "libformat.so");
    ErrCode |= HI_SVR_PLAYER_RegisterDynamic(HI_SVR_PLAYER_DLL_PARSER, "libffmpegformat.so");
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_ffmpeg_setup HI_SVR_PLAYER_RegisterDynamic ErrCode 0x%08x \n", ErrCode);
        return ErrCode;
    }

#ifdef USE_AVPLAYER_BUT_CREATE
    /* 使用外部创建的avplay句柄，该avplay必须已经绑定好window, sound设备，且需将window位置、大小设置好，
       sound音量大小设置好，HiPlayer内部对window,sound不做任何操作。
       1、dvb -> HiPlayer: 停止avplay，将avplay句柄付给s_stParam.hAVPlayer，调用HI_SVR_PLAYER_Create创建HiPlayer，
          HiPlayer会将avplay模式切换到想要的模式，并且会重新开关音视频通道；

       2、HiPlayer -> dvb: 调用HI_SVR_PLAYER_Destroy接口销毁HiPlayer，HiPlayer会关闭掉音视频通道，dvb需重新调用
          HI_UNF_AVPLAY_ChnOpen接口开启音视频通道；
    */

    ErrCode = HI_UNF_AVPLAY_GetDefaultConfig(&stAVPlayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
    ErrCode |= HI_UNF_AVPLAY_Create(&stAVPlayAttr, &hAVPlay);

    if (HI_SUCCESS != ErrCode)
    {
        printf("create avplay fail! \n");
        hAVPlay = HI_SVR_PLAYER_INVALID_HDL;
    }
    else
    {
        ErrCode = HI_UNF_AVPLAY_ChnOpen(hAVPlay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
        ErrCode |= HI_UNF_SND_Attach(HI_UNF_SND_0, hAVPlay, HI_UNF_SND_MIX_TYPE_MASTER, 100);

        if (HI_SUCCESS != ErrCode)
        {
            printf("attach sound device fail! \n");
        }

        stWinAttr.enVo = HI_UNF_VO_HD0;
        stWinAttr.bVirtual = HI_FALSE;
        stWinAttr.enAspectRatio = HI_UNF_ASPECT_RATIO_4TO3;
        stWinAttr.enAspectCvrs  = HI_UNF_ASPECT_CVRS_IGNORE;
        stWinAttr.stInputRect.s32X = 0;
        stWinAttr.stInputRect.s32Y = 0;
        stWinAttr.stInputRect.s32Width  = 1920;
        stWinAttr.stInputRect.s32Height = 1080;
        stWinAttr.stOutputRect.s32X = 0;
        stWinAttr.stOutputRect.s32Y = 0;
        stWinAttr.stOutputRect.s32Width  = 1920;
        stWinAttr.stOutputRect.s32Height = 1080;

        ErrCode = HI_UNF_VO_CreateWindow(&stWinAttr, &hWindow);

        if (HI_SUCCESS != ErrCode)
        {
            printf("create window fail! \n");
        }

        ErrCode = HI_UNF_AVPLAY_ChnOpen(hAVPlay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
        if (HI_SUCCESS != ErrCode)
        {
            printf("open vid channel failed.\n");
        }
        
        ErrCode = HI_UNF_VO_AttachWindow(hWindow, hAVPlay);

        if (HI_SUCCESS != ErrCode)
        {
            printf("attach window failed.\n");
        }
        
        ErrCode = HI_UNF_VO_SetWindowEnable(hWindow, HI_TRUE);

        if (HI_SUCCESS != ErrCode)
        {
            printf("enable window failed.\n");
        }
    }
    s_stParam.hAVPlayer = hAVPlay;
#else
    /* 不使用外部创建的avplay句柄，HiPlayer负责创建avplay */

    s_stParam.hAVPlayer = HI_SVR_PLAYER_INVALID_HDL;
#endif


    /* 创建一个播放器实例 */
    //s_stParam.hAVPlayer = HI_SVR_PLAYER_INVALID_HDL;
    s_stParam.u32MixHeight = 50;
    s_stParam.u32SndPort = 0;
    s_stParam.u32DmxId= 4;
    s_stParam.u32PortId= 3;
    mHiPlayer.HiPlayerAvHandle = s_stParam.hAVPlayer;
    ErrCode = HI_SVR_PLAYER_Create(&s_stParam, &mFfmpegPlayHandle);
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_ffmpeg_setup HI_SVR_PLAYER_Create ErrCode 0x%08x \n", ErrCode);
        return ErrCode;
    }

    ErrCode = HI_SVR_PLAYER_RegCallback(mFfmpegPlayHandle, drv_ffmpeg_CallBack);
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_ffmpeg_setup HI_SVR_PLAYER_RegCallback ErrCode 0x%08x \n", ErrCode);
        return ErrCode;
    }
    
    if(mHiPlayer.HiPlayerSemLock != NULL)
		mHiPlayer.HiPlayerSemLock = DRV_OS_SemCreate(NULL,1, DRVOS_INIT_FIFO_WAIT);
	mHiPlayer.HiPlayerStatus = DRV_HIPLAYER_OPEN;
	return ErrCode;
}

DRV_ErrCode DRV_ffmpeg_term(void)
{
	DRV_ErrCode ErrCode = HI_SUCCESS;
	DRVOS_Sem_t   *TempSemLock;
	TempSemLock = mHiPlayer.HiPlayerSemLock;

        if(mHiPlayer.HiPlayerStatus == DRV_HIPLAYER_CLOSE)//wrwei 防止重复调用，状态已经为BUTT直接返回
	{
            printf("[DRV_ffmpeg_term]in !mHiPlayer.HiPlayerStatus == DRV_HIPLAYER_CLOSE\n");
            return HI_SUCCESS;
	}
	else
	{
            printf("DRV_ffmpeg_term mHiPlayer.HiPlayerStatus %d in\n", mHiPlayer.HiPlayerStatus);
	}
	DRV_OS_SemWait_MS(mHiPlayer.HiPlayerSemLock,DRVOS_WAIT_INFINITY,0);

    ErrCode = HI_SVR_PLAYER_Destroy(mFfmpegPlayHandle);
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_ffmpeg_term HI_SVR_PLAYER_Destroy ErrCode 0x%08x \n", ErrCode);
		DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
        return ErrCode;
    }

    ErrCode = HI_SVR_PLAYER_UnRegisterDynamic(HI_SVR_PLAYER_DLL_PARSER, NULL);
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_ffmpeg_term HI_SVR_PLAYER_UnRegisterDynamic ErrCode 0x%08x \n", ErrCode);
		DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
        return ErrCode;
    }

    /* 调用deinit前，必须先调用HI_SVR_PLAYER_Destroy释放掉播放器资源 */
    ErrCode = HI_SVR_PLAYER_Deinit();
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_ffmpeg_term HI_SVR_PLAYER_Deinit ErrCode 0x%08x \n", ErrCode);
		DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
        return ErrCode;
    }
	
    ErrCode = HI_UNF_DMX_AttachTSPort(0,DEFAULT_DVB_PORT);
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_ffmpeg_term HI_UNF_DMX_AttachTSPort ErrCode 0x%08x \n", ErrCode);
    }
#ifdef USE_AVPLAYER_BUT_CREATE
    /* HiPlayer -> dvb，重新开启音视频通道 */
    //ErrCode = HI_UNF_AVPLAY_ChnOpen(mAvPlayHandle, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    //ErrCode |= HI_UNF_AVPLAY_ChnOpen(mAvPlayHandle, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    //if (HI_SUCCESS != ErrCode)
    //{
    //    printf("DRV_ffmpeg_term HI_UNF_AVPLAY_ChnOpen ErrCode 0x%08x \n", ErrCode);
	//	DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
    //    return ErrCode;
    //}
        
    ErrCode = HI_UNF_VO_SetWindowEnable(hWindow, HI_FALSE);
    if (HI_SUCCESS != ErrCode)
    {
        printf("HI_UNF_VO_SetWindowEnable disable window failed.\n");
    }
	
    ErrCode = HI_UNF_VO_DetachWindow(hWindow, hAVPlay);
    if (HI_SUCCESS != ErrCode)
    {
        printf("HI_UNF_VO_DetachWindow detach window failed.\n");
    }

    ErrCode = HI_UNF_AVPLAY_ChnClose(hAVPlay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
    if (HI_SUCCESS != ErrCode)
    {
        printf("HI_UNF_AVPLAY_ChnClose Close vid channel failed.\n");
    }

    ErrCode = HI_UNF_VO_DestroyWindow(hWindow);
    if (HI_SUCCESS != ErrCode)
    {
        printf("HI_UNF_VO_DestroyWindow Destroy window fail! \n");
    }

    ErrCode = HI_UNF_AVPLAY_ChnClose(hAVPlay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
    ErrCode |= HI_UNF_SND_Detach(HI_UNF_SND_0, hAVPlay);
    if (HI_SUCCESS != ErrCode)
    {
        printf("HI_UNF_SND_Detach detach sound device fail! \n");
    }

    ErrCode = HI_UNF_AVPLAY_Destroy(hAVPlay);
    if (HI_SUCCESS != ErrCode)
    {
        printf("HI_UNF_AVPLAY_Destroy Destroy avplay fail! \n");
        hAVPlay = HI_SVR_PLAYER_INVALID_HDL;
    }
	
	ErrCode = DRV_AvplayerSetup(HI_UNF_AVPLAY_STREAM_TYPE_TS,HI_UNF_AVPLAY_MEDIA_CHAN_AUD|HI_UNF_AVPLAY_MEDIA_CHAN_VID,&mAvPlayHandle);
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("DRV_ffmpeg_term DRV_AvplayerSetup failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }
	
#else
	ErrCode = DRV_AvplayerSetup(HI_UNF_AVPLAY_STREAM_TYPE_TS,HI_UNF_AVPLAY_MEDIA_CHAN_AUD|HI_UNF_AVPLAY_MEDIA_CHAN_VID,&mAvPlayHandle);
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("DRV_ffmpeg_term DRV_AvplayerSetup failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }
#endif
	DRV_VideoDisable();
	memset(&mHiPlayer,0,sizeof(DRV_HiPlayer_t));
	mHiPlayer.HiPlayerStatus = DRV_HIPLAYER_CLOSE;
	mHiPlayer.HiPlayerSemLock = TempSemLock;
	mFfmpegPlayHandle = 0;
	mHiPlayerStatus = HI_SVR_PLAYER_STATE_BUTT;
    
        DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
	return ErrCode;
}


DRV_ErrCode DRV_ffmpeg_start(U8* FileName)
{
    HI_FORMAT_FILE_INFO_S stFileInfo;
    HI_SVR_PLAYER_INFO_S stPlayerInfo;
    HI_SVR_PLAYER_STREAMID_S stStreamId;
    HI_SVR_PLAYER_MEDIA_S stMedia;
    HI_U32 Status;
    HI_U32 i;
	DRV_ErrCode ErrCode = HI_SUCCESS;

	if((mHiPlayer.HiPlayerStatus == DRV_HIPLAYER_STOP)||(mHiPlayer.HiPlayerStatus == DRV_HIPLAYER_OPEN))
	{
        printf("DRV_ffmpeg_start entry\n");
	}
	else
	{
        printf("DRV_ffmpeg_start mHiPlayer.HiPlayerStatus %d Error\n", mHiPlayer.HiPlayerStatus);
        return -1;
	}

    mIsHiplayerStopNow = FALSE;
    //memset(&stMedia, 0, HI_FORMAT_MAX_URL_LEN);//wrwei 防止意外死机
    memset(&stMedia, 0, sizeof(stMedia));//wrwei 防止意外死机
    //memset(&stMedia, 0, sizeof(FileName));
    printf("DRV_ffmpeg_start sizeof(stMedia) = %d\n", sizeof(stMedia));
    
    if(UsbFlag == 1)
    {
    DRV_VfsPathUserToDrv(FileName,stMedia.aszUrl);
    }
    else
    {
    #if 0
        U32_YS strlong,i;
    	strlong=strlen((char *)FileName);
    	for(i=1;i<strlong;i++)
    	{
            if (*(FileName+i)!='/')
                *(stMedia.aszUrl + i - 1) = *(FileName + i);
    	    else
    		    *(stMedia.aszUrl + i - 1) = '/';       
    	}
        *(stMedia.aszUrl + i - 1) = '\0';
        sprintf(FileName, "%s", stMedia.aszUrl);
    #endif
        sprintf(stMedia.aszUrl, "%s", FileName);
    }
    
	DRV_OS_SemWait_MS(mHiPlayer.HiPlayerSemLock,DRVOS_WAIT_INFINITY,0);
    //strcpy(mHiPlayer.HiPlayerFileName,stMedia.aszUrl);
    printf("[DRV_ffmpeg_start]FileName =%s, stMedia.aszUrl=%s\n",FileName, stMedia.aszUrl);

    /* 初始化缓冲buffer配置 */
    {
        HI_FORMAT_BUFFER_CONFIG_S stBufConfig;
        stBufConfig.eType = HI_FORMAT_BUFFER_CONFIG_SIZE;
        stBufConfig.s64EventEnough = 4*1024*1024;   /* 4M bytes */
        stBufConfig.s64EventStart  = 512*1024;      /* 512K bytes */
        stBufConfig.s64TimeOut     = 2*60*1000;//10000;         /* 10 sec */
        stBufConfig.s64Total       = 5*1024*1024;   /* 5M bytes */
        ErrCode = HI_SVR_PLAYER_Invoke(mFfmpegPlayHandle, HI_FORMAT_INVOKE_BUFFER_SETCONFIG, &stBufConfig);
        if (HI_SUCCESS != ErrCode)
        {
            printf("ERR: HI_SVR_PLAYER_Invoke setBuffer Config function fail, ret = 0x%x \n", ErrCode);
        }
    }
    
    stMedia.u32UserData = 0;
    // 设置播放模式 HI_SVR_PLAYER_PLAYMODE_NORMAL, HI_SVR_PLAYER_PLAYMODE_PRELOAD
    if(UsbFlag == 1)
    {
        stMedia.s32PlayMode = HI_SVR_PLAYER_PLAYMODE_NORMAL;  //本地播放
    }
    else
    {
        stMedia.s32PlayMode = HI_SVR_PLAYER_PLAYMODE_PRELOAD; //网络播放
    }
    printf("HI_SVR_PLAYER_SetMedia start time = %d \n", DRV_OS_TimeNow_MS());
    //stMedia.aszSubTitle = NULL;
    //stMedia.u32SubTitileNum = 0;
    ErrCode = HI_SVR_PLAYER_SetMedia(mFfmpegPlayHandle, HI_SVR_PLAYER_MEDIA_STREAMFILE, &stMedia);
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_ffmpeg_start HI_SVR_PLAYER_SetMedia ErrCode 0x%08x \n", ErrCode);
		DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
        return ErrCode;
    }
    //printf("HI_SVR_PLAYER_SetMedia end time = %d \n", DRV_OS_TimeNow_MS());

    memset(&stFileInfo, 0, sizeof(stFileInfo));
    ErrCode = HI_SVR_PLAYER_GetFileInfo(mFfmpegPlayHandle, &stFileInfo);
    if (HI_SUCCESS == ErrCode)
    {
        outFileInfo(&stFileInfo);
    }
    else
    {
        printf("ERR: get file info fail! \n");
    }

    //printf("HI_SVR_PLAYER_Play start time = %d \n", DRV_OS_TimeNow_MS());
    ErrCode = HI_SVR_PLAYER_Play(mFfmpegPlayHandle);
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_ffmpeg_start HI_SVR_PLAYER_Play ErrCode 0x%08x \n", ErrCode);
		DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
        return ErrCode;
    }
    printf("HI_SVR_PLAYER_Play end time = %d \n", DRV_OS_TimeNow_MS());
#if 1
{
        int i = 0;
        HI_U32 Status;

    //判断播放器是否处于play状态
        for(i = 0;i < 500;i ++)
    {
        DRV_ffmpegGetStatus(&Status);
        if(Status == HI_SVR_PLAYER_STATE_PLAY)
        {
            break;
        }
            DRV_OS_TaskDelay_MS(10);
    }
        if(i == 500)
    {
        printf("error!the status is %d, time = %d \n", Status, DRV_OS_TimeNow_MS());
            DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
        return HI_FAILURE;
    }
    printf("times = %d,the status is %d, ErrCode = %d \n", i, Status, ErrCode);
}
#endif
	mHiPlayer.HiPlayerStatus = DRV_HIPLAYER_START;
       DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
	return ErrCode;
}

DRV_ErrCode DRV_ffmpeg_stop()
{
	DRV_ErrCode ErrCode = HI_SUCCESS;
	
	if((mHiPlayer.HiPlayerStatus == DRV_HIPLAYER_STOP)||(mHiPlayer.HiPlayerStatus == DRV_HIPLAYER_CLOSE))
	{
            printf("DRV_ffmpeg_stop mHiPlayer.HiPlayerStatus %d \n", mHiPlayer.HiPlayerStatus);
            return HI_SUCCESS;//wrwei 已经停止后再调用直接返回success
	}
	else
	{
            printf("DRV_ffmpeg_stop mHiPlayer.HiPlayerStatus %d in\n", mHiPlayer.HiPlayerStatus);
	}
        
	DRV_OS_SemWait_MS(mHiPlayer.HiPlayerSemLock,DRVOS_WAIT_INFINITY,0);
	if(!mIsHiplayerStopNow)
	{
		ErrCode = HI_SVR_PLAYER_Stop(mFfmpegPlayHandle);
	    if (HI_SUCCESS != ErrCode)
	    {
	        printf("DRV_ffmpeg_stop DRV_ffmpeg_stop ErrCode 0x%08x \n", ErrCode);
			DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
	        return ErrCode;
	    }
    }
#if 1
{
        int i = 0;
        HI_U32 Status;

        //判断播放器是否处于stop状态
        for(i = 0;i < 500;i ++)
        {
            DRV_ffmpegGetStatus(&Status);
            if(Status == HI_SVR_PLAYER_STATE_STOP)
            {
                break;
            }
            DRV_OS_TaskDelay_MS(10);
        }
        if(i == 500)
        {
            printf("error!the status is %d, time = %d \n", Status, DRV_OS_TimeNow_MS());
	DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
            return HI_FAILURE;
        }
}
#endif
	mHiPlayer.HiPlayerStatus = DRV_HIPLAYER_STOP;
       DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
	return ErrCode;
}


DRV_ErrCode DRV_ffmpeg_seek(S64 TimeMs)
{
    HI_S64 s64SeekTime = TimeMs;
	DRV_ErrCode ErrCode = HI_SUCCESS;
    
	if((mHiPlayer.HiPlayerStatus == DRV_HIPLAYER_START)||(mHiPlayer.HiPlayerStatus == DRV_HIPLAYER_FF)||(mHiPlayer.HiPlayerStatus == DRV_HIPLAYER_BW))
	{
        printf("DRV_ffmpeg_seek entry in time = %d \n", DRV_OS_TimeNow_MS());
	}
	else
	{
        printf("DRV_ffmpeg_seek mHiPlayer.HiPlayerStatus %d Error\n", mHiPlayer.HiPlayerStatus);
        return -1;
	}
    
	DRV_OS_SemWait_MS(mHiPlayer.HiPlayerSemLock,DRVOS_WAIT_INFINITY,0);
    ErrCode = HI_SVR_PLAYER_Seek(mFfmpegPlayHandle, s64SeekTime);
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_ffmpeg_seek DRV_ffmpeg_seek ErrCode 0x%08x \n", ErrCode);
		DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
        return ErrCode;
    }
	if(mIsHiplayerStopNow)
	{
	    ErrCode = HI_SVR_PLAYER_Play(mFfmpegPlayHandle);
	    if (HI_SUCCESS != ErrCode)
	    {
	        printf("DRV_ffmpeg_seek HI_SVR_PLAYER_Play ErrCode 0x%08x \n", ErrCode);
	    }
	    mIsHiplayerStopNow = FALSE;
#if 1
{
        int i = 0;
        HI_U32 Status;

        //判断播放器是否处于play状态
        for(i = 0;i < 500;i ++)
        {
            DRV_ffmpegGetStatus(&Status);
            if(Status == HI_SVR_PLAYER_STATE_PLAY)
            {
                break;
            }
            DRV_OS_TaskDelay_MS(10);
    }
        if(i == 500)
        {
            printf("error!the status is %d, time = %d \n", Status, DRV_OS_TimeNow_MS());
	DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
            return HI_FAILURE;
        }
}
#endif
    }

	mHiPlayer.HiPlayerStatus = DRV_HIPLAYER_START;	
	DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
	return ErrCode;
}


DRV_ErrCode DRV_ffmpeg_BackForward(U32 Speed)
{
	HI_S32 s32Speed = 0;
	DRV_ErrCode ErrCode = HI_SUCCESS;
    
	if((mHiPlayer.HiPlayerStatus == DRV_HIPLAYER_START)||(mHiPlayer.HiPlayerStatus == DRV_HIPLAYER_FF)||(mHiPlayer.HiPlayerStatus == DRV_HIPLAYER_BW)||(mHiPlayer.HiPlayerStatus == DRV_HIPLAYER_SEEK))
	{
        printf("DRV_ffmpeg_BackForward entry\n");
	}
	else
	{
        printf("DRV_ffmpeg_BackForward mHiPlayer.HiPlayerStatus %d Error\n", mHiPlayer.HiPlayerStatus);
        return -1;
	}
	s32Speed = Speed;
	//s32Speed *= HI_SVR_PLAYER_PLAY_SPEED_NORMAL;

	if (s32Speed > HI_SVR_PLAYER_PLAY_SPEED_32X_FAST_FORWARD)
	{
	    printf("ERR: not support tplay speed! \n");
	    return -1;
	}
	DRV_OS_SemWait_MS(mHiPlayer.HiPlayerSemLock,DRVOS_WAIT_INFINITY,0);
    s32Speed = 0 - s32Speed;
    if(s32Speed != HI_SVR_PLAYER_PLAY_SPEED_NORMAL)
    {
	    printf("backward speed = %d \n", s32Speed);
	    ErrCode = HI_SVR_PLAYER_TPlay(mFfmpegPlayHandle, s32Speed);
	    if (HI_SUCCESS != ErrCode)
	    {
	        printf("DRV_ffmpeg_BackForward HI_SVR_PLAYER_TPlay ErrCode 0x%08x \n", ErrCode);
			DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
	        return ErrCode;
	    }
		mHiPlayer.HiPlayerStatus = DRV_HIPLAYER_BW;	
	}
	else
	{
		if(mIsHiplayerStopNow)
		{
		    ErrCode = HI_SVR_PLAYER_Play(mFfmpegPlayHandle);
		    if (HI_SUCCESS != ErrCode)
		    {
		        printf("DRV_ffmpeg_BackForward HI_SVR_PLAYER_Play ErrCode 0x%08x \n", ErrCode);
		    }
		    mIsHiplayerStopNow = FALSE;
	    }
	    else
	    {
		    ErrCode = HI_SVR_PLAYER_Resume(mFfmpegPlayHandle);
		    if (HI_SUCCESS != ErrCode)
		    {
		        printf("DRV_ffmpeg_BackForward DRV_ffmpeg_Resume ErrCode 0x%08x \n", ErrCode);
				//DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
		        //return ErrCode;
		    }
	    }
		mHiPlayer.HiPlayerStatus = DRV_HIPLAYER_START;		
	}
	DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
	return ErrCode;
}

DRV_ErrCode DRV_ffmpeg_SetSpeed(U32 Speed)
{
	HI_S32 s32Speed = 0;
	DRV_ErrCode ErrCode = HI_SUCCESS;
    
	if((mHiPlayer.HiPlayerStatus == DRV_HIPLAYER_START)||(mHiPlayer.HiPlayerStatus == DRV_HIPLAYER_FF)||(mHiPlayer.HiPlayerStatus == DRV_HIPLAYER_BW)||(mHiPlayer.HiPlayerStatus == DRV_HIPLAYER_SEEK))
	{
        printf("DRV_ffmpeg_SetSpeed entry\n");
	}
	else
	{
        printf("DRV_ffmpeg_SetSpeed mHiPlayer.HiPlayerStatus %d Error\n", mHiPlayer.HiPlayerStatus);
        return -1;
	}
	s32Speed = Speed;
    //外面已经处理了s32Speed *= HI_SVR_PLAYER_PLAY_SPEED_NORMAL;

    if (s32Speed > HI_SVR_PLAYER_PLAY_SPEED_32X_FAST_FORWARD)
    {
        printf("ERR: not support tplay speed! \n");
	    return -1;
    }
	DRV_OS_SemWait_MS(mHiPlayer.HiPlayerSemLock,DRVOS_WAIT_INFINITY,0);
    if(s32Speed != HI_SVR_PLAYER_PLAY_SPEED_NORMAL)
    {
	    printf("forward speed = %d \n", s32Speed);
	    ErrCode = HI_SVR_PLAYER_TPlay(mFfmpegPlayHandle, s32Speed);
	    if (HI_SUCCESS != ErrCode)
	    {
	        printf("DRV_ffmpeg_SetSpeed HI_SVR_PLAYER_TPlay ErrCode 0x%08x \n", ErrCode);
			DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
	        return ErrCode;
	    }
		mHiPlayer.HiPlayerStatus = DRV_HIPLAYER_FF;	
	}
	else
	{	
		if(mIsHiplayerStopNow)
		{
		    ErrCode = HI_SVR_PLAYER_Play(mFfmpegPlayHandle);
		    if (HI_SUCCESS != ErrCode)
		    {
		        printf("DRV_ffmpeg_SetSpeed HI_SVR_PLAYER_Play ErrCode 0x%08x \n", ErrCode);
		    }
		    mIsHiplayerStopNow = FALSE;
	    }
	    else
	    {
		    ErrCode = HI_SVR_PLAYER_Resume(mFfmpegPlayHandle);
		    if (HI_SUCCESS != ErrCode)
		    {
		        printf("DRV_ffmpeg_SetSpeed DRV_ffmpeg_Resume ErrCode 0x%08x \n", ErrCode);
				//DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
		        //return ErrCode;
		    }
	    }
		mHiPlayer.HiPlayerStatus = DRV_HIPLAYER_START;		
	}
	DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
	return ErrCode;
}

DRV_ErrCode DRV_ffmpeg_Pause(void)
{
	DRV_ErrCode ErrCode = HI_SUCCESS;
    
	if((mHiPlayer.HiPlayerStatus == DRV_HIPLAYER_START)||(mHiPlayer.HiPlayerStatus == DRV_HIPLAYER_FF)||(mHiPlayer.HiPlayerStatus == DRV_HIPLAYER_BW))
	{
        printf("DRV_ffmpeg_Pause entry\n");
	}
	else
	{
        printf("DRV_ffmpeg_Pause mHiPlayer.HiPlayerStatus %d Error\n", mHiPlayer.HiPlayerStatus);
        return -1;
	}
	DRV_OS_SemWait_MS(mHiPlayer.HiPlayerSemLock,DRVOS_WAIT_INFINITY,0);
    ErrCode = HI_SVR_PLAYER_Pause(mFfmpegPlayHandle);
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_ffmpeg_Pause HI_SVR_PLAYER_Pause ErrCode 0x%08x \n", ErrCode);
		DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
        return ErrCode;
    }
#if 1
{
        int i = 0;
        HI_U32 Status;

        //判断播放器是否处于play状态
        for(i = 0;i < 500;i ++)
        {
            DRV_ffmpegGetStatus(&Status);
            if(Status == HI_SVR_PLAYER_STATE_PAUSE)
            {
                break;
            }
            DRV_OS_TaskDelay_MS(10);
        }
        if(i == 500)
        {
            printf("error!the status is %d, time = %d \n", Status, DRV_OS_TimeNow_MS());
	DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
            return HI_FAILURE;
        }
}
#endif
	mHiPlayer.HiPlayerStatus = DRV_HIPLAYER_PAUSE;	
       DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
	return ErrCode;
}


DRV_ErrCode DRV_ffmpeg_Resume(void)
{
	DRV_ErrCode ErrCode = HI_SUCCESS;
	if((mHiPlayer.HiPlayerStatus == DRV_HIPLAYER_PAUSE)||(mHiPlayer.HiPlayerStatus == DRV_HIPLAYER_FF)||(mHiPlayer.HiPlayerStatus == DRV_HIPLAYER_BW))
	{
        printf("DRV_ffmpeg_Resume entry\n");
	}
	else
	{
        printf("DRV_ffmpeg_Resume mHiPlayer.HiPlayerStatus %d Error\n", mHiPlayer.HiPlayerStatus);
        return -1;
	}
	DRV_OS_SemWait_MS(mHiPlayer.HiPlayerSemLock,DRVOS_WAIT_INFINITY,0);
    ErrCode = HI_SVR_PLAYER_Resume(mFfmpegPlayHandle);
    if (HI_SUCCESS != ErrCode)
    {
        printf("HI_SVR_PLAYER_Resume DRV_ffmpeg_Resume ErrCode 0x%08x \n", ErrCode);
		DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
        return ErrCode;
    }
#if 1
{
        int i = 0;
        HI_U32 Status;

        //判断播放器是否处于play状态
        for(i = 0;i < 500;i ++)
        {
            DRV_ffmpegGetStatus(&Status);
            if(Status == HI_SVR_PLAYER_STATE_PLAY)
            {
                break;
            }
            DRV_OS_TaskDelay_MS(10);
        }
        if(i == 500)
        {
            printf("error!the status is %d, time = %d \n", Status, DRV_OS_TimeNow_MS());
	DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
            return HI_FAILURE;
        }
}
#endif
	mHiPlayer.HiPlayerStatus = DRV_HIPLAYER_START;	
       DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
	return ErrCode;
}


DRV_ErrCode DRV_ffmpeg_SetSync(DRV_AvSynMode SynMode)
{
    HI_SVR_PLAYER_SYNC_ATTR_S stSyncAttr;
	DRV_ErrCode ErrCode = HI_SUCCESS;
    
	DRV_OS_SemWait_MS(mHiPlayer.HiPlayerSemLock,DRVOS_WAIT_INFINITY,0);
	if(SynMode == DRV_AVSYN_VID_PTS)
	{
		stSyncAttr.s32VFrameOffset = 0;
	}
	else if(SynMode == DRV_AVSYN_AUD_PTS)
	{
		stSyncAttr.s32AFrameOffset = 0;
	}
	else
	{
		stSyncAttr.s32AFrameOffset = 0;
	}
	ErrCode = HI_SVR_PLAYER_SetParam(mFfmpegPlayHandle, HI_SVR_PLAYER_ATTR_SYNC, (HI_VOID*)&stSyncAttr);
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_ffmpeg_SetSync HI_UNF_AVPLAY_SetAttr ErrCode 0x%08x \n", ErrCode);
		DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
        return ErrCode;
    }
	DRV_OSSemSignal(mHiPlayer.HiPlayerSemLock);
	return ErrCode;
}

DRV_ErrCode DRV_ffmpeg_Get_DurationTime(U32 * Time)
{
	DRV_ErrCode ErrCode = HI_SUCCESS;
    HI_SVR_PLAYER_INFO_S struInfo;

    ErrCode = HI_SVR_PLAYER_GetPlayerInfo(mFfmpegPlayHandle, &struInfo);
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_ffmpeg_Get_DurationTime HI_SVR_PLAYER_GetPlayerInfo ErrCode 0x%08x \n", ErrCode);
        return ErrCode;
    }
    
	*Time = mHiPlayer.HiPlayerDuration;//struInfo.u64TimePlayed;
    
	return ErrCode;
}

DRV_ErrCode DRV_ffmpeg_Get_NowTime(HI_U64 *Time)
{
	DRV_ErrCode ErrCode = HI_SUCCESS;
    HI_SVR_PLAYER_INFO_S struInfo;

    ErrCode = HI_SVR_PLAYER_GetPlayerInfo(mFfmpegPlayHandle, &struInfo);
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_ffmpeg_Get_NowTime HI_SVR_PLAYER_GetPlayerInfo ErrCode 0x%08x \n", ErrCode);
        return ErrCode;
    }
    
    *Time = struInfo.u64TimePlayed;
    return ErrCode;
}

DRV_ErrCode DRV_ffmpeg_GetSpeed(U32 *Speed)
{
	DRV_ErrCode ErrCode = HI_SUCCESS;
	HI_SVR_PLAYER_INFO_S struInfo;
    ErrCode = HI_SVR_PLAYER_GetPlayerInfo(mFfmpegPlayHandle, &struInfo);
    mHiPlayer.HiPlayerSpeed = struInfo.s32Speed;
	*Speed = mHiPlayer.HiPlayerSpeed;
	return ErrCode;
}
U32 DRV_ffmpeg_GetSpeedtime(U32 * ops,U32 *time)
{
		U32 speed;
		//speed = mHiPlayer.speed;
		switch (speed)
		{
			case 200:
				*time=2000;
				*ops=10;
				break;
			case 400:
				*time=1500;
				*ops=10;
				break;
			case 800:
				*time=1000;
				*ops=10;
				break;
			case 1600:
				*time=500;
				*ops=15;
				break;
			case 3200:
				*time=500;
				*ops=20;
				break;
			case -100:
				*time=2000;
				*ops=-10;
			case -200:
				*time=2000;
				*ops=-10;
				break;
			case -400:
				time=1500;
				ops=-10;
				break;
			case -800:
				*time=1000;
				*ops=-10;
				break;
			case -1600:
				*time=500;
				*ops=-15;
				break;
			case -3200:
				*time=500;
				*ops=-20;
				break;
			default:
				break;
		}
}


DRV_ErrCode DRV_ffmpegEnableCallback(DRV_PlayRecCallback Callback_f)
{
    mHiPlayer.HiPlayerCallback = Callback_f;
    return DRV_NO_ERROR;
}
DRV_ErrCode DRV_ffmpegDisableCallback(void)
{
    mHiPlayer.HiPlayerCallback = NULL;
    return DRV_NO_ERROR;
}
DRV_ErrCode DRV_ffmpegSetAudio( HI_U32 vol)
{
    HI_HANDLE hdl= 0;   
	DRV_ErrCode ErrCode = HI_SUCCESS;
    ErrCode = HI_SVR_PLAYER_GetParam(mFfmpegPlayHandle, HI_SVR_PLAYER_ATTR_AVPLAYER_HDL, &hdl);
    if (HI_SUCCESS != ErrCode)
    {
        printf("ERR: get avplay hdl fail! \n");
        return HI_FAILURE;
    }

    ErrCode = HI_UNF_SND_SetMixWeight(HI_UNF_SND_0, hdl, vol);
    if (HI_SUCCESS != ErrCode)
    {
        printf("ERR: get avplay hdl fail! \n");
        return HI_FAILURE;
    }
    return DRV_NO_ERROR;
}

DRV_ErrCode DRV_ffmpegGetStatus( HI_U32 *Status)
{
	DRV_ErrCode ErrCode = HI_SUCCESS;
    HI_SVR_PLAYER_INFO_S struInfo;

    ErrCode = HI_SVR_PLAYER_GetPlayerInfo(mFfmpegPlayHandle, &struInfo);
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_ffmpegGetStatus HI_SVR_PLAYER_GetPlayerInfo ErrCode 0x%08x \n", ErrCode);
        return ErrCode;
    }
	*Status = struInfo.eStatus;
    
	return ErrCode;
}

