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
    DRV_AUDIO_TYPE_DOLBY_PLUS /*�ű�*/
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
	DRV_PLAYREC_REACH_END_OF_PLAYBACK_EVT,/*���ﲥ����ĩλ��*/
	DRV_PLAYREC_REACH_BEGIN_OF_PLAYBACK_EVT,/*���ﲥ������ʼλ��*/
	DRV_PLAYREC_PLAY_DECODING_ERROR_EVT,/*���ų��ִ��󣬱���overflow,underflow,�������,Ӧ�ÿ��Ժ���*/
	DRV_PLAYREC_PLAY_FILE_NOT_FOUND_EVT,/*�ļ�û���ҵ�*/
	DRV_PLAYREC_PLAY_READ_ERROR_EVT,/*�����ж�ȡ�ļ�����*/
	DRV_PLAYREC_PLAY_SEEK_ERROR_EVT,/*��������ת�ļ�����*/
	DRV_PLAYREC_RECORD_WRITE_ERROR_EVT,/*¼����д�ļ�����*/
	DRV_PLAYREC_REACH_END_OF_FILE_EVT,/*¼�ƹ����е����ļ�����*/
	DRV_PLAYREC_PLAY_REACH_RECORD_EVT,/*timeshift׷��live*/
	DRV_PLAYREC_DISK_LOW_LEVEL_EVT/*¼�ƹ����У����̿ռ䲻��*/
}DRV_PlayerRecEvt;

typedef void (*DRV_PlayRecCallback)(DRV_PlayerRecEvt event,void *Payload);

typedef enum DRV_AvSynMode_e
{
	DRV_AVSYN_NONE,/*�����κ�ͬ��,Ҳ����PCR*/
	DRV_AVSYN_PCR,/*PCRͬ����Ĭ������Ƶ��enable����AUD,VID�޻ص�*/
	DRV_AVSYN_AUD_PTS,/*PCR enable��Vidͬ��enable,audͬ��disable,aud�лص�*/
	DRV_AVSYN_VID_PTS,/*PCR enable��audͬ��enable,Vidͬ��disable,aud�лص�*/
	DRV_AVSYN_NONE_WITH_PCR/*PCR enable��audͬ��disable,Vidͬ��disable,aud�лص�*/
}DRV_AvSynMode;

/*extern av ���ȫ�ֱ�����һ������ģ��ʹ��*/
extern HI_HANDLE  mFfmpegPlayHandle;


extern DRV_Pid m_curVidPid;/*video��ǰ��PID*/
extern DRV_Pid m_curAudPid;/*audio��ǰ��PID*/
extern DRV_Pid m_curPcrPid;/*pcr��ǰ��PID*/
extern DRV_Pid m_curTtxPid;/*txt��ǰ��PID*/
extern DRV_AvSynMode m_DecodeCurAvSynMode[];/*��ǰ��ͬ��ģʽ*/
extern DRV_AvSynMode m_DecodeLastAvSynMode[];/*��������ʼ��ʱ��ͬ��ģʽ*/
extern BOOL_YS m_bEnableVideo;/*video��ǰ��״̬��TRUEΪstart��FALSEΪstop*/
extern BOOL_YS m_bEnableAudio;/*audio��ǰ��״̬��TRUEΪstart��FALSEΪstop*/
extern BOOL_YS m_bEnablePCR;/*pcr��ǰ��״̬��TRUEΪstart��FALSEΪstop*/
extern DRV_VideoType m_curVidStreamContent;
extern DRV_VideoType m_LastVidStreamContent;
extern BOOL m_IsEnableDolbyOutput;
extern BOOL m_DisableOutPutWhenVideoStop;
extern BOOL m_IsNPSwitchAuto;
extern BOOL m_OutputAlreadyEnabled;


/**********************************************************
Function Name  :DRV_AvSynModeSet��DRV_AvSynModeSetWithParam
Description       :����ͬ������
param[in]         :SynMode��ͬ��ģʽ
param[in]         :VidStreamContent����Ч����
***********************************************************/
DRV_ErrCode DRV_AvSynModeSet(DRV_AvSynMode SynMode);
DRV_ErrCode DRV_AvSynModeSetWithParam(DRV_AvSynMode SynMode,DRV_VideoType VidStreamContent);


/**********************************************************
Function Name  :drv_AvSynModeChangeCheck
Description       :���ͬ��ģʽ�Ƿ�ת����
param[in]         :LastSynMode��ͬ��ģʽ
param[in]         :CurSynMode��ͬ��ģʽ
***********************************************************/
BOOL drv_AvSynModeChangeCheck(DRV_AvSynMode LastSynMode,DRV_AvSynMode CurSynMode);


/**********************************************************
Function Name  :DRV_ClkRVEnable
Description       :PCRͬ��
***********************************************************/
void DRV_ClkRVEnable(void);


/**********************************************************
Function Name  :DRV_ClkRVDisable
Description       :�����κ�ͬ��
***********************************************************/
void DRV_ClkRVDisable(void);


/**********************************************************
Function Name  :DRV_DENC_DisableVideo
Description       :�ر���Ƶ��ͼ�β����ʾ���
                        һ��ֻ�ڴ���ʱ��
***********************************************************/
DRV_ErrCode  DRV_DENC_DisableVideo(void);


/**********************************************************
Function Name  :DRV_DENC_EnableVideo
Description       :����Ƶ��ͼ�β����ʾ���
***********************************************************/
DRV_ErrCode  DRV_DENC_EnableVideo(void);


/**********************************************************
Function Name  :DRV_VideoEnable
Description       :����Ƶ����ʾ���
***********************************************************/
DRV_ErrCode DRV_VideoEnable(void);


/**********************************************************
Function Name  :DRV_VideoDisable
Description       :�ر���Ƶ�㣬����
***********************************************************/
DRV_ErrCode DRV_VideoDisable(void);


/**********************************************************
Function Name  :DRV_VideoGetVisiable
Description       :��ȡ��Ƶ�Ƿ����TRUE/FALSE
***********************************************************/
BOOL DRV_VideoGetVisiable(void);


/**********************************************************
Function Name  :DRV_AV_MuteAudio
Description       :���þ���
***********************************************************/
DRV_ErrCode DRV_AV_MuteAudio(void);


/**********************************************************
Function Name  :DRV_AV_UnMuteAudio
Description       :ȡ������
***********************************************************/
DRV_ErrCode DRV_AV_UnMuteAudio(void);


/**********************************************************
Function Name  :DRV_AV_SetAudio
Description       :��������0Ϊ���63Ϊ��С
***********************************************************/
DRV_ErrCode DRV_AV_SetAudio ( U8_YS left, U8_YS right);


/**********************************************************
Function Name  :DRV_SetScreenNoBlack
Description       :������̨������
***********************************************************/
void DRV_SetScreenNoBlack(void);


/**********************************************************
Function Name  :DRV_SetScreenBlack
Description       :������̨����
***********************************************************/
void DRV_SetScreenBlack(void);


/**********************************************************
Function Name  :DRV_SetNPSwitchNotAuto
Description       :����NP�Ʋ��Ǹ������������Զ��л�
***********************************************************/
void DRV_SetNPSwitchNotAuto(void);


/**********************************************************
Function Name  :DRV_SetNPSwitchAuto
Description       :����NP���Ǹ������������Զ��л�
***********************************************************/
void DRV_SetNPSwitchAuto(void);


/**********************************************************
Function Name  :DRV_AV_GetVideoWin
Description       :��ȡ�����������λ�úʹ�С
param[in]         :pstRectIn�����봰��λ�úʹ�С
param[in]         :pstRectOut���������λ�úʹ�С
***********************************************************/
DRV_ErrCode DRV_AV_GetVideoWin(DRV_RectWh *pstRectIn,DRV_RectWh *pstRectOut);


/**********************************************************
Function Name  :DRV_AV_SetVideoWin
Description       :���������������λ�úʹ�С
param[in]         :pstRectIn�����봰��λ�úʹ�С
param[in]         :pstRectOut���������λ�úʹ�С
param[in]         :AutoMode����Ϊ0ʱ�����Ϊ1920*1080
***********************************************************/
DRV_ErrCode DRV_AV_SetVideoWin(DRV_RectWh *pstRectIn, DRV_RectWh *pstRectOut, BOOL AutoMode);


/**********************************************************
Function Name  :drv_VideoSetMode
Description       :������Ƶ��ʾ��ʽ
param[in]         :mode����Ƶ��ʽ
***********************************************************/
DRV_ErrCode drv_VideoSetMode(DRV_VtgTimingMode_t mode);


/**********************************************************
Function Name  :DRV_EnableDolbyOutput
Description       :���öű�5.1���
param[in]         :AC3��ʽ
***********************************************************/
DRV_ErrCode DRV_EnableDolbyOutput(HI_BOOL bbhdmipassthrough,HI_BOOL bspdifpassthrough);


/**********************************************************
Function Name  :DRV_DisableDolbyOutput
Description       :���öű�5.1���
param[in]         :PCM��ʽ
***********************************************************/
DRV_ErrCode DRV_DisableDolbyOutput(void);

/**********************************************************
Function Name  :DRV_GetAudioType
Description       :��ȡ��ǰ��Ƶ��ʽ
param[in]         :��Ƶ��ʽ
***********************************************************/
DRV_ErrCode DRV_GetAudioType(DRV_AudioType *AudStreamContent);

/**********************************************************
Function Name  :DRV_SetVoiceMode
Description       :������Ƶ����ģʽ
param[in]         :mode��0(�����) 1(������) 2(������) 3(������)
***********************************************************/
DRV_ErrCode  DRV_SetVoiceMode(char  mode);


/**********************************************************
Function Name  :DRV_SetScreenRatio
Description       :������Ļ��߱�
param[in]         :mode�� 0(4:3) 1( 16:9)  2(auto)
***********************************************************/
DRV_ErrCode  DRV_SetScreenRatio(char  mode);


/**********************************************************
Function Name  :DRV_SetScreenRatioConversion
Description       :���ô��ڵĿ��ת����ʽ
param[in]         :mode��ת����ʽ
***********************************************************/
DRV_ErrCode  DRV_SetScreenRatioConversion(DRV_ScreenRatioConversion  mode);


/*audio ����*/


/**********************************************************
Function Name  :DRV_AV_StopAudio
Description       :�ر���Ƶ������
param[in]         :PTIInst����Ч����
***********************************************************/
DRV_ErrCode  DRV_AV_StopAudio(U32 PTIInst);


/**********************************************************
Function Name  :DRV_AV_StartAudio��DRV_AV_StartAudioWithParam
Description       :������Ƶ������
param[in]         :PTIInst����Ч����
param[in]         :StreamType����Ч����
param[in]         :AudPID����Ч����
param[in]         :AudStartParams����Ч����
param[in]         :AudStreamContent����Ƶ�����ʽ
***********************************************************/
DRV_ErrCode DRV_AV_StartAudio(U32 PTIInst,STAUD_StreamType_t StreamType, STPTI_Pid_t AudPID,DRV_AudioType AudStreamContent);
DRV_ErrCode DRV_AV_StartAudioWithParam(U32 PTIInst,STAUD_StartParams_t AudStartParams,DRV_AudioType AudStreamContent);


/**********************************************************
Function Name  :DRV_AV_PauseAudio
Description       :��ͣ��Ƶ����
param[in]         :PTIInst����Ч����
***********************************************************/
DRV_ErrCode DRV_AV_PauseAudio(U32 PTIInst);


/**********************************************************
Function Name  :DRV_AV_ResumeAudio
Description       :�ָ���Ƶ����
param[in]         :PTIInst����Ч����
***********************************************************/
DRV_ErrCode DRV_AV_ResumeAudio(U32 PTIInst);


/**********************************************************
Function Name  :DRV_AV_GetAudioType
Description       :��ȡ��Ƶ�����ʽ
param[in]         :AudStreamContent����Ƶ��������
***********************************************************/
DRV_ErrCode DRV_AV_GetAudioType(DRV_AudioType *AudStreamContent);


/*video ����*/


/**********************************************************
Function Name  :DRV_AV_StopVideo
Description       :�ر���Ƶ������
param[in]         :PTIInst����Ч����
param[in]         :DisableOutputWindow����Ч����
***********************************************************/
DRV_ErrCode DRV_AV_StopVideo(U32 PTIInst, BOOL DisableOutputWindow);


/**********************************************************
Function Name  :DRV_AV_StartVideo��DRV_AV_StartVideoWithParam
Description       :������Ƶ������
param[in]         :PTIInst����Ч����
param[in]         :StreamType����Ч����
param[in]         :VidStartParams����Ч����
param[in]         :VidStreamContent����Ƶ�����ʽ
***********************************************************/
DRV_ErrCode DRV_AV_StartVideo(U32 PTIInst, STVID_StreamType_t StreamType,DRV_VideoType VidStreamContent);
DRV_ErrCode DRV_AV_StartVideoWithParam(U32 PTIInst, STVID_StartParams_t VidStartParams,DRV_VideoType VidStreamContent);


/**********************************************************
Function Name  :DRV_AV_PauseVideo
Description       :��ͣ��Ƶ����
param[in]         :VidDeviceId����Ч����
***********************************************************/
DRV_ErrCode DRV_AV_PauseVideo(VID_DeviceId_t VidDeviceId);


/**********************************************************
Function Name  :DRV_AV_ResumeVideo
Description       :�ָ���Ƶ����
param[in]         :VidDeviceId����Ч����
***********************************************************/
DRV_ErrCode DRV_AV_ResumeVideo(VID_DeviceId_t VidDeviceId);


/**********************************************************
Function Name  :DRV_AV_GetDmxStatus
Description       :��ȡ��ǰDMXͨ��״̬
param[in]         :VidValid����Ƶ�������ݴ���FALSE/TRUE
param[in]         :AudValid����Ƶ�������ݴ���FALSE/TRUE
param[in]         :TimeMs���ȴ���ʱʱ�䣬��λ����
***********************************************************/
DRV_ErrCode DRV_AV_GetDmxStatus(BOOL *VidValid,BOOL *AudValid,U32 TimeMs);


/**********************************************************
Function Name  :DRV_AVAPI_DTVResume��DRV_AV_SetDecodedPictures��DRV_DTV_ClearSlot
Description       :��Ч�Ľӿ�
***********************************************************/
void DRV_AVAPI_DTVResume(STVID_StreamType_t StreamType,STPTI_Pid_t VidPID);
DRV_ErrCode DRV_AV_SetDecodedPictures(DRV_DecodedPictures_t type);
DRV_ErrCode DRV_DTV_ClearSlot(U32 PTIInst);



/*AV ����*/


/**********************************************************
Function Name  :DRV_AVAPI_AVstart
Description       :��ʼ������������Ƶ�������Ŀ
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
Description       :��ʼ������������Ƶ�������Ŀ
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
Description       :��ȡ��ǰ��Ŀ����Ƶ��PCR��PID
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
Description       :ֹͣ����Ƶ����
param[in]         :decode_channel����Ч����
                        ��DRV_AVAPI_AVstop_HD�ӿ�ͬ��
***********************************************************/
DRV_ErrCode DRV_AVAPI_AVstop(U32 decode_channel);


/**********************************************************
Function Name  :DRV_AVAPI_AVstop_HD
Description       :ֹͣ����Ƶ����
param[in]         :decode_channel����Ч����
param[in]         :PTIInst����Ч��������Ϊ�㼴��
***********************************************************/
DRV_ErrCode DRV_AVAPI_AVstop_HD(U32 decode_channel,U32 PTIInst);


/**********************************************************
Function Name  :DRV_AV_Reset
Description       :��λAV����
***********************************************************/
DRV_ErrCode DRV_AV_Reset(void);


/*��Ƶָ���������*/


/**********************************************************
Function Name  :DRV_DENC_SetDefaultValue
Description       :��ȡ��ǰ���ȡ��Աȶȡ����Ͷ�
***********************************************************/
DRV_ErrCode DRV_DENC_SetDefaultValue(void);


/**********************************************************
Function Name  :DRV_BrightAdjust
Description       :��������ֵ
param[in]         :����ֵΪBrightness_Offset+50
***********************************************************/
DRV_ErrCode DRV_BrightAdjust(S8_YS Brightness_Offset);


/**********************************************************
Function Name  :DRV_ContraAdjust
Description       :���öԱȶ�ֵ
param[in]         :�Աȶ�ֵΪContrast_Offset+50
***********************************************************/
DRV_ErrCode DRV_ContraAdjust(S8_YS Contrast_Offset);


/**********************************************************
Function Name  :DRV_SaturaAdjust
Description       :���ñ��Ͷ�ֵ
param[in]         :���Ͷ�ֵΪSaturation_Offset+50
***********************************************************/
DRV_ErrCode DRV_SaturaAdjust(S8_YS Saturation_Offset);





/*ysvidinject.c����ļ�*/


/**********************************************************
Function Name  :drv_InjectIframe
Description       :�������ݣ�������I֡
param[in]         :pData��������
param[in]         :uiDataLength�����ݳ���
param[in]         :pstCapPicture����pstCapPictureΪ��ָ��ʱ�������I֡����window����ʾ��
                        ����ǿգ��򲻻���ʾ���ǽ�I֡��Ϣ�ϱ�
***********************************************************/
DRV_ErrCode drv_InjectIframe(U8 * pData,U32 uiDataLength,HI_UNF_CAPTURE_PICTURE_S *pstCapPicture);


/**********************************************************
Function Name  :InjectMpeg
Description       :�������ݣ�����ʾ���һ֡
param[in]         :pData��������
param[in]         :uiDataLength�����ݳ���
***********************************************************/
DRV_ErrCode InjectMpeg(U8_YS * pData,U32_YS uiDataLength);

/**********************************************************
Function Name  :
Description    : ���¿���logo
param[in]      : pData��������
param[in]      : uiDataLength�����ݳ���
***********************************************************/
DRV_ErrCode DRV_LogoUpdate(U8 * pData,U32 uiDataLength);

/**********************************************************
Function Name  :
Description    : �����Ƿ��logo
param[in]      : disp [in] : 1:on 0:off
param[in]      : uiDataLength�����ݳ���
***********************************************************/
DRV_ErrCode DRV_LogoDisplay(bool disp);

/**********************************************************
Function Name  :DRV_CreatStillRegion
Description       :����STILL
***********************************************************/
DRV_ErrCode DRV_CreatStillRegion(void);


/**********************************************************
Function Name  :DRV_AudioInjectStop
Description       :���I֡��ʾ
***********************************************************/
DRV_ErrCode  drv_CleanIframe(void);




/*ysaudplay.c����ļ�*/



/**********************************************************
Function Name  :DRV_AudioInjectStart
Description       :������Ƶ����������ע�����ݲ���
param[in]         :DeviceId����Ч��������Ϊ�㼴��
param[in]         :AUD_StartParams����Ч��������Ϊ�㼴��
param[in]         :NbLoops����Ч��������Ϊ�㼴��
param[in]         :DataSource_p��ע������ݽṹ
***********************************************************/
DRV_ErrCode DRV_AudioInjectStart(U32 DeviceId,DRV_AudioInjectSource *DataSource_p,STAUD_StartParams_t *AUD_StartParams,U32 NbLoops);


/**********************************************************
Function Name  :DRV_AudioInjectStop
Description       :�ر���Ƶ������
param[in]         :DeviceId����Ч��������Ϊ�㼴��
***********************************************************/
DRV_ErrCode DRV_AudioInjectStop(U32 DeviceId);


/**********************************************************
Function Name  :DRV_AudioInjectPause
Description       :��ͣ��Ƶ������
param[in]         :DeviceId����Ч��������Ϊ�㼴��
***********************************************************/
DRV_ErrCode DRV_AudioInjectPause(U32 DeviceId);


/**********************************************************
Function Name  :DRV_AudioInjectResume
Description       :�ָ���Ƶ������
param[in]         :DeviceId����Ч��������Ϊ�㼴��
***********************************************************/
DRV_ErrCode DRV_AudioInjectResume(U32 DeviceId);


/**********************************************************
Function Name  :DRV_AudioInjectEnableCallback
Description       :ע��ص�����
param[in]         :Callback_f���ص�����
***********************************************************/
DRV_ErrCode DRV_AudioInjectEnableCallback(DRV_PlayRecCallback Callback_f);


/**********************************************************
Function Name  :DRV_AudioInjectDisableCallback
Description       :��ע��ص�����
***********************************************************/
DRV_ErrCode DRV_AudioInjectDisableCallback(void);




/*ysesinject.c*/


/******************************************************
Function Name  :DRV_AudioESInjectEnableCallback
Description       :�����ص�����
param[in]         :Callback_f���ص�����
*******************************************************/
DRV_ErrCode DRV_AudioESInjectEnableCallback(DRV_PlayRecCallback Callback_f);


/******************************************************
Function Name  :DRV_AudioESInjectEnableCallback
Description       :�رջص�����
*******************************************************/
DRV_ErrCode DRV_AudioESInjectDisableCallback(void);


/******************************************************
Function Name  :DRV_ESInjectSetup
Description       :��ֱ���л���ESע��
*******************************************************/
DRV_ErrCode DRV_ESInjectSetup(void);


/******************************************************
Function Name  :DRV_ESInjectTerm
Description       :��ESע���л���ֱ��
*******************************************************/
DRV_ErrCode DRV_ESInjectTerm(void);


/******************************************************
Function Name  :DRV_AudioESInjectStart
Description       :������Ƶ������
param[in]         :AudStreamContent����Ƶ�����ʽ
*******************************************************/
DRV_ErrCode DRV_AudioESInjectStart(DRV_AudioType AudStreamContent);


/******************************************************
Function Name  :DRV_VideoESInjectStart
Description       :������Ƶ������
param[in]         :VidStreamContent����Ƶ�����ʽ
*******************************************************/
DRV_ErrCode DRV_VideoESInjectStart(DRV_VideoType VidStreamContent);


/******************************************************
Function Name  :DRV_VideoESInjectBuffer
Description       :��ʼע����Ƶ����
param[in]         :DataSource_p����Ƶ���ݽṹ
*******************************************************/
DRV_ErrCode DRV_VideoESInjectBuffer(DRV_VideoESInjectSource *DataSource_p);


/******************************************************
Function Name  :DRV_AudioESInjectBuffer
Description       :��ʼע����Ƶ
param[in]         :DataSource_p����Ƶ���ݽṹ
*******************************************************/
DRV_ErrCode DRV_AudioESInjectBuffer(DRV_AudioESInjectSource *DataSource_p);


/******************************************************
Function Name  :DRV_AudioESInjectStop
Description       :ֹͣ������Ƶ
*******************************************************/
DRV_ErrCode DRV_AudioESInjectStop(void);


/******************************************************
Function Name  :DRV_AudioESInjectPause
Description       :��ͣ����
*******************************************************/
DRV_ErrCode DRV_AudioESInjectPause(void);


/******************************************************
Function Name  :DRV_AudioESInjectResume
Description       :�ָ�����
*******************************************************/
DRV_ErrCode DRV_AudioESInjectResume(void);


/******************************************************
Function Name  :DRV_VideoESInjectStop
Description       :ֹͣ������Ƶ
*******************************************************/
DRV_ErrCode DRV_VideoESInjectStop(void);


/******************************************************
Function Name  :DRV_VideoESInjectPause
Description       :��ͣ����
*******************************************************/
DRV_ErrCode DRV_VideoESInjectPause(void);


/******************************************************
Function Name  :DRV_VideoESInjectResume
Description       :�ָ�����
*******************************************************/
DRV_ErrCode DRV_VideoESInjectResume(void);



/*yspesinject.c����ļ�*/



/*****************************************************************
Function Name  :DRV_PesInjectSetup
Description       :��ֱ���л���pesע�룬�ر�ֱ����ȥ��ʼ��
******************************************************************/
DRV_ErrCode DRV_PesInjectSetup(void);


/*****************************************************************
Function Name  :DRV_PesInjectTerm
Description       :��pesע���л���ֱ����������Ƶͨ����
*****************************************************************/
DRV_ErrCode DRV_PesInjectTerm(void);


/******************************************************
Function Name  :DRV_VideoPesInjectStart
Description       :����PES��Ƶ������
param[in]         :VidStartParams����Ч��������Ϊ�㼴��
param[in]         :VidStreamContent����Ƶ�����ʽ
*******************************************************/
DRV_ErrCode DRV_VideoPesInjectStart(STVID_StartParams_t VidStartParams,DRV_VideoType VidStreamContent);


/********************************************************
Function Name  :DRV_VideoPesInjectStop
Description       :�ر�PES��Ƶ������
param[in]         :DisableLastFrame����Ч��������Ϊ�㼴��
param[in]         :VidStreamContent����Ч��������Ϊ�㼴��
*********************************************************/
DRV_ErrCode DRV_VideoPesInjectStop(DRV_VideoType VidStreamContent, BOOL DisableLastFrame);


/*********************************************
Function Name  :DRV_VideoPesInjectPause
Description       :��ͣAV���ţ�����PAUSE״̬
param[in]         :VidStreamContent����Ƶ�����ʽ
**********************************************/
DRV_ErrCode DRV_VideoPesInjectPause(DRV_VideoType VidStreamContent);


/*********************************************
Function Name  :DRV_VideoPesInjectPause
Description       :�ָ�AV���ţ�����PLAY״̬
param[in]         :VidStreamContent����Ƶ�����ʽ
**********************************************/
DRV_ErrCode DRV_VideoPesInjectResume(DRV_VideoType VidStreamContent);


/*********************************************
Function Name  :DRV_VideoPesInjectCopy
Description       :COPYҪ���ŵ�PES����
param[in]         :consumer��ע���buffer����
param[in]         :SizeToCopy�����ݳ���
param[in]         :VidStreamContent����Ƶ�����ʽ
**********************************************/
DRV_ErrCode DRV_VideoPesInjectCopy(U8 *consumer,U32 SizeToCopy,DRV_VideoType VidStreamContent);


/****************************************************************
Function Name  :DRV_VideoPesGetPesBufferFreeSize��DRV_VideoPesInjectGetFreeSize
Description       :��ȡ������ʣ������ݴ�С
param[in]         :FreeSize�����ݴ�С
param[in]         :VidStreamContent����Ƶ�����ʽ
****************************************************************/
DRV_ErrCode DRV_VideoPesGetPesBufferFreeSize(U32 *FreeSize,DRV_VideoType VidStreamContent);
DRV_ErrCode DRV_VideoPesInjectGetFreeSize(U32 *FreeSize,DRV_VideoType VidStreamContent);


/*********************************************
Function Name  :DRV_VideoPesGetPesBufferSize
Description       :��ȡ�����������ݴ�С
param[in]         :BufferSize�����ݴ�С
param[in]         :VidStreamContent����Ƶ�����ʽ
**********************************************/
DRV_ErrCode DRV_VideoPesGetPesBufferSize(U32 *BufferSize,DRV_VideoType VidStreamContent);


/******************************************************
Function Name  :DRV_VideoPesGetPesBufferSize,DRV_VideoFlushBitBuffer
Description       :��λAVPLAY buffer
param[in]         :VidStreamContent����Ƶ�����ʽ
*******************************************************/
DRV_ErrCode DRV_VideoPesFlushBuffer(DRV_VideoType VidStreamContent);
DRV_ErrCode DRV_VideoFlushBitBuffer(DRV_VideoType VidStreamContent);


/*********************************************
Function Name  :DRV_VideoPesGetPesBufferSize
Description       :��ȡ����������
param[in]         :Rate_p������
param[in]         :VidStreamContent����Ƶ�����ʽ
**********************************************/
DRV_ErrCode	DRV_VideoPesInjectBufferRate(U32 *Rate_p,DRV_VideoType VidStreamContent);


/*********************************************************
Function Name  :DRV_VideoPesSetSpeed
Description       :���ò��ŵ��ٶ�
param[in]         :speed�������ٶȣ�Ŀǰֻ֧��2�����
**********************************************************/
DRV_ErrCode DRV_VideoPesSetSpeed(int speed);


/**********************************************************
Function Name  :DRV_AudioPesInjectStart
Description       :����PES��Ƶ������
param[in]         :AudStartParams����Ч��������Ϊ�㼴��
param[in]         :AudPCMInputParams����Ч��������Ϊ�㼴��
param[in]         :AudStreamContent����Ƶ�����ʽ
***********************************************************/
DRV_ErrCode DRV_AudioPesInjectStart(STAUD_StartParams_t AudStartParams,STAUD_PCMInputParams_t AudPCMInputParams,DRV_AudioType AudStreamContent);


/*********************************************
Function Name  :DRV_AudioPesInjectStop
Description       :�ر�PES��Ƶ������
**********************************************/
DRV_ErrCode DRV_AudioPesInjectStop(void);


/*********************************************
Function Name  :DRV_AudioPesInjectPause
Description       :��ͣAV������
**********************************************/
DRV_ErrCode DRV_AudioPesInjectPause(void);


/*********************************************
Function Name  :DRV_AudioPesInjectResume
Description       :�ָ�AV������
**********************************************/
DRV_ErrCode DRV_AudioPesInjectResume(void);


/*********************************************
Function Name  :DRV_AudioPesInjectCopy
Description       :COPYҪ���ŵ�PES����
param[in]         :consumer��ע���buffer����
param[in]         :SizeToCopy�����ݳ���
**********************************************/
DRV_ErrCode DRV_AudioPesInjectCopy(U8 *consumer,U32 SizeToCopy);


/*********************************************************
Function Name  :DRV_AudioPesFlushBuffer,DRV_AudioFlushBitBuffer
Description       :��λAVPLAY buffer
param[in]         :AudStartParams����Ч��������Ϊ�㼴��
param[in]         :AudStreamContent����Ч��������Ϊ�㼴��
**********************************************************/
DRV_ErrCode DRV_AudioPesFlushBuffer(void);
DRV_ErrCode DRV_AudioFlushBitBuffer(STAUD_StartParams_t AudStartParams,DRV_AudioType AudStreamContent);


/*****************************************************************
Function Name  :DRV_AudioPesGetPesBufferFreeSize��DRV_AudioPesInjectGetFreeSize
Description       :��ȡ������ʣ������ݴ�С
param[in]         :FreeSize�����ݴ�С
******************************************************************/
DRV_ErrCode DRV_AudioPesGetPesBufferFreeSize(U32 *FreeSize);
DRV_ErrCode DRV_AudioPesInjectGetFreeSize(U32 *FreeSize);


/*********************************************
Function Name  :DRV_AudioPesInjectBufferRate
Description       :��ȡ����������
param[in]         :Rate_p������
**********************************************/
DRV_ErrCode	DRV_AudioPesInjectBufferRate(U32 *Rate_p);


/**********************************************************
Function Name  :DRV_AudioPesSetSpeed
Description       :���ò��ŵ��ٶ�
param[in]         :speed�������ٶȣ�Ŀǰֻ֧��2�����
***********************************************************/
DRV_ErrCode DRV_AudioPesSetSpeed(int speed);

/* C++ support */
/* ----------- */
#ifdef __cplusplus
}
#endif
#endif

