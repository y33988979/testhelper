/******************************************************************************

  Copyright (C), 2001-2011, HiSilicon Technologies Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_avplay.h
  Version       : Initial draft
  Author        : HiSilicon multimedia software group
  Created Date  : 2009-12-21
  Description   :
  Change History:
  Date          : 2009-12-21
  Author        : Wan Li (employee ID: 58735)
  Description   : This file is created.

*******************************************************************************/
/** 
 * \file
 * \brief Describes the information about the audio/video player (AVPLAY) module. CNcomment: �ṩAVPLAY�������Ϣ
 */
#ifndef __HI_UNF_AVPLAY_H__
#define __HI_UNF_AVPLAY_H__

#include "hi_unf_common.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

/*********************************error  macro******************************************/
/*************************** Structure Definition ****************************/
/** \addtogroup      AVPLAY */
/** @{ */  /** <!-- ��AVPLAY�� */

/**Highest priority of the video decoder*/
/**CNcomment:��Ƶ��������������ȼ�*/
#define HI_UNF_VCODEC_MAX_PRIORITY  16


/**Defines the buffer ID required in an AVPLAY.*/
/**CNcomment:����AV����������Ҫ��BufferIDö������ */
typedef enum hiUNF_AVPLAY_BUFID_E
{
    HI_UNF_AVPLAY_BUF_ID_ES_VID  = 0,    /**<ID of the buffer for storing the video elementary streams (ESs) played by an AVPLAY*//**<CNcomment: AV������ES��ƵBuffer ID*/
    HI_UNF_AVPLAY_BUF_ID_ES_AUD  = 1,    /**<ID of the buffer for storing the audio ESs played by an AVPLAY*//**<CNcomment: AV������ES��ƵBuffer ID*/
    HI_UNF_AVPLAY_BUF_ID_BUTT
} HI_UNF_AVPLAY_BUFID_E;

/********************************************************************************/
/**Defines the type of a media channel.*/
/**CNcomment:����ý��ͨ������ */
typedef enum hiUNF_AVPLAY_MEDIA_CHAN
{
    HI_UNF_AVPLAY_MEDIA_CHAN_AUD  = 0x01,  /**<Audio channel*//**<CNcomment: ��Ƶͨ�� */
    HI_UNF_AVPLAY_MEDIA_CHAN_VID  = 0x02,  /**<Video channel*//**<CNcomment: ��Ƶͨ�� */

    HI_UNF_AVPLAY_MEDIA_CHAN_BUTT = 0x8
} HI_UNF_AVPLAY_MEDIA_CHAN_E;


/**Defines the type of an input stream interface.*/
/**CNcomment:���������������ӿ����� */
typedef enum hiUNF_AVPLAY_STREAM_TYPE_E
{
    HI_UNF_AVPLAY_STREAM_TYPE_TS = 0,   /**<Transport stream (TS)*/
    HI_UNF_AVPLAY_STREAM_TYPE_ES,       /**<ES stream*/
    HI_UNF_AVPLAY_STREAM_TYPE_VP,       /**<VP stream*/

    HI_UNF_AVPLAY_STREAM_TYPE_BUTT
} HI_UNF_AVPLAY_STREAM_TYPE_E;

/********************************************************************************/

/**Defines the stream attributes.*/
/**CNcomment:�����������ԵĽṹ�� */
typedef struct hiUNF_AVPLAY_STREAM_ATTR_S
{
    HI_UNF_AVPLAY_STREAM_TYPE_E enStreamType;   /**<Stream type*//**<CNcomment:�������� */

    HI_U32                      u32VidBufSize;  /**<Video buffer size*//**<CNcomment: ��Ƶ�����С */
    HI_U32                      u32AudBufSize;  /**<Audio buffer size*//**<CNcomment: ��Ƶ�����С */

	HI_UNF_VP_NET_PARA_S        stNetCfgPara;	/**<Network parameter. It is valid when the stream type is VP.*//**<CNcomment: �����������������ΪVPʱ��Ч */

} HI_UNF_AVPLAY_STREAM_ATTR_S;

/**Supported synchronization control mode*/
/**CNcomment:֧�ֵ�ͬ������ģʽ */
typedef enum hiUNF_SYNC_REF_E
{
    HI_UNF_SYNC_REF_NONE = 0, /**<Free playing without synchronization*//**<CNcomment: ���ɲ���*/
    HI_UNF_SYNC_REF_AUDIO,    /**<Audio-based synchronization*//**<CNcomment: ����ƵΪ׼  */
    HI_UNF_SYNC_REF_VIDEO,    /**<Video-based synchronization*//**<CNcomment: ����ƵΪ׼  */
    HI_UNF_SYNC_REF_PCR,      /**<Program clock reference (PCR)-based synchronization*//**<CNcomment: ��PCR��Program Clock Reference��Ϊ׼ */
    HI_UNF_SYNC_REF_SCR,      /**<Program clock reference (SCR)-based synchronization*//**<CNcomment: ��SCR(System Clock Reference) Ϊ׼ */

    HI_UNF_AVPLAY_SYNC_REF_BUTT
} HI_UNF_SYNC_REF_E;

/**Defines the status of a buffer.*/
/**CNcomment:����ʹ�õĻ�����״̬ö������ */
typedef enum hiUNF_AVPLAY_BUF_STATE_E
{
    HI_UNF_AVPLAY_BUF_STATE_EMPTY = 0,   /**<The buffer is idle.*//**<CNcomment: ����������*/
    HI_UNF_AVPLAY_BUF_STATE_LOW,         /**<The buffer usage is too low.*//**<CNcomment: ������ռ���ʹ���*/
    HI_UNF_AVPLAY_BUF_STATE_NORMAL,      /**<The buffer works normally.*//**<CNcomment: ������ʹ������*/
    HI_UNF_AVPLAY_BUF_STATE_HIGH,        /**<The buffer usage is too high.*//**<CNcomment: ������ռ���ʹ���*/
    HI_UNF_AVPLAY_BUF_STATE_FULL         /**<The buffer is full.*//**<CNcomment: ����������*/
}HI_UNF_AVPLAY_BUF_STATE_E;

/**Type of the event callback function*/
/**CNcomment: �¼��ص��������� */
typedef enum hiUNF_AVPLAY_EVENT_E
{
    HI_UNF_AVPLAY_EVENT_EOS,                   /**<The end of stream (EOS) operation is performed, NULL*//**<CNcomment: EOSִ�н���, NULL */
    HI_UNF_AVPLAY_EVENT_STOP,                  /**<The stop operation is performed, NULL*//**<CNcomment: STOPִ�н���, NULL   */
    HI_UNF_AVPLAY_EVENT_RNG_BUF_STATE,         /**<Status change of the media buffer queue, HI_UNF_AVPLAY_EVENT_RNG_BUF_STATE*//**<CNcomment: ý�建�����״̬�仯, HI_UNF_AVPLAY_EVENT_RNG_BUF_STATE */
    HI_UNF_AVPLAY_EVENT_NORM_SWITCH,           /**<Standard switch, HI_UNF_ENC_FMT_E*//**<CNcomment: ��ʽ�л�, HI_UNF_ENC_FMT_E        */
    HI_UNF_AVPLAY_EVENT_FRAMEPACKING_CHANGE,   /**<3D Frame packing change,HI_UNF_VIDEO_FRAME_PACKING_TYPE_E*//**<CNcomment: 3D֡���ͱ仯, HI_UNF_VIDEO_FRAME_PACKING_TYPE_E        */
    //HI_UNF_AVPLAY_EVENT_NEW_SEQ,             /**<Information about the new sequence, HI_UNF_VIDEO_SEQUENCE_INFO_S*//**<CNcomment: ��������Ϣ, HI_UNF_VIDEO_SEQUENCE_INFO_S */
    HI_UNF_AVPLAY_EVENT_NEW_VID_FRAME,         /**<New video frame, HI_UNF_VO_FRAMEINFO_S*//**<CNcomment: ����Ƶ֡, HI_UNF_VO_FRAMEINFO_S */
    HI_UNF_AVPLAY_EVENT_NEW_AUD_FRAME,         /**<New audio frame, HI_UNF_AO_FRAMEINFO_S*//**<CNcomment: ����Ƶ֡, HI_UNF_AO_FRAMEINFO_S */
    HI_UNF_AVPLAY_EVENT_NEW_USER_DATA,         /**<New video user data, HI_UNF_VIDEO_USERDATA_S*//**<CNcomment: ����Ƶ�û�����, HI_UNF_VIDEO_USERDATA_S */
    HI_UNF_AVPLAY_EVENT_GET_AUD_ES,            /**<New audio ES data, HI_UNF_ES_BUF_S*//**<CNcomment: ����ƵES����, HI_UNF_ES_BUF_S */
    HI_UNF_AVPLAY_EVENT_IFRAME_ERR,      /**<I frame decode error*//**<CNcomment: ����I֡���� */
    HI_UNF_AVPLAY_EVENT_JUMP_STREAM,           /**<discard stream data while fast forward*//**<CNcomment: �����ʱ����Ҫ������������ */
    HI_UNF_AVPLAY_EVENT_SOUND_POOL,      /**< Reserve *//**<CNcomment: ����*/

    HI_UNF_AVPLAY_EVENT_BUTT
} HI_UNF_AVPLAY_EVENT_E;

/**Mode of processing the buffer overflow*/
/**CNcomment: ��������������� */
typedef enum hiUNF_AVPLAY_OVERFLOW_E
{
    HI_UNF_AVPLAY_OVERFLOW_RESET,              /**<Reset during overflow*//**<CNcomment: ���ʱ���и�λ  */
    HI_UNF_AVPLAY_OVERFLOW_DISCARD,            /**<Discard during overflow*//**<CNcomment: ���ʱ���ж���  */
    HI_UNF_AVPLAY_OVERFLOW_BUTT
} HI_UNF_AVPLAY_OVERFLOW_E;

/**Defines the type of the event callback function.*/
/**CNcomment: �����¼��ص�����ö������*/
typedef HI_S32 (*HI_UNF_AVPLAY_EVENT_CB_FN)(HI_HANDLE hAvplay, HI_UNF_AVPLAY_EVENT_E enEvent, HI_U32 u32Para);

/**Defines the attribute ID of an AVPLAY.*/
/**CNcomment: ����AV����������IDö������ */
typedef enum hiUNF_AVPLAY_ATTR_ID_E
{
    HI_UNF_AVPLAY_ATTR_ID_STREAM_MODE = 0,  /**<Playing mode, HI_UNF_AVPLAY_ATTR_S*//**<CNcomment: ����ģʽ , HI_UNF_AVPLAY_ATTR_S */

    HI_UNF_AVPLAY_ATTR_ID_ADEC,             /**<Audio attribute, HI_UNF_ACODEC_ATTR_S*//**<CNcomment: ��Ƶ���� , HI_UNF_ACODEC_ATTR_S */
    HI_UNF_AVPLAY_ATTR_ID_VDEC,             /**<Video attribute, HI_UNF_VCODEC_ATTR_S*//**<CNcomment: ��Ƶ���� , HI_UNF_VCODEC_ATTR_S  */

    HI_UNF_AVPLAY_ATTR_ID_VDEC_ADV,          /**<Advanced video attribute, HI_UNF_VCODEC_ADV_ATTR_S*//**<CNcomment: �߼���Ƶ���� , HI_UNF_VCODEC_ADV_ATTR_S  */

    HI_UNF_AVPLAY_ATTR_ID_AUD_PID,          /**<Audio packet identifier (PID), HI_U32*//**<CNcomment: ��ƵPID , HI_U32*/
    HI_UNF_AVPLAY_ATTR_ID_VID_PID,          /**<Video PID , HI_U32*//**<CNcomment: ��ƵPID , HI_U32*/
    HI_UNF_AVPLAY_ATTR_ID_PCR_PID,          /**<PCR PID, HI_U32*//**<CNcomment: PCR PID , HI_U32*/

    HI_UNF_AVPLAY_ATTR_ID_SYNC,             /**<Synchronization attribute, HI_UNF_SYNC_ATTR_S*//**<CNcomment: ͬ������ , HI_UNF_SYNC_ATTR_S */
    HI_UNF_AVPLAY_ATTR_ID_AFD,              /**<Whether to enable the active format descriptor (AFD), HI_BOOL*//**<CNcomment: AFD �Ƿ����� HI_BOOL */
    HI_UNF_AVPLAY_ATTR_ID_OVERFLOW,         /**<Overflow processing type, HI_UNF_AVPLAY_OVERFLOW_E*//**<CNcomment: ����������� , HI_UNF_AVPLAY_OVERFLOW_E */

    HI_UNF_AVPLAY_ATTR_ID_FRMRATE,          /**< FrameRate HI_UNF_VCODEC_FRMRATE_S */ /**<CNcomment: ֡�� , HI_UNF_VCODEC_FRMRATE_S */
    HI_UNF_AVPLAY_ATTR_ID_ADEC_VOL,         /**<Audio decode volume*//**<CNcomment: ADEC���� , HI_U32 */
    HI_UNF_AVPLAY_ATTR_ID_BUTT
} HI_UNF_AVPLAY_ATTR_ID_E;

/*******************************************************************************/

/**Defines the  synchronization region.*/
/**CNcomment: ��������Ƶͬ������ */
typedef struct hiUNF_SYNC_REGION_S
{
    HI_S32                s32VidPlusTime;        /**<Plus time range during video synchronization*//**<CNcomment: ��Ƶͬ����ǰ��ʱ�䷶Χ */
    HI_S32                s32VidNegativeTime;    /**<Negative time range during video synchronization*//**<CNcomment: ��Ƶͬ������ʱ�䷶Χ */
    HI_BOOL               bSmoothPlay;           /**<Slow playing enable*//**<CNcomment: ����ʹ�� */
} HI_UNF_SYNC_REGION_S;

/**Defines the audio/video synchronization attributes.*/
/**CNcomment: ��������Ƶͬ������ */
typedef struct hiUNF_SYNC_ATTR_S
{
    HI_UNF_SYNC_REF_E     enSyncRef;             /**<Audio-based synchronization, PCR-based synchronization, and free playing without synchronization*//**<CNcomment: ��ƵΪ׼��PCRΪ׼�����ɲ��ŵ� */
    HI_UNF_SYNC_REGION_S  stSyncStartRegion;     /**<Synchronization start region*//**<CNcomment: ͬ��������� */
    HI_UNF_SYNC_REGION_S  stSyncNovelRegion;     /**<Synchronization exception region*//**<CNcomment: ͬ���쳣���� */
    HI_S32                s32VidPtsAdjust;       /**<Video presentation time stamp (PTS) adjustment*//**<CNcomment: ��ƵPTS����*/
    HI_S32                s32AudPtsAdjust;       /**<Audio PTS adjustment*//**<CNcomment: ��ƵPTS����*/

    HI_U32                u32PreSyncTimeoutMs;   /**<Pre-synchronization timeout, in ms*//**<CNcomment: Ԥͬ���ĳ�ʱʱ�䣬��λΪ���� */
    HI_BOOL               bQuickOutput;          /**<Fast output enable*//**<CNcomment: �������ʹ�� */
} HI_UNF_SYNC_ATTR_S;

/**Defines the playing attributes of an AVPLAY.*/
/**CNcomment: ����AV�������� */
typedef struct hiUNF_AVPLAY_ATTR_S
{
    HI_U32                       u32DemuxId;   /**<ID of the DEMUX used by an AVPLAY, only for ts mode*//**<CNcomment: AVPLAY��ʹ�õ�DEMUX ID ������������ΪTSʱ��Ч*/
    HI_UNF_AVPLAY_STREAM_ATTR_S  stStreamAttr; /**<Stream attributes*//**<CNcomment: �������� */
} HI_UNF_AVPLAY_ATTR_S;

/********************************************************************************/
/**Defines the synchronization status when an AVPLAY is running.*/
/**CNcomment: ���岥��������״̬��Ϣ��ͬ��״̬��Ϣ���� */
typedef struct hiUNF_SYNC_STATUS_S
{
    HI_U32 u32FirstAudPts;    /**<PTS of the first audio frame*//**<CNcomment: ��һ����Ƶ֡ PTS */
    HI_U32 u32FirstVidPts;    /**<PTS of the first video frame*//**<CNcomment: ��һ����Ƶ֡ PTS */
    HI_U32 u32LastAudPts;     /**<PTS of the last audio frame*//**<CNcomment: ������ŵ�һ����Ƶ֡ PTS */
    HI_U32 u32LastVidPts;     /**<PTS of the last video frame*//**<CNcomment: ������ŵ�һ����Ƶ֡ PTS */
    HI_S32 s32DiffAvPlayTime; /**<Playing time difference between audio and video frames*//**<CNcomment: ����Ƶ ����ʱ�� */
    HI_U32 u32PlayTime;       /**<Playing time*//**<CNcomment: ��ǰ�Ѳ���ʱ�� */
    HI_U32 u32LocalTime;      /**<Local synchronization reference time*//**<CNcomment: ����ͬ���ο�ʱ�� */
} HI_UNF_SYNC_STATUS_S;

/**Defines the status of a media buffer.*/
/**CNcomment:����ý�建������״̬��Ϣ */
typedef struct hiUNF_AVPLAY_BUF_STATUS_S
{
    HI_U32 u32BufId;         /**<Media buffer ID*//**<CNcomment: ý�建�� ��ʶ */
    HI_U32 u32BufSize;       /**<Media buffer size*//**<CNcomment: ý�建���С */
    HI_U32 u32BufRptr;       /**<Read pointer of the media buffer. This pointer is valid when TSs are being played.*//**<CNcomment: ý�建���ָ��,Ts����ʱ��Ч*/
    HI_U32 u32BufWptr;       /**<Write pointer of the media buffer. This pointer is valid when TSs are being played.*//**<CNcomment: ý�建��дָ��,Ts����ʱ��Ч*/
    HI_U32 u32UsedSize;      /**<Used size of the media buffer*//**<CNcomment: ý�建����ʹ�ô�С */
    HI_U32 u32FrameBufTime;  /**<Frame buffer time*//**<CNcomment: ֡����ʱ�� */
    HI_U32 u32FrameBufNum;   /**<The number of frames in frame buffer*//**<CNcomment: ֡������Ŀ ��VIDEO��Ч*/
    HI_BOOL bEndOfStream;    /**<Flag to indicate end of stream*//**<CNcomment: ����������������ϱ�ʶ ��VIDEO��Ч*/
} HI_UNF_AVPLAY_BUF_STATUS_S;

/**Defines the playing status of an AVPLAY.*/
/**CNcomment:����AV�Ĳ���״̬ */
typedef enum hiUNF_AVPLAY_STATUS_E
{
    HI_UNF_AVPLAY_STATUS_STOP = 0, /**<Stop*//**<CNcomment: ֹͣ */
    HI_UNF_AVPLAY_STATUS_PLAY,     /**<Play*//**<CNcomment: ���� */
    HI_UNF_AVPLAY_STATUS_TPLAY,    /**<Trick play, such as fast forward and rewind*//**<CNcomment: TPlay, ������� */
    HI_UNF_AVPLAY_STATUS_PAUSE,    /**<Pause*//**<CNcomment: ��ͣ */
    HI_UNF_AVPLAY_STATUS_EOS,      /**<EOS*//**<CNcomment: �������Ž��� */
    HI_UNF_AVPLAY_STATUS_SEEK ,    /**<Seek play*//**<CNcomment: ��λ���� */

    HI_UNF_AVPLAY_STATUS_BUTT
}HI_UNF_AVPLAY_STATUS_E;

/**Defines the output value after the playing status of an AVPLAY is queried.*/
/**CNcomment:����AV����״̬��ѯ���ֵ���� */
typedef struct hiUNF_AVPLAY_STATUS_INFO_S
{
    HI_UNF_SYNC_STATUS_S       stSyncStatus;      /**<Synchronization status*//**<CNcomment: ͬ��״̬ */
    HI_UNF_AVPLAY_STATUS_E     enRunStatus;       /**<Running status*//**< CNcomment:����״̬ */
    HI_U32                     u32VidFrameCount;  /**<Count of played video frames*//**<CNcomment: ��Ƶ�Ѳ���֡�� */
    HI_U32                     u32AuddFrameCount; /**<Count of played audio frames*//**<CNcomment: ��Ƶ�Ѳ���֡�� */
    HI_UNF_AVPLAY_BUF_STATUS_S stBufStatus[HI_UNF_AVPLAY_BUF_ID_BUTT]; /**<Status of the media buffer*//**<CNcomment:ý�建��״̬ */
    HI_U32                     u32VidErrorFrameCount; /**<Number of error frames during video decoding*/ /**<CNcomment: ��Ƶ�������֡�� */    
} HI_UNF_AVPLAY_STATUS_INFO_S;

/********************************************************************************/
/**Defines the information about the playing program streams to be queried.*/
/**CNcomment: ���岥�Ž�Ŀ����ѯ��Ϣ����*/
typedef struct hiUNF_AVPLAY_STREAM_INFO_S
{
    HI_UNF_VCODEC_STREAMINFO_S stVidStreamInfo; /**<Video stream information*//**<CNcomment:��Ƶ����Ϣ */
    HI_UNF_ACODEC_STREAMINFO_S stAudStreamInfo; /**<Audio stream information*//**<CNcomment:��Ƶ����Ϣ */
} HI_UNF_AVPLAY_STREAM_INFO_S;

/********************************************************************************/
/**Defines the information about an I frame.*//**CNcomment:����I֡������Ϣ���� */
typedef struct hiUNF_AVPLAY_I_FRAME_S
{
    HI_U8  *             pu8Addr;    /**<User-state virtual address of a frame*//**<CNcomment:֡�����û�̬�����ַ*/
    HI_U32               u32BufSize; /**<Frame size, in byte*//**<CNcomment:֡���ݴ�С����λ�ֽ�*/
    HI_UNF_VCODEC_TYPE_E enType;     /**<Protocol type of a data segment*//**<CNcomment:��Ƭ���ݵ�Э������*/
} HI_UNF_AVPLAY_I_FRAME_S;

/********************************************************************************/
/**Defines the decoder type. The occupied memory varies according to decoders.*/
/**CNcomment:������������� ��ͬ���͵Ľ�����ռ���ڴ治ͬ*/
typedef enum hiHI_UNF_VCODEC_DEC_TYPE_E
{
    HI_UNF_VCODEC_DEC_TYPE_NORMAL,            /**<Normal type.*//**<CNcomment:��ͨ����*/

    /**<I frame decoding type. If an AVPLAY is used to decode I frames only (HI_UNF_AVPLAY_DecodeIFrame), you can select this type to reduce the memory usage.*/
    /**<CNcomment:I֡�������� ���avplay������I֡����(HI_UNF_AVPLAY_DecodeIFrame) ����Ϊ�����Ϳ��Խ�ʡ�ڴ�*/    
    HI_UNF_VCODEC_DEC_TYPE_ISINGLE,  
    
    HI_UNF_VCODEC_DEC_TYPE_BUTT               
}HI_UNF_VCODEC_DEC_TYPE_E;

/**Defines the level of the protocol supported by the decoder. This value affects the number of frame buffers allocated by the normal decoder. 
The greater the value, the more the required frame buffers.*/
/**CNcomment:���������֧�ֵ�Э�鼶�� Ӱ��NORMAL���ͽ����������֡����� ֵԽ����Ҫ��֡����ĿԽ��*/
typedef enum hiHI_UNF_VCODEC_PRTCL_LEVEL_E
{
    HI_UNF_VCODEC_PRTCL_LEVEL_MPEG = 0,   /**<Protocols excluding the H.264 protocol*//**<CNcomment:��h264�������Э�� */
    HI_UNF_VCODEC_PRTCL_LEVEL_H264 = 1,  /**<H.264 protocol and other protocols*//**<CNcomment:h264Э�� */    
    HI_UNF_VCODEC_PRTCL_LEVEL_BUTT
}HI_UNF_VCODEC_PRTCL_LEVEL_E;

/**Defines the attributes when an AVPLAY enables the video decoder. The settings affect the memory occupied by the video decoder and decoding performance.*/
/**CNcomment:����AV����������Ƶ������ʱ�������ýṹ�� Ӱ����Ƶ������ռ���ڴ��С����������*/
typedef struct hiHI_UNF_AVPLAY_OPEN_OPT_S
{
    HI_UNF_VCODEC_DEC_TYPE_E    enDecType;       /**<Maximum resolution supported by the decoder. This value affects the size of each frame buffer.*//**<CNcomment:Decoder type*//**<����������*/   
    HI_UNF_VCODEC_CAP_LEVEL_E   enCapLevel;      /**<Maximum resolution supported by the decoder. This value affects the size of each frame buffer.*//**<CNcomment:������֧�ֵ����ֱ��� Ӱ��ÿ��֡��Ĵ�С*/
    HI_UNF_VCODEC_PRTCL_LEVEL_E enProtocolLevel; /**<Supported protocol level. This value affects the number of frame buffers.*//**<CNcomment:֧�ֵ�Э�鼶�� Ӱ��֡����Ŀ*/
}HI_UNF_AVPLAY_OPEN_OPT_S;

/**Defines the video display mode after an AVPLAY is stopped.*/
/**CNcomment:����AV������ֹͣʱ��Ƶ��ʾģʽ */
typedef enum hiUNF_AVPLAY_STOP_MODE_E
{
    HI_UNF_AVPLAY_STOP_MODE_STILL = 0,  /**<The last frame is still after an AVPLAY is stopped.*//**<CNcomment:stop�������һ֡*/
    HI_UNF_AVPLAY_STOP_MODE_BLACK = 1,  /**<The blank screen is displayed after an AVPLAY is stopped.*//**<CNcomment:stop�����*/
    HI_UNF_AVPLAY_STOP_MODE_BUTT
} HI_UNF_AVPLAY_STOP_MODE_E;

/**Defines the attributes when an AVPLAY is started.*/
/**CNcomment:����AV����������ʱ�������ýṹ��*/
typedef struct hiAVPLAY_START_OPT_S
{
    HI_U32       u32Reserved;
} HI_UNF_AVPLAY_START_OPT_S;

/**Defines the attributes when an AVPLAY is stopped.*/
/**CNcomment:����AV������ֹͣʱ�������ýṹ��*/
typedef struct hiAVPLAY_STOP_OPT_S
{
    /**
         s32Timeout: end of stream timeout
         s32Timeout = 0   Wait until streams are played in non-block mode, that is, the interface is returned immediately. CNcomment:�������ȴ��������Ž�������������
         s32Timeout > 0   Block timeout, in ms, CNcomment:������ʱʱ�䣬��λΪ����
         s32Timeout = -1  Infinite wait,CNcomment:���޵ȴ�
     */
    HI_U32                    u32TimeoutMs;    /**<Timeout, in ms*//**<CNcomment:��ʱֵ����λΪ����*/
    HI_UNF_AVPLAY_STOP_MODE_E enMode;          /**<Video display mode*//**<CNcomment:��Ƶ��ʾģʽ*/
} HI_UNF_AVPLAY_STOP_OPT_S;

/**Defines the attributes when an AVPLAY is paused.*/
/**CNcomment:����AV��������ͣʱ�������ýṹ��*/
typedef struct hiAVPLAY_PAUSE_OPT_S
{
    HI_U32       u32Reserved;
} HI_UNF_AVPLAY_PAUSE_OPT_S;

/**Defines the attributes when the playing mode of an AVPLAY is fast forward.*/
/**CNcomment:����AV���������ʱ�������ýṹ��*/
typedef struct hiAVPLAY_TPLAY_OPT_S
{
    HI_U32       u32TplaySpeed;        /**<The speed when the playing mode of an AVPLAY is fast forward.*//**<CNcomment:����ٶ�*/
} HI_UNF_AVPLAY_TPLAY_OPT_S;

/**Defines the attributes when an AVPLAY is resumed.*/
/**CNcomment:����AV�������ָ�ʱ�������ýṹ��*/
typedef struct hiAVPLAY_RESUME_OPT_S
{
    HI_U32       u32Reserved;
} HI_UNF_AVPLAY_RESUME_OPT_S;

/**Defines the attributes when an AVPLAY is reset.*/
/**CNcomment:����AV��������λʱ�������ýṹ��*/
typedef struct hiAVPLAY_RESET_OPT_S
{
    HI_U32       u32Reserved;
} HI_UNF_AVPLAY_RESET_OPT_S;

/** @} */  /** <!-- ==== Structure Definition End ==== */

/******************************* API Declaration *****************************/
/** \addtogroup      AVPLAY */
/** @{ */  /** <!-- ��AVPLAY�� */

/**
\brief Initializes the AVPLAY module.CNcomment:��ʼ��AVPLAYģ��
\attention \n
Before calling ::HI_UNF_AVPLAY_Create to create an AVPLAY, you must call this application programming interface (API).
CNcomment �ڵ���::HI_UNF_AVPLAY_Create�ӿڴ���������֮ǰ�����ñ��ӿ�
\param  N/A
\retval ::HI_SUCCESS Success CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NOT_EXIST There is no AVPLAY. CNcomment:AVPLAY�豸������
\retval ::HI_ERR_AVPLAY_NOT_DEV_FILE  The file is not an AVPLAY file. CNcomment:AVPLAY���豸
\retval ::HI_ERR_AVPLAY_DEV_OPEN_ERR  An AVPLAY fails to be started. CNcomment:AVPLAY��ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_Init(HI_VOID);

/**
\brief Deinitializes the AVPLAY module. CNcomment:ȥ��ʼ��AVPLAYģ��
\attention \n
Before calling ::HI_UNF_AVPLAY_Destroy to destroy all players, you must call this API.
CNcomment: �ڵ���::HI_UNF_AVPLAY_Destroy�ӿ��������еĲ������󣬵��ñ��ӿ�
\param N/A
\retval ::HI_SUCCESS Success CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_INVALID_OPT  The operation is invalid.  CNcomment:�����Ƿ�
\retval ::HI_ERR_AVPLAY_DEV_CLOSE_ERR  An AVPLAY fails to be stopped. CNcomment:AVPLAY�ر�ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_DeInit(HI_VOID);

/**
\brief Obtains the default configuration of an AVPLAY. CNcomment:��ȡȱʡ��AV��������
\attention \n
When calling this API to set the enCfg parameter, you must enter correct mode of the player to be created.\n
It is recommended that you call this API to obtain the default AVPLAY attributes before creating an AVPLAY. This avoids creation failures due to incomplete or incorrect parameters.
CNcomment:���ñ��ӿ�����enCfg����ʱ������ȷ������Ҫ����������ģʽ\n
����AV������ǰ������ñ��ӿڣ���ȡ��AV������Ĭ�����ԣ����ⴴ��AV������ʱ���ڲ�����ȫ����������²������������ɹ�����
\param[out] pstAvAttr  Pointer to AVPLAY attributes. For details, see the description of ::HI_UNF_AVPLAY_ATTR_S. CNcomment: ָ�����ͣ�AV�������ԣ���μ�::HI_UNF_AVPLAY_ATTR_S
\param[in] enCfg       AVPLAY type. For details, see the description of ::HI_UNF_AVPLAY_STREAM_TYPE_E. CNcomment: AV���ŵ����ͣ���μ�::HI_UNF_AVPLAY_STREAM_TYPE_E
\retval ::HI_SUCCESS Success CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_GetDefaultConfig(HI_UNF_AVPLAY_ATTR_S *pstAvAttr, HI_UNF_AVPLAY_STREAM_TYPE_E enCfg);

/**
\brief Registers a dynamic audio decoding library. CNcomment:ע����Ƶ��̬�����
\attention \n
\param[in] pFileName Name of the file in the audio decoding library CNcomment:��Ƶ������ļ���
\retval ::HI_SUCCESS  Success CNcomment:�ɹ�
\retval ::HI_FAILURE Failure CNcomment:ʧ��
\retval ::HI_ERR_AVPLAY_NULL_PTR  The input pointer is null. CNcomment:����ָ��Ϊ��

\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_RegisterAcodecLib(const HI_CHAR *pFileName);

/**
\brief Searches for registered dynamic audio decoding libraries. CNcomment:����ע����Ƶ��̬�����
\attention \n
\param[in] enDstCodecID Codec ID of an audio decoding library  CNcomment:��Ƶ�����CodecID
\retval ::HI_SUCCESS Success CNcomment:�ɹ�
\retval ::HI_FAILURE Failure CNcomment:ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_FoundRegisterDeoder(const HI_U32 enDstCodecID);

/**
\brief Searches for registered dynamic audio decoding libraries based on the audio format. 
CNcomment:������Ƶ��ʽ, ����ע����Ƶ��̬�����
\attention \n
\param[in] enFormat Audio format CNcomment:��Ƶ��ʽ
\param[out] penDstCodecID If an audio decoding library is found, its codec ID is returned. 
CNcomment:�ɹ��򷵻���Ƶ�����CodecID
\retval ::HI_SUCCESS Success CNcomment:�ɹ�
\retval ::HI_FAILURE Failure CNcomment:ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_FoundSupportDeoder(const HA_FORMAT_E enFormat,HI_U32 * penDstCodecID);


/**
\brief Sets private commands for a dynamic audio decoding library. These commands are used to call ha_codec. 
CNcomment:����˽���������Ƶ��̬�����, ����ha_codec ���� 
HI_HA_ERRORTYPE_E (*DecSetConfig)(HI_VOID * hDecoder, HI_VOID * pstConfigStructure);
\attention \n
\param[in] hAvplay         AVPLAY handle  CNcomment:AV���ž��
\param[in] pPara  Attribute structure CNcomment:���Խṹ
\retval ::HI_SUCCESS Success CNcomment:�ɹ�
\retval ::HI_FAILURE Failure CNcomment:ʧ��
\retval ::HI_ERR_AVPLAY_NULL_PTR  The input pointer is null. CNcomment:����ָ��Ϊ��
\see \n
N/A
*/

HI_S32 HI_UNF_AVPLAY_ConfigAcodec(const HI_U32 enDstCodecID, HI_VOID *pPara);

//HI_S32 HI_UNF_AVPLAY_ControlAcodec( HI_HANDLE hAvplay, HI_VOID *pPara);

/**
\brief Creates an AVPLAY. CNcomment:����AV������
\attention \n
Note the following point when setting the input parameter pstAttr: The stream source can be HI_UNF_AVPLAY_STREAM_TYPE_ES (ESs input from the memory) or HI_UNF_AVPLAY_STREAM_TYPE_TS (ESs input from the memory and TSs input from the Tuner).
CNcomment:�������Բ���pstAttr���м�����Ҫע�⣺����Դ֧��HI_UNF_AVPLAY_STREAM_TYPE_ES���ڴ�����ES������HI_UNF_AVPLAY_STREAM_TYPE_TS���ڴ�����TS����TUNER����TS ����
\param[in]  pstAvAttr   Pointer to AVPLAY attributes. For details, see the description of ::HI_UNF_AVPLAY_ATTR_S. ָ�����ͣ�AV�������ԣ���μ�::HI_UNF_AVPLAY_ATTR_S
\param[out] phAvplay    Pointer to the handle of a created AVPLAY.CNcomment:ָ�����ͣ�������AV���ž��
\retval ::HI_SUCCESS  Success CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_AVPLAY_CREATE_ERR       The AVPLAY fails to be created. CNcomment:AVPLAY����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_Create(const HI_UNF_AVPLAY_ATTR_S *pstAvAttr, HI_HANDLE *phAvplay);

/**
\brief Destroys an AVPLAY. CNcomment:����AV������
\attention \n
N/A
\param[in] hAvplay  AVPLAY handle CNcomment:AV���ž��
\retval ::HI_SUCCESS Success CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_AVPLAY_INVALID_OPT      The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_Destroy(HI_HANDLE hAvplay);

/**
\brief Sets the attributes of an AVPLAY. CNcomment:����AV��������
\attention \n
You can set the audio/video PID, audio/video decoding type, and synchronization mode by calling this API.\n
Different attribute IDs correspond to different data types. For details, see the Note part of HI_UNF_AVPLAY_ATTR_ID_E. The attribute types of the pPara and enAttrID parameters must be the same.\n
Before setting HI_UNF_AVPLAY_ATTR_ID_ADEC (audio decoding attribute) and HI_UNF_AVPLAY_ATTR_ID_VDEC (video decoding attribute),\n
you must disable the audio channel or video channel. The new attributes take effect when you enable the audio channel or video channel again.
CNcomment:���ñ��ӿڿ�ʵ����������ƵPID����������Ƶ�������͡�����ͬ����ʽ�ȹ���\n
��ͬ������ID��Ӧ�Ľṹ����μ��ṹ��HI_UNF_AVPLAY_ATTR_ID_E��"ע��"��pPara����Ҫ��enAttrID��Ӧ�����Խṹ�����ͱ���һ��\n
����Ҫ����HI_UNF_AVPLAY_ATTR_ID_ADEC����Ƶ�������ԣ���HI_UNF_AVPLAY_ATTR_ID_VDEC����Ƶ�������ԣ�ʱ\n
��Ҫ�ȹر���Ƶ����Ƶͨ���������������ԣ�Ȼ�������´���Ƶ����Ƶͨ�������Բſ�����Ч��
\param[in] hAvplay         AVPLAY handle CNcomment:AV���ž��
\param[in] enAttrID        Attribute ID CNcomment:����ID
\param[in] pPara  Data type corresponding to an attribute ID CNcomment:����ID��Ӧ�ṹ
\retval ::HI_SUCCESS Success CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_AVPLAY_INVALID_OPT      The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_SetAttr(HI_HANDLE hAvplay, HI_UNF_AVPLAY_ATTR_ID_E enAttrID, HI_VOID *pPara);

/**
\brief Obtains the attributes of an AVPLAY. CNcomment:��ȡAV��������
\attention \n
N/A
\param[in] hAvplay          AVPLAY handle AV���ž��
\param[in] enAttrID         Attribute ID CNcomment:����ID
\param[in] pPara   Data type corresponding to an attribute ID, CNcomment:����ID��Ӧ�ṹ
\retval ::HI_SUCCESS Success CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_AVPLAY_INVALID_OPT      The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_GetAttr(HI_HANDLE hAvplay, HI_UNF_AVPLAY_ATTR_ID_E enAttrID, HI_VOID *pPara);

/**
\brief Decodes I frames. CNcomment:����I֡����
\attention \n
If pstCapPicture is null, the decoded I frames are displayed in the window; if pstCapPicture is not null, the information about I frames is reported.\n
You must stop the video decoder before calling this API. If I frames are being processed, do not enable the video decoder.
CNcomment:��pstCapPictureΪ��ָ��ʱ�������I֡����window����ʾ������ǿգ��򲻻���ʾ���ǽ�I֡��Ϣ�ϱ���\n
���øýӿ�ǰ����ֹͣ��Ƶ��������I֡����û�����ʱ��Ҳ����������Ƶ������
\param[in] hAvplay         AVPLAY handle CNcomment:AV���ž��
\param[in] pstFrame        Pointer to frame attributes CNcomment:ָ�����ͣ�֡��������
\param[in] pstCapPicture   Pointer to the frame information CNcomment:ָ�����ͣ�֡��Ϣ
\retval ::HI_SUCCESS Success CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_AVPLAY_INVALID_OPT      The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_DecodeIFrame(HI_HANDLE hAvplay, const HI_UNF_AVPLAY_I_FRAME_S *pstFrame, HI_UNF_CAPTURE_PICTURE_S *pstCapPicture);

/**
\brief Sets the mode of a video decoder. CNcomment:������Ƶ��������ģʽ
\attention \n
This API is used in trick play mode. Before switching the mode to the trick play mode, you must enable a decoder to decode only I frames by calling this API.\n
Before switching the mode to the normal mode, you also need to set the mode of a decoder to normal by calling this API.
CNcomment:���ӿ���ҪӦ���ڱ��ٲ��ŵĳ��������л������ٲ���ǰ���ȵ��ñ��ӿڽ�����������Ϊֻ��I֡��\n
���л�����������ǰ���ȵ��ñ��ӿڽ�����������ΪNORMAL��
\param[in] hAvplay            AVPLAY handle CNcomment:AV���ž��
\param[in] enDecodeMode       Decoding mode CNcomment:����ģʽ
\retval ::HI_SUCCESS Success CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_AVPLAY_INVALID_OPT      The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_SetDecodeMode(HI_HANDLE hAvplay, HI_UNF_VCODEC_MODE_E enDecodeMode);

/**
\brief Registers an event. CNcomment:ע���¼�
\attention \n
N/A
\param[in] hAvplay     AVPLAY handle CNcomment:AV���ž��
\param[in] enEvent     Event type enumeration CNcomment:ö�����ͣ���ʾ�¼�����
\param[in] pfnEventCB  Pointer to the callback function corresponding to the registered event. CNcomment:�ص�����ָ�룬ָ����ע���¼���Ӧ�Ļص�����
\retval ::HI_SUCCESS Success CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT     The AVPLAY is not initialized.  CNcomment:AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_AVPLAY_INVALID_OPT      The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_RegisterEvent(HI_HANDLE      hAvplay,
                                   HI_UNF_AVPLAY_EVENT_E     enEvent,
                                   HI_UNF_AVPLAY_EVENT_CB_FN pfnEventCB);


/**
\brief Deregisters an event. CNcomment:ȡ��ע���¼�
\attention \n
N/A
\param[in] hAvplay   AVPLAY handle CNcomment:AV���ž��
\param[in] enEvent   Event type enumeration CNcomment:ö�����ͣ���ʾ�¼�����
\retval ::HI_SUCCESS Success CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_AVPLAY_INVALID_OPT      The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_UnRegisterEvent(HI_HANDLE hAvplay, HI_UNF_AVPLAY_EVENT_E enEvent);

/**
\brief Enables an AVPLAY channel. CNcomment:��AV������ͨ��
\attention \n
You can enable an audio channel and a video channel for each AVPLAY. If you only need to play videos or audios, you can enable the corresponding channel to save resources.
CNcomment:ÿ��AV��������֧�ִ�����Ƶͨ����1�������ֻ������Ƶ����Ƶ��ֻ��Ҫ����Ӧͨ�����Խ�ʡ��Դ��
\param[in] hAvplay  AVPLAY handle CNcomment:AV���ž��
\param[in] enChn    Separate audio channel or video channel. For details, see the description of ::HI_UNF_AVPLAY_MEDIA_CHAN_E. CNcomment:����������Ƶͨ������μ�::HI_UNF_AVPLAY_MEDIA_CHAN_E
\param[in] pPara    Pointer type. For details, see the description of ::HI_UNF_AVPLAY_OPEN_OPT_S. CNcomment:ָ�����ͣ���μ�::HI_UNF_AVPLAY_OPEN_OPT_S
    If enChn is set to HI_UNF_AVPLAY_MEDIA_CHAN_VID, this API is used to specify the maximum decoding performance of the video decoder.
    If enChn is set to NULL, the maximum performance H264+HI_UNF_VCODEC_CAP_LEVEL_FULLHD is used by default.
    The higher the configured decoding performance, the larger the required MMZ. It is recommended that you configure the performance as required.
	CNcomment:enChnΪHI_UNF_AVPLAY_MEDIA_CHAN_VIDʱ����ָ����Ƶ��������������������
    �����ΪNULL����Ĭ��Ϊ�������: H264+HI_UNF_VCODEC_CAP_LEVEL_FULLHD��
    ����֧�ֵ�����Խ����Ҫ��MMZ�����ڴ�Ҳ��Խ�󣬽��鰴�����ü��ɡ�
\retval ::HI_SUCCESS Success             CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_AVPLAY_INVALID_OPT      The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_ChnOpen(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn, const HI_VOID *pPara);

/**
\brief Disables an AVPLAY channel. CNcomment:�ر�AV������ͨ��
\attention \n
N/A
\param[in] hAvplay   AVPLAY handle CNcomment:AV���ž��
\param[in] enChn     Separate audio channel or video channel. For details, see the description of ::HI_UNF_AVPLAY_MEDIA_CHAN_E. CNcomment:����������Ƶͨ������μ�::HI_UNF_AVPLAY_MEDIA_CHAN_E
\retval ::HI_SUCCESS Success CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_AVPLAY_INVALID_OPT      The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_ChnClose(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn);

/**
\brief Starts an AVPLAY. The AVPLAY is in play mode. CNcomment:����������������PLAY״̬
\attention \n
After enabling channels and setting their attributes, you can call this API to start an AVPLAY to enable it to work in play mode. The audios and videos can be played separately or simultaneously.
CNcomment:�����ͨ���򿪺��������ú󣬵��ñ��ӿ��������ţ�����PLAY״̬��֧�ֱַ��ͬʱ��������Ƶ���š�
\param[in] hAvplay         AVPLAY handle  CNcomment:AV���ž��
\param[in] enChn           Separate audio channel or video channel. For details, see the description of ::HI_UNF_AVPLAY_MEDIA_CHAN_E. CNcomment:����������Ƶͨ������μ�::HI_UNF_AVPLAY_MEDIA_CHAN_E
\param[in] pstStartOpt     Pointer used for expansion. You can set it to NULL. CNcomment:ָ�����ͣ�����չʹ�ã���ΪNULL����
\retval ::HI_SUCCESS  Success CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_AVPLAY_INVALID_OPT      The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_Start(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn, const HI_UNF_AVPLAY_START_OPT_S *pstStartOpt);

/**
\brief Stops an AVPLAY. Then the AVPLAY is in stop mode. CNcomment:ֹͣAV���ţ�����STOP״̬
\attention \n
If you call this API, all selected channels stop playing. The playing audios and videos can be stopped separately or simultaneously.\n
The parameter pstStopOpt->enMode takes effect only when a video channel is selected.\n
If pstStopOpt->enMode is set to NULL or HI_UNF_AVPLAY_STOP_MODE_STILL, the last frame is kept still.\n
If pstStopOpt->enMode is set to HI_UNF_AVPLAY_STOP_MODE_BLACK, the last frame is cleared, and the blank screen appears.\n
When pstStopOpt->u32TimeoutMs is 0, the AVPLAY stops and this API is returned.\n
When pstStopOpt->u32TimeoutMs is greater than 0, this API is blocked until the data in the buffer is used up.\n
When pstStopOpt->u32TimeoutMs is greater than �C1, this API is blocked until the data in the buffer is used up.\n
To stop the audio or video separately when both the audio and video are enabled, you must set pstStopOpt->u32TimeoutMs to 0.
CNcomment:���ñ��ӿڽ�ֹͣ��ѡͨ���Ĳ��ţ�֧�ֱַ��ͬʱֹͣ����Ƶ���š�\n
����ѡͨ���а�����Ƶͨ��ʱ������pstStopOpt->enMode�������塣\n
��pstStopOpt->enModeΪ�ջ���ΪHI_UNF_AVPLAY_STOP_MODE_STILLʱ���������һ֡��Ƶͼ��\n
��pstStopOpt->enModeΪHI_UNF_AVPLAY_STOP_MODE_BLACKʱ�������Ƶ���һ֡����Ƶ���Ϊ������\n
��pstStopOpt->u32TimeoutMsΪ0ʱ���뿪ֹͣ���Ų����ء�\n
��pstStopOpt->u32TimeoutMs>0ʱ��������Ӧʱ�䣬ֱ�������е����ݲ��ꡣ\n
��pstStopOpt->u32TimeoutMs=-1ʱ��һֱ�����������е����ݲ��ꡣ\n
������Ƶ�����ڿ���״̬ʱ,Ҫ����ֹͣ��Ƶ����Ƶ����������pstStopOpt->u32TimeoutMsΪ0
\param[in] hAvplay  AVPLAY handle CNcomment:AV���ž��
\param[in] enChn    Separate audio channel or video channel. For details, see the description of ::HI_UNF_AVPLAY_MEDIA_CHAN_E. CNcomment:����������Ƶͨ������μ�::HI_UNF_AVPLAY_MEDIA_CHAN_E
\param[in] pstStopOpt   Pointer to the clear screen mode. For details, see the description of ::HI_UNF_AVPLAY_STOP_OPT_S. CNcomment:ָ�����ͣ�����ģʽ����μ�::HI_UNF_AVPLAY_STOP_OPT_S
\retval ::HI_SUCCESS Success  CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_AVPLAY_INVALID_OPT    The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_Stop(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn, const HI_UNF_AVPLAY_STOP_OPT_S *pstStopOpt);

/**
\brief Pauses an AVPLAY. Then the AVPLAY is in pause mode. CNcomment:��ͣAV���ţ�����PAUSE״̬
\attention \n
N/A
\param[in] hAvplay  AVPLAY handle CNcomment: AV���ž��
\param[in] pstPauseOpt  Pointer used for expansion. You can set it to NULL. CNcomment:ָ�����ͣ�����չʹ�ã���ΪΪNULL����
\retval ::HI_SUCCESS Success  CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_AVPLAY_INVALID_OPT      The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_Pause(HI_HANDLE hAvplay, const HI_UNF_AVPLAY_PAUSE_OPT_S *pstPauseOpt);

/**
\brief Plays videos or audios in trick play mode. Then the AVPLAY is in TPLAY mode. CNcomment:���ٲ��ţ�����TPLAY״̬
\attention \n
Before switching the mode to the trick play mode, you must enable a decoder to decode only I frames by calling HI_UNF_AVPLAY_SetDecodeMode API\n
CNcomment:����TPLAY״̬ǰ���������HI_UNF_AVPLAY_SetDecodeMode�ӿڰѽ���������Ϊֻ��I֡ģʽ\n
\param[in] hAvplay  AVPLAY handle CNcomment: AV���ž��
\param[in] pstTplayOpt   Pointer used for expansion. You can set it to NULL. CNcomment:ָ�����ͣ�����չʹ�ã���ΪΪNULL����
\retval ::HI_SUCCESS Success  CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_AVPLAY_INVALID_OPT    The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_Tplay(HI_HANDLE hAvplay, const HI_UNF_AVPLAY_TPLAY_OPT_S *pstTplayOpt);

/**
\brief Plays videos or audios in trick play mode. Then the AVPLAY is in fast forward mode. CNcomment:����AVPLAYΪ���ģʽ
\attention \n
N/A
\param[in] hAvplay  AVPLAY handle CNcomment: AV���ž��
\param[in] bEnable  FFMode Enable Flag CNcomment: ���ʹ�ܱ�־
\param[in] u32Speed AVPLAY play speed CNcomment: AV���ž��
\retval ::HI_SUCCESS Success  CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_AVPLAY_INVALID_OPT    The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_SetPlayerFFMode(HI_HANDLE hAvplay, HI_BOOL bEnable, HI_U32 u32Speed);

/**
\brief Resumes an AVPLAY. Then the AVPLAY is in play mode. CNcomment:�ָ�AV���ţ�����PLAY״̬
\attention \n
By calling this API, you can resume an AVPLAY from the trick play mode or pause mode rather than the stop mode.
CNcomment:���ӿ����������ٻ���ͣ״̬�ָ�Ϊ����״̬�����޷���ֹͣ״̬�ָ�Ϊ����״̬��
\param[in] hAvplay  AVPLAY handle CNcomment:AV���ž��
\param[in] pstResumeOpt  Pointer used for expansion. You can set it to NULL. CNcomment:ָ�����ͣ�����չʹ�ã�����Ϊ�ռ���
\retval ::HI_SUCCESS Success  CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_AVPLAY_INVALID_OPT    The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_Resume(HI_HANDLE hAvplay, const HI_UNF_AVPLAY_RESUME_OPT_S *pstResumeOpt);

/**
\brief Resets an AVPLAY. In this case, the play mode is not changed. CNcomment:��λAV���ţ����ı�״̬
\attention \n
N/A
\param[in] hAvplay  AVPLAY handle CNcomment:AV���ž��
\param[in] pstResetOpt   Pointer used for expansion. You can set it to NULL. CNcomment:ָ�����ͣ�����չʹ�ã���ΪΪNULL����
\retval ::HI_SUCCESS Success  CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_AVPLAY_INVALID_OPT    The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_Reset(HI_HANDLE hAvplay, const HI_UNF_AVPLAY_RESET_OPT_S *pstResetOpt);

/**
\brief Applies for a buffer for storing the streams played by an AVPLAY. CNcomment:����AV���ŵ���������
\attention \n
This API is used only when you want to play the ESs obtained from Internet or local ESs.\n
The pstData parameter is used to return the start address and size of the buffer applied for.\n
If u32TimeOutMs is set to 0, it indicates that the waiting time is 0; if u32TimeOutMs is set to 0XFFFFFFFF, it indicates that the API waits for an infinite time; if u32TimeOutMs is set to other values, it indicates that the waiting time is u32TimeOutMs ms.\n
If no buffer can be applied for during the block period, an error code indicating full buffer is returned.\n
If u32TimeOutMs is set to 0, and no buffer can be applied for, it indicates that the audio and video buffers are full. In this case, you need to call the usleep(N*1000) function to release the CPU.
Therefore, other threads can be scheduled.
CNcomment:����������򱾵�ES��ʱ����Ҫʹ�ñ��ӿڡ�\n
����pstData�������سɹ����뵽��Buffer���׵�ַ�Լ���С��\n
u32TimeOutMs����Ϊ0��ʾ���ȴ�������Ϊ0xffffffff��ʾһֱ�ȴ�������Ϊ����ֵ��ʾ�ȴ�u32TimeOutMs���롣\n
����������ʱ�䣬���޷����뵽Buffer���򷵻�buffer��������\n
u32TimeOutMs����Ϊ0ʱ��������벻��Buffer��˵����ʱ����Ƶ��Buffer��������Ҫͨ��usleep(N*1000)�ͷ�cpu
��ʹ�����߳��ܹ��õ����ȡ�
\param[in] hAvplay  AVPLAY handle CNcomment:AV���ž��
\param[in] enBufId     Buffer queue ID CNcomment:�������ID
\param[in] u32ReqLen   Size of the buffer applied for CNcomment:���뻺��Ĵ�С
\param[out] pstData    Pointer to the returned buffer CNcomment:���ػ���ָ��
\param[in] u32TimeOutMs      Wait timeout, in ms CNcomment:�ȴ���ʱʱ�䣬��λms
\retval ::HI_SUCCESS Success  CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_AVPLAY_INVALID_OPT    The operation is invalid. CNcomment:�����Ƿ�
N/A
*/
HI_S32 HI_UNF_AVPLAY_GetBuf(HI_HANDLE  hAvplay,
                            HI_UNF_AVPLAY_BUFID_E enBufId,
                            HI_U32                u32ReqLen,
                            HI_UNF_STREAM_BUF_S  *pstData,
                            HI_U32                u32TimeOutMs);

/**
\brief Updates the write pointer after data is successfully copied. CNcomment:�������ݳɹ��󣬸���дָ��
\attention \n
After transmitting streams to the buffer applied for, you can call this API to update the write pointer of the audio and video buffers.\n
If the transmitted streams do not contain PTS, u32ptsMs must be set to �C1.
CNcomment:�������뵽�Ļ���������������Ϻ󣬵��ñ��ӿڸ�������Ƶ��������дָ�롣\n
����������������û�ж�Ӧ��PTS��u32ptsMs����Ϊ-1��
\param[in] hAvplay  AVPLAY handle CNcomment:AV���ž��
\param[in] enBufId     Buffer queue ID CNcomment:�������ID
\param[in] u32ValidDataLen     Number of bytes that are written to the buffer CNcomment:ʵ��д�뻺�������ֽ���
\param[in] u32PtsMs            PTS, in ms  CNcomment:ʱ���,�Ժ���Ϊ��λ
\retval ::HI_SUCCESS Success  CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_AVPLAY_INVALID_OPT    The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_PutBuf(HI_HANDLE hAvplay, HI_UNF_AVPLAY_BUFID_E enBufId,
                         HI_U32 u32ValidDataLen, HI_U32 u32PtsMs);


/**
\brief Sets the speed in slow playing mode. CNcomment:�������ŵ��ٶ�
\attention \n
You can set the speed in slow playing mode. The value 1 indicates the normal speed, and the value 2 indicates the half of the normal speed. If you want to switch the slow playing mode to normal mode, you can set u32Repeat to 1 by calling this API.
CNcomment:�������ŵ��ٶȡ�1����������2����2��֮1�ٶȡ����Ҫ�����Żָ�����������,�Ե��øýӿڣ�u32Repeat����Ϊ1
\param[in] hAvplay             AVPLAY handle CNcomment:AV���ž��
\param[in] u32Repeat           Slow playing speed CNcomment:�����ٶ�
\retval ::HI_SUCCESS Success  CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_AVPLAY_INVALID_OPT    The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_SetWindowRepeat(HI_HANDLE hAvplay, HI_U32 u32Repeat);


/**
\brief Obtains the handle of the DMX audio channel used by an AVPLAY in TS mode. CNcomment:TSģʽʱ��ȡAV������ʹ�õ�DMX��Ƶͨ����Handle
\attention \n
N/A
\param[in] hAvplay  AVPLAY handle CNcomment:AV���ž��
\param[in] phDmxAudChn    Pointer to the handle of a DMX audio channel CNcomment:DMX��Ƶͨ��Handleָ��
\retval ::HI_SUCCESS Success  CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_AVPLAY_INVALID_OPT    The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_GetDmxAudChnHandle(HI_HANDLE hAvplay, HI_HANDLE *phDmxAudChn);

/**
\brief Obtains the handle of the DMX video channel used by an AVPLAY in TS mode. CNcomment:TSģʽʱ��ȡAV������ʹ�õ�DMX��Ƶͨ����Handle
\attention \n
N/A
\param[in] hAvplay  AVPLAY handle CNcomment:AV���ž��
\param[in] phDmxVidChn    Pointer to the handle of a DMX video channel CNcomment:DMX��Ƶͨ��Handleָ��
\retval ::HI_SUCCESS Success  CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_AVPLAY_INVALID_OPT    The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_GetDmxVidChnHandle(HI_HANDLE hAvplay, HI_HANDLE *phDmxVidChn);


/**
\brief Obtains the status information about an AVPLAY. CNcomment:��ȡAV����״̬��Ϣ
\attention \n
The parameter stSyncStatus in the data type pstStatusInfo indicates the current synchronization status. This parameter can be used to implement synchronization such as subtitle synchronization.\n
enRunStatus indicates the status of an AVPLAY.\n
u32VidFrameCount indicates the number of played video frames, and u32AuddFrameCount indicates the number of played audio frames.\n
In ES mode, you can query the statuses of the audio and video buffers by checking the stBufStatus parameter.
CNcomment:�ṹ��pstStatusInfo�в���stSyncStatus��ʾ��ȡ�ĵ�ǰͬ��״̬����������֧�������Ļ�ȵ�ͬ����\n
enRunStatus��ʾ��ȡ��AV����������״̬��\n
u32VidFrameCount��u32AuddFrameCount������ʾ��ǰ����Ƶ�Ѳ��ŵ�֡����\n
�����ESģʽ������ͨ��stBufStatus��ѯ����ƵBuffer��״̬��
\param[in] hAvplay  AVPLAY handle  CNcomment:AV���ž��
\param[out] pstStatusInfo  Pointer to the status of an AVPLAY. For details, see the description of HI_UNF_AVPLAY_STATUS_INFO_S. CNcomment:ָ�����ͣ�AV����״̬��Ϣ����μ�HI_UNF_AVPLAY_STATUS_INFO_S
\retval ::HI_SUCCESS Success  CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_AVPLAY_INVALID_OPT    The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_GetStatusInfo(HI_HANDLE          hAvplay,
                                 HI_UNF_AVPLAY_STATUS_INFO_S *pstStatusInfo);


/**
\brief Obtains the information about audio and video streams. CNcomment:��ȡ����Ƶ������Ϣ
\attention \n
N/A
\param[in] hAvplay  AVPLAY handle  CNcomment:AV���ž��
\param[in] pstStreamInfo     Pointer to the information about audio and video streams. For details, see the description of HI_UNF_AVPLAY_STREAM_INFO_S. CNcomment:ָ�����ͣ�����Ƶ������Ϣ����μ�HI_UNF_AVPLAY_STREAM_INFO_S
\retval ::HI_SUCCESS Success  CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_AVPLAY_INVALID_OPT    The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_GetStreamInfo(HI_HANDLE          hAvplay,
                                   HI_UNF_AVPLAY_STREAM_INFO_S *pstStreamInfo);

/**
\brief Obtains the information audio spectrums. CNcomment:��ȡ��Ƶ������Ϣ
\attention \n
N/A
\param[in] hAvplay  AVPLAY handle  CNcomment:AV���ž��
\param[in] pSpectrum      Pointer to the array of audio spectrums. CNcomment:ָ�����ͣ���Ƶ������Ϣ����ָ��
\param[in] u32BandNum     Length of an audio spectrum array CNcomment:��Ƶ������Ϣ���鳤��
\retval ::HI_SUCCESS Success  CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_AVPLAY_INVALID_OPT    The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_GetAudioSpectrum(HI_HANDLE hAvplay, HI_U16 *pSpectrum, HI_U32 u32BandNum);

/**
\brief Queries whether the AVPLAY buffer is empty. CNcomment:��ѯAVPLAY buffer�Ƿ��Ѿ�Ϊ��
\attention \n
N/A
\param[in] hAvplay        AVPLAY handle CNcomment:AV���ž��
\param[out] pbIsEmpty      Pointer type. This pointer indicates whether the AVPLAY buffer is empty (the playing ends). CNcomment:ָ�����ͣ�ָʾbuffer�Ƿ��Ѿ�Ϊ��(�������)
\retval ::HI_SUCCESS Success  CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_IsBuffEmpty(HI_HANDLE hAvplay, HI_BOOL * pbIsEmpty);

/**
\brief Switch the demux audio channel CNcomment:�л���ƵDEMUX���
\attention \n
N/A
\param[in] hAvplay       AVPLAY handle CNcomment: AV���ž��
\param[in] hNewDmxAud     New demux audio handle CNcomment:��DMX���
\param[out] phOldDmxAud     Old  demux audio handle CNcomment:��DMX���ָ��
\retval ::HI_SUCCESS Success  CNcomment:�ɹ�
\retval ::HI_ERR_AVPLAY_DEV_NO_INIT      The AVPLAY is not initialized. CNcomment:AVPLAYδ��ʼ��
\retval ::HI_ERR_AVPLAY_NULL_PTR         The input pointer is null. CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_PARA     The input parameter is invalid. CNcomment:��������Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AVPLAY_SwitchDmxAudChn(HI_HANDLE hAvplay, HI_HANDLE hNewDmxAud, HI_HANDLE *phOldDmxAud);




/** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

#endif
