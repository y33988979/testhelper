/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : 	ysav.h
    author   : 		yzb creat 2004-3-17
    Description : 
    Others:       
 
*******************************************************************************/

#ifndef _YSAV_H_
#define _YSAV_H_

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include <stddefs.h>
#include <hi_unf_common.h>
#include <hi_unf_demux.h>
#include "drv_typedef.h"
#include "drv_avplay.h"
#include "drv_snd.h"
#include "ysfilter.h"
#include "drv_disp.h"
#include "drv_vo.h"
/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Private Constant & Enum & Macro
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Global variable
 *----------------------------------------------------------------------------*/
#define PTI_CURRENT_USE 0
#define CURRENT_DECODE_CHANNEL (0)
#define VIDEO_DEFAULT_PTI_INSTANCE 0
#define VIDEO_INJECT_PTI_INSTANCE 2

#define PVR_DMX_ID_LIVE             HI_UNF_DMX_ID_0_PLAY
#define PVR_DMX_PORT_ID_IP          HI_UNF_DMX_PORT_3_RAM
#define PVR_DMX_PORT_ID_DVB         HI_UNF_DMX_PORT_0_TUNER
#define PVR_DMX_PORT_ID_PLAYBACK    HI_UNF_DMX_PORT_4_RAM
#define PVR_DMX_ID_REC              HI_UNF_DMX_ID_1_REC
/*----------------------------------------------------------------------------
 *	Global Function Prototype
 *----------------------------------------------------------------------------*/
typedef enum DRV_VideoType_e
{
    DRV_VIDEO_TYPE_MP1V,
    DRV_VIDEO_TYPE_MP2V,
    DRV_VIDEO_TYPE_H264,
    DRV_VIDEO_TYPE_MPEG4P2,    
    DRV_VIDEO_TYPE_VC1,
    DRV_VIDEO_TYPE_MP1V_PIP,
    DRV_VIDEO_TYPE_MP2V_PIP,
    DRV_VIDEO_TYPE_H264_PIP,
    DRV_VIDEO_TYPE_MPEG4P2_PIP,    
    DRV_VIDEO_TYPE_VC1_PIP,
    DRV_VIDEO_TYPE_AVS,
    DRV_VIDEO_TYPE_MAX
} DRV_VideoType;

typedef enum DRV_AudioType_e
{
    DRV_AUDIO_TYPE_AC3,
    DRV_AUDIO_TYPE_DTS,
    DRV_AUDIO_TYPE_MPEG1,/* stream type 3 */
    DRV_AUDIO_TYPE_MPEG2,
    DRV_AUDIO_TYPE_CDDA,
    DRV_AUDIO_TYPE_PCM,
    DRV_AUDIO_TYPE_LPCM,
    DRV_AUDIO_TYPE_PINK_NOISE,
    DRV_AUDIO_TYPE_MP3,
    DRV_AUDIO_TYPE_MLP,
    DRV_AUDIO_TYPE_BEEP_TONE,
    DRV_AUDIO_TYPE_MPEG_AAC, /* stream type 15 */
    DRV_AUDIO_TYPE_MPEG_HE_AAC,/* stream type 17*/
    DRV_AUDIO_TYPE_WMA,
    DRV_AUDIO_TYPE_MAX,
    DRV_AUDIO_TYPE_DOLBY_PLUS /*杜比*/
} DRV_AudioType;

typedef enum STAUD_StreamType_e
{
    STAUD_STREAM_TYPE_ES            = (1 << 0),
    STAUD_STREAM_TYPE_PES           = (1 << 1),
    STAUD_STREAM_TYPE_PES_AD        = (1 << 2),
    STAUD_STREAM_TYPE_MPEG1_PACKET  = (1 << 3),
    STAUD_STREAM_TYPE_PES_DVD       = (1 << 4),
    STAUD_STREAM_TYPE_PES_DVD_AUDIO = (1 << 5),
    STAUD_STREAM_TYPE_SPDIF         = (1 << 6),
    STAUD_STREAM_TYPE_PES_ST        = (1 << 7)
}STAUD_StreamType_t;

#if 1//st
typedef enum STAUD_StreamContent_e
{
    STAUD_STREAM_CONTENT_AC3        = (1 << 0),
    STAUD_STREAM_CONTENT_DTS        = (1 << 1),
    STAUD_STREAM_CONTENT_MPEG1      = (1 << 2),
    STAUD_STREAM_CONTENT_MPEG2      = (1 << 3),
    STAUD_STREAM_CONTENT_CDDA       = (1 << 4),
    STAUD_STREAM_CONTENT_PCM        = (1 << 5),
    STAUD_STREAM_CONTENT_LPCM       = (1 << 6),
    STAUD_STREAM_CONTENT_PINK_NOISE = (1 << 7),
    STAUD_STREAM_CONTENT_MP3        = (1 << 8),
    STAUD_STREAM_CONTENT_MLP        = (1 << 9),
    STAUD_STREAM_CONTENT_BEEP_TONE  = (1 << 10),
    STAUD_STREAM_CONTENT_MPEG_AAC   = (1 << 11),
    STAUD_STREAM_CONTENT_WMA        = (1 << 12),
    STAUD_STREAM_CONTENT_DV         = (1 << 13),
    STAUD_STREAM_CONTENT_CDDA_DTS   = (1 << 14),
    STAUD_STREAM_CONTENT_LPCM_DVDA  = (1 << 15),
    STAUD_STREAM_CONTENT_HE_AAC       = (1 << 16),
    STAUD_STREAM_CONTENT_DDPLUS       = (1 << 17),
    STAUD_STREAM_CONTENT_WMAPROLSL    = (1 << 18),
    STAUD_STREAM_CONTENT_ALSA         = (1 << 19),
    STAUD_STREAM_CONTENT_MP4_FILE = (1 << 20),
    STAUD_STREAM_CONTENT_ADIF     = (1 << 21),
    STAUD_STREAM_CONTENT_RAW_AAC  = (1 << 22),
    STAUD_STREAM_CONTENT_WAVE     =(1<<23),
    STAUD_STREAM_CONTENT_AIFF     =(1<<24),
    STAUD_STREAM_CONTENT_DRA      =(1<<25),
    STAUD_STREAM_CONTENT_RA      =(1<<26),
    STAUD_STREAM_CONTENT_VRBS      =(1<<27),
    STAUD_STREAM_CONTENT_FLAC      =(1<<28),
    STAUD_STREAM_CONTENT_DDPULSE   = (1<<29),
    STAUD_STREAM_CONTENT_NULL     = (1 << 30)
     // DO NOT EDIT BELOW THIS LINE
} STAUD_StreamContent_t;

typedef struct STAUD_StartParams_s
{
    STAUD_StreamContent_t StreamContent;
    STAUD_StreamType_t StreamType;
    U32 SamplingFrequency;
    U8 StreamID;

} STAUD_StartParams_t;

typedef struct STAUD_PCMInputParams_s
{
    U32 Frequency;
    U32 DataPrecision;
    U8  NumChannels;
} STAUD_PCMInputParams_t;

typedef enum STVID_StreamType_e
{
    STVID_STREAM_TYPE_ES           = 1,
    STVID_STREAM_TYPE_PES          = 2,
    STVID_STREAM_TYPE_MPEG1_PACKET = 4, /* This type is obsolete, MPEG1 packet extraction is not handled at STAPI driver level and should be handled by application above STAPI */
    STVID_STREAM_TYPE_UNCOMPRESSED = 8
} STVID_StreamType_t;

typedef enum STVID_BroadcastProfile_e
{
    STVID_BROADCAST_DVB     = 1,
    STVID_BROADCAST_DIRECTV = 2,
    STVID_BROADCAST_ATSC    = 4,
    STVID_BROADCAST_DVD     = 8,
    STVID_BROADCAST_ARIB    = 16
} STVID_BroadcastProfile_t;

typedef struct STVID_StartParams_s
{
    BOOL                        RealTime;
    BOOL                        UpdateDisplay;
    STVID_BroadcastProfile_t    BrdCstProfile;
    STVID_StreamType_t          StreamType;
    U8                          StreamID;
    BOOL                        DecodeOnce;
} STVID_StartParams_t;

#endif

typedef enum DRV_DigitalMode_e
{
    DRV_AUDIO_DIGITAL_MODE_PCM,
    DRV_AUDIO_DIGITAL_MODE_AC3,
    DRV_AUDIO_DIGITAL_MODE_MAX
} DRV_DigitalMode;


typedef enum DRV_DecodedPictures_e
{
    DRV_DECODED_PICTURES_ALL,
    DRV_DECODED_PICTURES_IP,
    DRV_DECODED_PICTURES_I,
    DRV_DECODED_PICTURES_FIRST_I
} DRV_DecodedPictures_t;

typedef struct DRV_InjectData_s
{
	U32 * Buffer;
	U32 Lenth;
	U32 BufferStatus;
}DRV_InjectData;

 typedef struct DRV_AudioInjectSource_s
{
	U8 *FileName;
	U8 *AudioData_p;
	U32 DataLength;
	BOOL IsCircleBuffer;
}DRV_AudioInjectSource;

 typedef struct DRV_AudioESInjectSource_s
{
	U8 *FileName;
	U8 *AudioData_p;
	U32 DataLength;
    S64 PtsMs;
}DRV_AudioESInjectSource;
 
 typedef struct DRV_VideoESInjectSource_s
 {
     U8 *FileName;
     U8 *VideoData_p;
     U32 DataLength;
     S64 PtsMs;
 }DRV_VideoESInjectSource;

typedef enum DRV_InjectSrc_e
{
	DRV_INJECT_FROM_FILE,
	DRV_INJECT_FROM_BUFFER
}DRV_InjectSrc;

typedef struct DRV_InjectBuffer_s
{
    U32          BufSize;
    U8*          BufTopPtr_p;
    U8*          BufBasePtr_p;
    U8*          BufWritePtr_p;
    U8*          BufReadPtr_p;  
    U32          BufThreshold;
}DRV_InjectBuffer;


typedef enum DRV_PlayRecEvt_s
{
	DRV_PLAYREC_REACH_END_OF_PLAYBACK_EVT,/*到达播放最末位置*/
	DRV_PLAYREC_REACH_BEGIN_OF_PLAYBACK_EVT,/*到达播放最起始位置*/
	DRV_PLAYREC_PLAY_DECODING_ERROR_EVT,/*播放出现错误，比如overflow,underflow,宏块错误等,应用可以忽略*/
	DRV_PLAYREC_PLAY_FILE_NOT_FOUND_EVT,/*文件没有找到*/
	DRV_PLAYREC_PLAY_READ_ERROR_EVT,/*播放中读取文件出错*/
	DRV_PLAYREC_PLAY_SEEK_ERROR_EVT,/*播放中跳转文件出错*/
	DRV_PLAYREC_RECORD_WRITE_ERROR_EVT,/*录制中写文件出错*/
	DRV_PLAYREC_REACH_END_OF_FILE_EVT,/*录制过程中到达文件结束*/
	DRV_PLAYREC_PLAY_REACH_RECORD_EVT,/*timeshift追上live*/
	DRV_PLAYREC_DISK_LOW_LEVEL_EVT/*录制过程中，磁盘空间不足*/
}DRV_PlayerRecEvt;

typedef void (*DRV_PlayRecCallback)(DRV_PlayerRecEvt event,void *Payload);

typedef enum DRV_AvSynMode_e
{
	DRV_AVSYN_NONE,/*不做任何同步,也不收PCR*/
	DRV_AVSYN_PCR,/*PCR同步，默认音视频都enable，但AUD,VID无回调*/
	DRV_AVSYN_AUD_PTS,/*PCR enable，Vid同步enable,aud同步disable,aud有回调*/
	DRV_AVSYN_VID_PTS,/*PCR enable，aud同步enable,Vid同步disable,aud有回调*/
	DRV_AVSYN_NONE_WITH_PCR/*PCR enable，aud同步disable,Vid同步disable,aud有回调*/
}DRV_AvSynMode;

/*extern av 相关全局变量，一边其他模块使用*/
extern HI_HANDLE  mFfmpegPlayHandle;


extern DRV_Pid m_curVidPid;/*video当前的PID*/
extern DRV_Pid m_curAudPid;/*audio当前的PID*/
extern DRV_Pid m_curPcrPid;/*pcr当前的PID*/
extern DRV_Pid m_curTtxPid;/*txt当前的PID*/
extern DRV_AvSynMode m_DecodeCurAvSynMode[];/*当前的同步模式*/
extern DRV_AvSynMode m_DecodeLastAvSynMode[];/*解码器初始化时的同步模式*/
extern BOOL_YS m_bEnableVideo;/*video当前的状态，TRUE为start，FALSE为stop*/
extern BOOL_YS m_bEnableAudio;/*audio当前的状态，TRUE为start，FALSE为stop*/
extern BOOL_YS m_bEnablePCR;/*pcr当前的状态，TRUE为start，FALSE为stop*/
extern DRV_VideoType m_curVidStreamContent;
extern DRV_VideoType m_LastVidStreamContent;
extern BOOL m_IsEnableDolbyOutput;
extern BOOL m_DisableOutPutWhenVideoStop;
extern BOOL m_IsNPSwitchAuto;
extern BOOL m_OutputAlreadyEnabled;


/**********************************************************
Function Name  :DRV_AvSynModeSet，DRV_AvSynModeSetWithParam
Description       :设置同步属性
param[in]         :SynMode，同步模式
param[in]         :VidStreamContent，无效参数
***********************************************************/
DRV_ErrCode DRV_AvSynModeSet(DRV_AvSynMode SynMode);
DRV_ErrCode DRV_AvSynModeSetWithParam(DRV_AvSynMode SynMode,DRV_VideoType VidStreamContent);


/**********************************************************
Function Name  :drv_AvSynModeChangeCheck
Description       :检查同步模式是否转换了
param[in]         :LastSynMode，同步模式
param[in]         :CurSynMode，同步模式
***********************************************************/
BOOL drv_AvSynModeChangeCheck(DRV_AvSynMode LastSynMode,DRV_AvSynMode CurSynMode);


/**********************************************************
Function Name  :DRV_ClkRVEnable
Description       :PCR同步
***********************************************************/
void DRV_ClkRVEnable(void);


/**********************************************************
Function Name  :DRV_ClkRVDisable
Description       :不作任何同步
***********************************************************/
void DRV_ClkRVDisable(void);


/**********************************************************
Function Name  :DRV_DENC_DisableVideo
Description       :关闭视频及图形层的显示输出
                        一般只在待机时用
***********************************************************/
DRV_ErrCode  DRV_DENC_DisableVideo(void);


/**********************************************************
Function Name  :DRV_DENC_EnableVideo
Description       :打开视频及图形层的显示输出
***********************************************************/
DRV_ErrCode  DRV_DENC_EnableVideo(void);


/**********************************************************
Function Name  :DRV_VideoEnable
Description       :打开视频层显示输出
***********************************************************/
DRV_ErrCode DRV_VideoEnable(void);


/**********************************************************
Function Name  :DRV_VideoDisable
Description       :关闭视频层，黑屏
***********************************************************/
DRV_ErrCode DRV_VideoDisable(void);


/**********************************************************
Function Name  :DRV_VideoGetVisiable
Description       :获取视频是否输出TRUE/FALSE
***********************************************************/
BOOL DRV_VideoGetVisiable(void);


/**********************************************************
Function Name  :DRV_AV_MuteAudio
Description       :设置静音
***********************************************************/
DRV_ErrCode DRV_AV_MuteAudio(void);


/**********************************************************
Function Name  :DRV_AV_UnMuteAudio
Description       :取消静音
***********************************************************/
DRV_ErrCode DRV_AV_UnMuteAudio(void);


/**********************************************************
Function Name  :DRV_AV_SetAudio
Description       :设置音量0为最大，63为最小
***********************************************************/
DRV_ErrCode DRV_AV_SetAudio ( U8_YS left, U8_YS right);


/**********************************************************
Function Name  :DRV_SetScreenNoBlack
Description       :设置切台不黑屏
***********************************************************/
void DRV_SetScreenNoBlack(void);


/**********************************************************
Function Name  :DRV_SetScreenBlack
Description       :设置切台黑屏
***********************************************************/
void DRV_SetScreenBlack(void);


/**********************************************************
Function Name  :DRV_SetNPSwitchNotAuto
Description       :设置NP制不是根据流的属性自动切换
***********************************************************/
void DRV_SetNPSwitchNotAuto(void);


/**********************************************************
Function Name  :DRV_SetNPSwitchAuto
Description       :设置NP制是根据流的属性自动切换
***********************************************************/
void DRV_SetNPSwitchAuto(void);


/**********************************************************
Function Name  :DRV_AV_GetVideoWin
Description       :获取输入输出窗口位置和大小
param[in]         :pstRectIn，输入窗口位置和大小
param[in]         :pstRectOut，输出窗口位置和大小
***********************************************************/
DRV_ErrCode DRV_AV_GetVideoWin(DRV_RectWh *pstRectIn,DRV_RectWh *pstRectOut);


/**********************************************************
Function Name  :DRV_AV_SetVideoWin
Description       :设置输入输出窗口位置和大小
param[in]         :pstRectIn，输入窗口位置和大小
param[in]         :pstRectOut，输出窗口位置和大小
param[in]         :AutoMode，不为0时，输出为1920*1080
***********************************************************/
DRV_ErrCode DRV_AV_SetVideoWin(DRV_RectWh *pstRectIn, DRV_RectWh *pstRectOut, BOOL AutoMode);


/**********************************************************
Function Name  :drv_VideoSetMode
Description       :设置视频显示制式
param[in]         :mode，视频制式
***********************************************************/
DRV_ErrCode drv_VideoSetMode(DRV_VtgTimingMode_t mode);


/**********************************************************
Function Name  :DRV_EnableDolbyOutput
Description       :设置杜比5.1输出
param[in]         :AC3格式
***********************************************************/
DRV_ErrCode DRV_EnableDolbyOutput(HI_BOOL bbhdmipassthrough,HI_BOOL bspdifpassthrough);


/**********************************************************
Function Name  :DRV_DisableDolbyOutput
Description       :设置杜比5.1输出
param[in]         :PCM格式
***********************************************************/
DRV_ErrCode DRV_DisableDolbyOutput(void);

/**********************************************************
Function Name  :DRV_GetAudioType
Description       :获取当前音频格式
param[in]         :音频格式
***********************************************************/
DRV_ErrCode DRV_GetAudioType(DRV_AudioType *AudStreamContent);

/**********************************************************
Function Name  :DRV_SetVoiceMode
Description       :设置音频声道模式
param[in]         :mode，0(立体道) 1(左声道) 2(右声道) 3(混合输出)
***********************************************************/
DRV_ErrCode  DRV_SetVoiceMode(char  mode);


/**********************************************************
Function Name  :DRV_SetScreenRatio
Description       :设置屏幕宽高比
param[in]         :mode， 0(4:3) 1( 16:9)  2(auto)
***********************************************************/
DRV_ErrCode  DRV_SetScreenRatio(char  mode);


/**********************************************************
Function Name  :DRV_SetScreenRatioConversion
Description       :设置窗口的宽高转换方式
param[in]         :mode，转换方式
***********************************************************/
DRV_ErrCode  DRV_SetScreenRatioConversion(DRV_ScreenRatioConversion  mode);


/*audio 操作*/


/**********************************************************
Function Name  :DRV_AV_StopAudio
Description       :关闭音频播放器
param[in]         :PTIInst，无效参数
***********************************************************/
DRV_ErrCode  DRV_AV_StopAudio(U32 PTIInst);


/**********************************************************
Function Name  :DRV_AV_StartAudio，DRV_AV_StartAudioWithParam
Description       :开启视频播放器
param[in]         :PTIInst，无效参数
param[in]         :StreamType，无效参数
param[in]         :AudPID，无效参数
param[in]         :AudStartParams，无效参数
param[in]         :AudStreamContent，音频编码格式
***********************************************************/
DRV_ErrCode DRV_AV_StartAudio(U32 PTIInst,STAUD_StreamType_t StreamType, STPTI_Pid_t AudPID,DRV_AudioType AudStreamContent);
DRV_ErrCode DRV_AV_StartAudioWithParam(U32 PTIInst,STAUD_StartParams_t AudStartParams,DRV_AudioType AudStreamContent);


/**********************************************************
Function Name  :DRV_AV_PauseAudio
Description       :暂停音频播放
param[in]         :PTIInst，无效参数
***********************************************************/
DRV_ErrCode DRV_AV_PauseAudio(U32 PTIInst);


/**********************************************************
Function Name  :DRV_AV_ResumeAudio
Description       :恢复音频播放
param[in]         :PTIInst，无效参数
***********************************************************/
DRV_ErrCode DRV_AV_ResumeAudio(U32 PTIInst);


/**********************************************************
Function Name  :DRV_AV_GetAudioType
Description       :获取视频编码格式
param[in]         :AudStreamContent，视频编码类型
***********************************************************/
DRV_ErrCode DRV_AV_GetAudioType(DRV_AudioType *AudStreamContent);


/*video 操作*/


/**********************************************************
Function Name  :DRV_AV_StopVideo
Description       :关闭视频播放器
param[in]         :PTIInst，无效参数
param[in]         :DisableOutputWindow，无效参数
***********************************************************/
DRV_ErrCode DRV_AV_StopVideo(U32 PTIInst, BOOL DisableOutputWindow);


/**********************************************************
Function Name  :DRV_AV_StartVideo，DRV_AV_StartVideoWithParam
Description       :开启视频播放器
param[in]         :PTIInst，无效参数
param[in]         :StreamType，无效参数
param[in]         :VidStartParams，无效参数
param[in]         :VidStreamContent，视频编码格式
***********************************************************/
DRV_ErrCode DRV_AV_StartVideo(U32 PTIInst, STVID_StreamType_t StreamType,DRV_VideoType VidStreamContent);
DRV_ErrCode DRV_AV_StartVideoWithParam(U32 PTIInst, STVID_StartParams_t VidStartParams,DRV_VideoType VidStreamContent);


/**********************************************************
Function Name  :DRV_AV_PauseVideo
Description       :暂停视频播放
param[in]         :VidDeviceId，无效参数
***********************************************************/
DRV_ErrCode DRV_AV_PauseVideo(VID_DeviceId_t VidDeviceId);


/**********************************************************
Function Name  :DRV_AV_ResumeVideo
Description       :恢复视频播放
param[in]         :VidDeviceId，无效参数
***********************************************************/
DRV_ErrCode DRV_AV_ResumeVideo(VID_DeviceId_t VidDeviceId);


/**********************************************************
Function Name  :DRV_AV_GetDmxStatus
Description       :获取当前DMX通道状态
param[in]         :VidValid，视频有无数据传输FALSE/TRUE
param[in]         :AudValid，音频有无数据传输FALSE/TRUE
param[in]         :TimeMs，等待超时时间，单位毫秒
***********************************************************/
DRV_ErrCode DRV_AV_GetDmxStatus(BOOL *VidValid,BOOL *AudValid,U32 TimeMs);


/**********************************************************
Function Name  :DRV_AVAPI_DTVResume，DRV_AV_SetDecodedPictures，DRV_DTV_ClearSlot
Description       :无效的接口
***********************************************************/
void DRV_AVAPI_DTVResume(STVID_StreamType_t StreamType,STPTI_Pid_t VidPID);
DRV_ErrCode DRV_AV_SetDecodedPictures(DRV_DecodedPictures_t type);
DRV_ErrCode DRV_DTV_ClearSlot(U32 PTIInst);



/*AV 操作*/


/**********************************************************
Function Name  :DRV_AVAPI_AVstart
Description       :开始播放流的音视频，标清节目
***********************************************************/
DRV_ErrCode DRV_AVAPI_AVstart(U32 decode_channel,
								   U32 ptiToUse, 
	 							   STPTI_Pid_t VidPID, 
								   STPTI_Pid_t AudPID, 
								   STPTI_Pid_t PcrPID, 
								   STPTI_Pid_t TtxPID, 
								   STPTI_Pid_t SubtPID);


/**********************************************************
Function Name  :DRV_AVAPI_AVstart_HD
Description       :开始播放流的音视频，高清节目
***********************************************************/
DRV_ErrCode DRV_AVAPI_AVstart_HD(U32 decode_channel,
								   U32 ptiToUse, 
	 							   STPTI_Pid_t VidPID, 
								   STPTI_Pid_t AudPID, 
								   STPTI_Pid_t PcrPID, 
								   STPTI_Pid_t TtxPID, 
								   STPTI_Pid_t SubtPID,
								   DRV_AudioType AudStreamContent,
								   DRV_VideoType VidStreamContent);


/**********************************************************
Function Name  :DRV_AVAPI_AVStatus_HD
Description       :获取当前节目音视频及PCR的PID
***********************************************************/
DRV_ErrCode DRV_AVAPI_AVStatus_HD(U32 *decode_channel,
								   U32 *ptiToUse, 
	 							   STPTI_Pid_t *VidPID, 
								   STPTI_Pid_t *AudPID, 
								   STPTI_Pid_t *PcrPID, 
								   STPTI_Pid_t *TtxPID, 
								   STPTI_Pid_t *SubtPID);


/**********************************************************
Function Name  :DRV_AVAPI_AVstop_HD
Description       :停止音视频播放
param[in]         :decode_channel，无效参数
                        与DRV_AVAPI_AVstop_HD接口同用
***********************************************************/
DRV_ErrCode DRV_AVAPI_AVstop(U32 decode_channel);


/**********************************************************
Function Name  :DRV_AVAPI_AVstop_HD
Description       :停止音视频播放
param[in]         :decode_channel，无效参数
param[in]         :PTIInst，无效参数，置为零即可
***********************************************************/
DRV_ErrCode DRV_AVAPI_AVstop_HD(U32 decode_channel,U32 PTIInst);


/**********************************************************
Function Name  :DRV_AV_Reset
Description       :复位AV播放
***********************************************************/
DRV_ErrCode DRV_AV_Reset(void);


/*视频指标相关设置*/


/**********************************************************
Function Name  :DRV_DENC_SetDefaultValue
Description       :获取当前亮度、对比度、饱和度
***********************************************************/
DRV_ErrCode DRV_DENC_SetDefaultValue(void);


/**********************************************************
Function Name  :DRV_BrightAdjust
Description       :设置亮度值
param[in]         :亮度值为Brightness_Offset+50
***********************************************************/
DRV_ErrCode DRV_BrightAdjust(S8_YS Brightness_Offset);


/**********************************************************
Function Name  :DRV_ContraAdjust
Description       :设置对比度值
param[in]         :对比度值为Contrast_Offset+50
***********************************************************/
DRV_ErrCode DRV_ContraAdjust(S8_YS Contrast_Offset);


/**********************************************************
Function Name  :DRV_SaturaAdjust
Description       :设置饱和度值
param[in]         :饱和度值为Saturation_Offset+50
***********************************************************/
DRV_ErrCode DRV_SaturaAdjust(S8_YS Saturation_Offset);





/*ysvidinject.c相关文件*/


/**********************************************************
Function Name  :drv_InjectIframe
Description       :灌入数据，并解码I帧
param[in]         :pData，数据流
param[in]         :uiDataLength，数据长度
param[in]         :pstCapPicture，当pstCapPicture为空指针时，解完的I帧将在window上显示，
                        如果非空，则不会显示而是将I帧信息上报
***********************************************************/
DRV_ErrCode drv_InjectIframe(U8 * pData,U32 uiDataLength,HI_UNF_CAPTURE_PICTURE_S *pstCapPicture);


/**********************************************************
Function Name  :InjectMpeg
Description       :灌入数据，并显示最后一帧
param[in]         :pData，数据流
param[in]         :uiDataLength，数据长度
***********************************************************/
DRV_ErrCode InjectMpeg(U8_YS * pData,U32_YS uiDataLength);

/**********************************************************
Function Name  :
Description    : 更新开机logo
param[in]      : pData，数据流
param[in]      : uiDataLength，数据长度
***********************************************************/
DRV_ErrCode DRV_LogoUpdate(U8 * pData,U32 uiDataLength);

/**********************************************************
Function Name  :
Description    : 设置是否打开logo
param[in]      : disp [in] : 1:on 0:off
param[in]      : uiDataLength，数据长度
***********************************************************/
DRV_ErrCode DRV_LogoDisplay(bool disp);

/**********************************************************
Function Name  :DRV_CreatStillRegion
Description       :创建STILL
***********************************************************/
DRV_ErrCode DRV_CreatStillRegion(void);


/**********************************************************
Function Name  :DRV_AudioInjectStop
Description       :清除I帧显示
***********************************************************/
DRV_ErrCode  drv_CleanIframe(void);




/*ysaudplay.c相关文件*/



/**********************************************************
Function Name  :DRV_AudioInjectStart
Description       :开启音频播放器，并注入数据播放
param[in]         :DeviceId，无效参数，置为零即可
param[in]         :AUD_StartParams，无效参数，置为零即可
param[in]         :NbLoops，无效参数，置为零即可
param[in]         :DataSource_p，注入的数据结构
***********************************************************/
DRV_ErrCode DRV_AudioInjectStart(U32 DeviceId,DRV_AudioInjectSource *DataSource_p,STAUD_StartParams_t *AUD_StartParams,U32 NbLoops);


/**********************************************************
Function Name  :DRV_AudioInjectStop
Description       :关闭音频播放器
param[in]         :DeviceId，无效参数，置为零即可
***********************************************************/
DRV_ErrCode DRV_AudioInjectStop(U32 DeviceId);


/**********************************************************
Function Name  :DRV_AudioInjectPause
Description       :暂停音频播放器
param[in]         :DeviceId，无效参数，置为零即可
***********************************************************/
DRV_ErrCode DRV_AudioInjectPause(U32 DeviceId);


/**********************************************************
Function Name  :DRV_AudioInjectResume
Description       :恢复音频播放器
param[in]         :DeviceId，无效参数，置为零即可
***********************************************************/
DRV_ErrCode DRV_AudioInjectResume(U32 DeviceId);


/**********************************************************
Function Name  :DRV_AudioInjectEnableCallback
Description       :注册回调函数
param[in]         :Callback_f，回调函数
***********************************************************/
DRV_ErrCode DRV_AudioInjectEnableCallback(DRV_PlayRecCallback Callback_f);


/**********************************************************
Function Name  :DRV_AudioInjectDisableCallback
Description       :反注册回调函数
***********************************************************/
DRV_ErrCode DRV_AudioInjectDisableCallback(void);




/*ysesinject.c*/


/******************************************************
Function Name  :DRV_AudioESInjectEnableCallback
Description       :开启回调函数
param[in]         :Callback_f，回调函数
*******************************************************/
DRV_ErrCode DRV_AudioESInjectEnableCallback(DRV_PlayRecCallback Callback_f);


/******************************************************
Function Name  :DRV_AudioESInjectEnableCallback
Description       :关闭回调函数
*******************************************************/
DRV_ErrCode DRV_AudioESInjectDisableCallback(void);


/******************************************************
Function Name  :DRV_ESInjectSetup
Description       :从直播切换到ES注入
*******************************************************/
DRV_ErrCode DRV_ESInjectSetup(void);


/******************************************************
Function Name  :DRV_ESInjectTerm
Description       :从ES注入切换到直播
*******************************************************/
DRV_ErrCode DRV_ESInjectTerm(void);


/******************************************************
Function Name  :DRV_AudioESInjectStart
Description       :开启音频播放器
param[in]         :AudStreamContent，音频编码格式
*******************************************************/
DRV_ErrCode DRV_AudioESInjectStart(DRV_AudioType AudStreamContent);


/******************************************************
Function Name  :DRV_VideoESInjectStart
Description       :开启视频播放器
param[in]         :VidStreamContent，视频编码格式
*******************************************************/
DRV_ErrCode DRV_VideoESInjectStart(DRV_VideoType VidStreamContent);


/******************************************************
Function Name  :DRV_VideoESInjectBuffer
Description       :开始注入视频数据
param[in]         :DataSource_p，视频数据结构
*******************************************************/
DRV_ErrCode DRV_VideoESInjectBuffer(DRV_VideoESInjectSource *DataSource_p);


/******************************************************
Function Name  :DRV_AudioESInjectBuffer
Description       :开始注入音频
param[in]         :DataSource_p，音频数据结构
*******************************************************/
DRV_ErrCode DRV_AudioESInjectBuffer(DRV_AudioESInjectSource *DataSource_p);


/******************************************************
Function Name  :DRV_AudioESInjectStop
Description       :停止播放音频
*******************************************************/
DRV_ErrCode DRV_AudioESInjectStop(void);


/******************************************************
Function Name  :DRV_AudioESInjectPause
Description       :暂停播放
*******************************************************/
DRV_ErrCode DRV_AudioESInjectPause(void);


/******************************************************
Function Name  :DRV_AudioESInjectResume
Description       :恢复播放
*******************************************************/
DRV_ErrCode DRV_AudioESInjectResume(void);


/******************************************************
Function Name  :DRV_VideoESInjectStop
Description       :停止播放视频
*******************************************************/
DRV_ErrCode DRV_VideoESInjectStop(void);


/******************************************************
Function Name  :DRV_VideoESInjectPause
Description       :暂停播放
*******************************************************/
DRV_ErrCode DRV_VideoESInjectPause(void);


/******************************************************
Function Name  :DRV_VideoESInjectResume
Description       :恢复播放
*******************************************************/
DRV_ErrCode DRV_VideoESInjectResume(void);



/*yspesinject.c相关文件*/



/*****************************************************************
Function Name  :DRV_PesInjectSetup
Description       :从直播切换到pes注入，关闭直播和去初始化
******************************************************************/
DRV_ErrCode DRV_PesInjectSetup(void);


/*****************************************************************
Function Name  :DRV_PesInjectTerm
Description       :从pes注入切换到直播，打开音视频通道等
*****************************************************************/
DRV_ErrCode DRV_PesInjectTerm(void);


/******************************************************
Function Name  :DRV_VideoPesInjectStart
Description       :开启PES视频播放器
param[in]         :VidStartParams，无效参数，置为零即可
param[in]         :VidStreamContent，视频编码格式
*******************************************************/
DRV_ErrCode DRV_VideoPesInjectStart(STVID_StartParams_t VidStartParams,DRV_VideoType VidStreamContent);


/********************************************************
Function Name  :DRV_VideoPesInjectStop
Description       :关闭PES视频播放器
param[in]         :DisableLastFrame，无效参数，置为零即可
param[in]         :VidStreamContent，无效参数，置为零即可
*********************************************************/
DRV_ErrCode DRV_VideoPesInjectStop(DRV_VideoType VidStreamContent, BOOL DisableLastFrame);


/*********************************************
Function Name  :DRV_VideoPesInjectPause
Description       :暂停AV播放，进入PAUSE状态
param[in]         :VidStreamContent，视频编码格式
**********************************************/
DRV_ErrCode DRV_VideoPesInjectPause(DRV_VideoType VidStreamContent);


/*********************************************
Function Name  :DRV_VideoPesInjectPause
Description       :恢复AV播放，进入PLAY状态
param[in]         :VidStreamContent，视频编码格式
**********************************************/
DRV_ErrCode DRV_VideoPesInjectResume(DRV_VideoType VidStreamContent);


/*********************************************
Function Name  :DRV_VideoPesInjectCopy
Description       :COPY要播放的PES数据
param[in]         :consumer，注入的buffer数据
param[in]         :SizeToCopy，数据长度
param[in]         :VidStreamContent，视频编码格式
**********************************************/
DRV_ErrCode DRV_VideoPesInjectCopy(U8 *consumer,U32 SizeToCopy,DRV_VideoType VidStreamContent);


/****************************************************************
Function Name  :DRV_VideoPesGetPesBufferFreeSize，DRV_VideoPesInjectGetFreeSize
Description       :获取缓冲区剩余的数据大小
param[in]         :FreeSize，数据大小
param[in]         :VidStreamContent，视频编码格式
****************************************************************/
DRV_ErrCode DRV_VideoPesGetPesBufferFreeSize(U32 *FreeSize,DRV_VideoType VidStreamContent);
DRV_ErrCode DRV_VideoPesInjectGetFreeSize(U32 *FreeSize,DRV_VideoType VidStreamContent);


/*********************************************
Function Name  :DRV_VideoPesGetPesBufferSize
Description       :获取缓冲区的数据大小
param[in]         :BufferSize，数据大小
param[in]         :VidStreamContent，视频编码格式
**********************************************/
DRV_ErrCode DRV_VideoPesGetPesBufferSize(U32 *BufferSize,DRV_VideoType VidStreamContent);


/******************************************************
Function Name  :DRV_VideoPesGetPesBufferSize,DRV_VideoFlushBitBuffer
Description       :复位AVPLAY buffer
param[in]         :VidStreamContent，视频编码格式
*******************************************************/
DRV_ErrCode DRV_VideoPesFlushBuffer(DRV_VideoType VidStreamContent);
DRV_ErrCode DRV_VideoFlushBitBuffer(DRV_VideoType VidStreamContent);


/*********************************************
Function Name  :DRV_VideoPesGetPesBufferSize
Description       :获取码流的码率
param[in]         :Rate_p，码率
param[in]         :VidStreamContent，视频编码格式
**********************************************/
DRV_ErrCode	DRV_VideoPesInjectBufferRate(U32 *Rate_p,DRV_VideoType VidStreamContent);


/*********************************************************
Function Name  :DRV_VideoPesSetSpeed
Description       :设置播放的速度
param[in]         :speed，播放速度，目前只支持2倍快进
**********************************************************/
DRV_ErrCode DRV_VideoPesSetSpeed(int speed);


/**********************************************************
Function Name  :DRV_AudioPesInjectStart
Description       :开启PES音频播放器
param[in]         :AudStartParams，无效参数，置为零即可
param[in]         :AudPCMInputParams，无效参数，置为零即可
param[in]         :AudStreamContent，音频编码格式
***********************************************************/
DRV_ErrCode DRV_AudioPesInjectStart(STAUD_StartParams_t AudStartParams,STAUD_PCMInputParams_t AudPCMInputParams,DRV_AudioType AudStreamContent);


/*********************************************
Function Name  :DRV_AudioPesInjectStop
Description       :关闭PES音频播放器
**********************************************/
DRV_ErrCode DRV_AudioPesInjectStop(void);


/*********************************************
Function Name  :DRV_AudioPesInjectPause
Description       :暂停AV播放器
**********************************************/
DRV_ErrCode DRV_AudioPesInjectPause(void);


/*********************************************
Function Name  :DRV_AudioPesInjectResume
Description       :恢复AV播放器
**********************************************/
DRV_ErrCode DRV_AudioPesInjectResume(void);


/*********************************************
Function Name  :DRV_AudioPesInjectCopy
Description       :COPY要播放的PES数据
param[in]         :consumer，注入的buffer数据
param[in]         :SizeToCopy，数据长度
**********************************************/
DRV_ErrCode DRV_AudioPesInjectCopy(U8 *consumer,U32 SizeToCopy);


/*********************************************************
Function Name  :DRV_AudioPesFlushBuffer,DRV_AudioFlushBitBuffer
Description       :复位AVPLAY buffer
param[in]         :AudStartParams，无效参数，置为零即可
param[in]         :AudStreamContent，无效参数，置为零即可
**********************************************************/
DRV_ErrCode DRV_AudioPesFlushBuffer(void);
DRV_ErrCode DRV_AudioFlushBitBuffer(STAUD_StartParams_t AudStartParams,DRV_AudioType AudStreamContent);


/*****************************************************************
Function Name  :DRV_AudioPesGetPesBufferFreeSize，DRV_AudioPesInjectGetFreeSize
Description       :获取缓冲区剩余的数据大小
param[in]         :FreeSize，数据大小
******************************************************************/
DRV_ErrCode DRV_AudioPesGetPesBufferFreeSize(U32 *FreeSize);
DRV_ErrCode DRV_AudioPesInjectGetFreeSize(U32 *FreeSize);


/*********************************************
Function Name  :DRV_AudioPesInjectBufferRate
Description       :获取码流的码率
param[in]         :Rate_p，码率
**********************************************/
DRV_ErrCode	DRV_AudioPesInjectBufferRate(U32 *Rate_p);


/**********************************************************
Function Name  :DRV_AudioPesSetSpeed
Description       :设置播放的速度
param[in]         :speed，播放速度，目前只支持2倍快进
***********************************************************/
DRV_ErrCode DRV_AudioPesSetSpeed(int speed);

/* C++ support */
/* ----------- */
#ifdef __cplusplus
}
#endif
#endif

