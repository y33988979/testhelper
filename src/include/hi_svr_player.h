/**
 \file
 \brief Server (SVR) player module. CNcomment:svr playerģ��
 \author HiSilicon Technologies Co., Ltd.
 \date 2008-2018
 \version 1.0
 \author
 \date 2010-02-10
 */

#ifndef __HI_SVR_PLAYER_H__
#define __HI_SVR_PLAYER_H__

#include "hi_type.h"
#include "hi_svr_format.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_2 */
/** @{ */  /** <!-- [HiPlayer data structure] CNcomment:��Hiplayer���ݽṹ�� */

/** Invalid handle */
/** CNcomment:�Ƿ���� */
#define HI_SVR_PLAYER_INVALID_HDL        (0)

/** Normal playing speed */
/** CNcomment:�����ٶȲ��� */
#define HI_SVR_PLAYER_PLAY_SPEED_NORMAL  (1024)

/** File type */
/** CNcomment:���õ��ļ����� */
typedef enum hiSVR_PLAYER_MEDIA_PARAM_E
{
    HI_SVR_PLAYER_MEDIA_STREAMFILE = (1 << 0),    /**< Audio and video (AV) media file *//**< CNcomment:����Ƶý���ļ� */
    HI_SVR_PLAYER_MEDIA_SUBTITLE   = (1 << 1),    /**< Subtitle file *//**< CNcomment:��Ļ�ļ� */
    HI_SVR_PLAYER_MEDIA_BUTT       = (1 << 2)
} HI_SVR_PLAYER_MEDIA_PARAM_E;

/** The play mode of the player */
/** CNcomment:���ò������������� */
typedef enum hiSVR_PLAYER_PLAY_MODE_E
{
    HI_SVR_PLAYER_PLAYMODE_NORMAL = 0x0, /**< Normal play mode *//**< CNcomment: ��������ģʽ */
    HI_SVR_PLAYER_PLAYMODE_PRELOAD = 0x1,/**< Preload play mode. This mode will be used only in network playing *//**< CNcomment: Ԥ���ز���ģʽ */
    HI_SVR_PLAYER_PLAYMODE_BUTT
} HI_SVR_PLAYER_PLAY_MODE_E;

/** Player attribute ID */
/** ����������ID */
typedef enum hiSVR_PLAYER_ATTR_E
{
    HI_SVR_PLAYER_ATTR_STREAMID = 0x0,         /**< Set or obtain the stream ID. This attribute is applicable to multi-audio multi-video streams. The parameter is ::HI_SVR_PLAYER_STREAMID_S. *//**< CNcomment:����/��ȡ����ţ���������Զ���Ƶ������Ƶ�������������Ϊ::HI_SVR_PLAYER_STREAMID_S */
    HI_SVR_PLAYER_ATTR_WINDOW_HDL,             /**< Obtain the window ID. The parameter is the HI_U32 variable. *//**< CNcomment:��ȡwindow id������ΪHI_U32���� */
    HI_SVR_PLAYER_ATTR_AVPLAYER_HDL,           /**< Obtain the audio/video play (AVPlay) handle. The parameter is the HI_HANDLE variable. *//**< CNcomment:��ȡAVPlayer���������ΪHI_HANDLE���� */
    HI_SVR_PLAYER_ATTR_SO_HDL,                 /**< Obtain the subtitle output (SO) handle. The parameter is the HI_HANDLE variable. *//**< CNcomment:��ȡso���������ΪHI_HANDLE���� */
    HI_SVR_PLAYER_ATTR_SYNC,                   /**< Set the sync attribute. The parameter is ::HI_SVR_PLAYER_SYNC_ATTR_S. *//**< CNcomment:��������Ƶ����Ļʱ���ƫ�ƣ�����Ϊ::HI_SVR_PLAYER_SYNC_ATTR_S */
    HI_SVR_PLAYER_ATTR_BUTT
} HI_SVR_PLAYER_ATTR_E;

/************************************Player Event Start***********************************/

/** Error information of the player */
/** ������������Ϣ */
typedef enum hiSVR_PLAYER_ERROR_E
{
    HI_SVR_PLAYER_ERROR_NON = 0x0,
    HI_SVR_PLAYER_ERROR_VID_PLAY_FAIL,         /**< The video fails to be played. *//**< CNcomment: ��Ƶ��������ʧ�� */
    HI_SVR_PLAYER_ERROR_AUD_PLAY_FAIL,         /**< The audio fails to be played. *//**< CNcomment: ��Ƶ��������ʧ�� */
    HI_SVR_PLAYER_ERROR_PLAY_FAIL,             /**< The file fails to be played. *//**< CNcomment: ����Ƶ����ʧ�� */
    HI_SVR_PLAYER_ERROR_TIMEOUT,               /**< Operation timeout. For example, reading data timeout. *//**< CNcomment: ������ʱ�� ���ȡ���ݳ�ʱ */
    HI_SVR_PLAYER_ERROR_NOT_SUPPORT,           /**< The file format is not supportted. *//**< CNcomment: �ļ���ʽ��֧�� */
    HI_SVR_PLAYER_ERROR_UNKNOW,                /**< Unknown error. *//**< CNcomment: δ֪���� */
    HI_SVR_PALYER_ERROR_BUTT,
} HI_SVR_PLAYER_ERROR_E;

/** Buffer status, the conditions of the event report can be configured by setting ::HI_FORMAT_BUFFER_CONFIG_S.
 1. When the data size is smaller than 1 MB or duration is smaller than 200 ms, the::HI_SVR_PLAYER_BUFFER_START event is reported.
 2. When the data size is 0 byte, the ::HI_SVR_PLAYER_BUFFER_EMPTY event is reported.
 3. When the data size is smaller than 4 MB or duration is smaller than 4 minutes, the player notify the ::HI_SVR_PLAYER_BUFFER_ENOUGH event.
 4. When the data size is larger than 4 MB or duration is smaller than 5 minutes, the ::HI_SVR_PLAYER_BUFFER_FULL event is reported.  */
/** CNcomment:����״̬������Ϣ,���¼��ϱ�������,��Ҫͨ��::HI_FORMAT_BUFFER_CONFIG_S,�����á�
  Ĭ��: ����Ƶ���ݻ���С�� 1M bytes���߻�������С��200ms, �ϱ�::HI_SVR_PLAYER_BUFFER_START
  ������Ƶ����Ϊ0 bytesʱ���ϱ�::HI_SVR_PLAYER_BUFFER_EMPTY
  ������Ƶ����С��4M bytes ���� ����Ƶ��������С��4����, �ϱ�::HI_SVR_PLAYER_BUFFER_ENOUGH
  ������Ƶ���ݴ���5M bytes ���� ����Ƶ���ݴ��� 5����, �ϱ�::HI_SVR_PLAYER_BUFFER_FULL
 */
typedef enum hiSVR_PLAYER_BUFFER_E
{
    HI_SVR_PLAYER_BUFFER_EMPTY=0,   /**< The buffer is empty. *//**< CNcomment:����Ϊ�� */
    HI_SVR_PLAYER_BUFFER_START,     /**< The data in the buffer is insufficient.*//**< CNcomment:���忪ʼ�¼� */
    HI_SVR_PLAYER_BUFFER_ENOUGH,    /**< The data in the buffer is sufficient *//**< CNcomment:�����㹻,���Բ��� */
    HI_SVR_PLAYER_BUFFER_FULL,      /**< The buffer is full. *//**< CNcomment:����Ϊ�� */
    HI_SVR_PLAYER_DOWNLOAD_FIN,     /**< The download is complete. *//**< CNcomment:������� */
    HI_SVR_PLAYER_BUFFER_BUTT
} HI_SVR_PLAYER_BUFFER_E;

/** Player status */
/** CNcomment:PLAYER״̬ */
typedef enum hiSVR_PLAYER_STATE_E
{
    HI_SVR_PLAYER_STATE_INI = 0,    /**< The player is in the initial state. It changes to the initial state after being created. *//**< CNcomment:��������ǰ���ڳ�ʼ״̬��create�󲥷�������ini״̬ */
    HI_SVR_PLAYER_STATE_DEINI,      /**< The player is deinitialized. *//**< CNcomment:�������Ѿ�ȥ��ʼ״̬ */
    HI_SVR_PLAYER_STATE_PLAY,       /**< The player is in the playing state. *//**< CNcomment:��������ǰ���ڲ���״̬ */
    HI_SVR_PLAYER_STATE_FORWARD,    /**< The player is in the fast forward state. *//**< CNcomment:��������ǰ���ڿ��״̬ */
    HI_SVR_PLAYER_STATE_BACKWARD,   /**< The player is in the rewind state. *//**< CNcomment:��������ǰ���ڿ���״̬ */
    HI_SVR_PLAYER_STATE_PAUSE,      /**< The player is in the pause state. *//**< CNcomment:��������ǰ������ͣ״̬ */
    HI_SVR_PLAYER_STATE_STOP,       /**< The player is in the stop state. *//**< CNcomment:��������ǰ����ֹͣ״̬ */
    HI_SVR_PLAYER_STATE_BUTT
} HI_SVR_PLAYER_STATE_E;

/** Event type */
/** �¼����� */
typedef enum hiSVR_PLAYER_EVENT_E
{
    HI_SVR_PLAYER_EVENT_STATE = 0x0,   /**< Player status change event. The parameter value is ::HI_SVR_PLAYER_STATE_E. *//**< CNcomment:������״̬ת���¼�������ֵΪ::HI_SVR_PLAYER_STATE_E */
    HI_SVR_PLAYER_EVENT_SOF,           /**< Event indicating that file playing starts or a file is rewound to the file header. The parameter type is HI_U32. The value ::HI_SVR_PLAYER_STATE_PLAY indicates that file playing starts and the value ::HI_SVR_PLAYER_STATE_BACKWARD indicates that a file is rewound to the file header. */
                                       /**< CNcomment:�ļ���ʼ���Ż���˵��ļ�ͷ�¼�����������ΪHI_U32��ֵΪ::HI_SVR_PLAYER_STATE_PLAY��ʾ��ʼ���ţ�����ֵΪ::HI_SVR_PLAYER_STATE_BACKWARD��ʾ���˵��ļ�ͷ */
    HI_SVR_PLAYER_EVENT_EOF,           /**< Event indicating that a file is played till the end of the file. There is no parameter. *//**< CNcomment:�ļ����ŵ�β�¼����޲��� */
    HI_SVR_PLAYER_EVENT_PROGRESS,      /**< Event indicating the current progress of the player. This event is reported once every 500 ms. The parameter is ::HI_SVR_PLAYER_PROGRESS_S. *//**< CNcomment:��������ǰ���Ž����¼���ÿ��300ms�ϱ�һ�θ��¼�������ֵΪ::HI_SVR_PLAYER_PROGRESS_S */
    HI_SVR_PLAYER_EVENT_STREAMID_CHG,  /**< Stream ID change event. The parameter is ::HI_SVR_PLAYER_STREAMID_S. *//**< CNcomment:stream id �����仯�¼�������Ϊ::HI_SVR_PLAYER_STREAMID_S */
    HI_SVR_PLAYER_EVENT_SEEK_FIN,      /**< Seek operation is complete. The parameter is HI_U32. When the value of the parameter is ::HI_FAILURE, the seek operation fails. When the value of the parameter is ::HI_SUCCESS, the seek operation succeeds. When the value of the parameter is ::HI_FORMAT_ERRO_ENDOFFILE, the file is read to the end. */
                                       /**< CNcomment:Seek������ɣ�����ΪHI_U32��ֵΪ::HI_FAILURE Seekʧ�ܣ�::HI_SUCCESS seek�ɹ���::HI_FORMAT_ERRO_ENDOFFILE �ļ���ȡ���ļ�β */
    HI_SVR_PLAYER_EVENT_CODETYPE_CHG,  /**< Event of indicating the byte encoding configuration is complete. The parameter is ::HI_SVR_PLAYER_SUB_CODETYPE_S. *//**< Ncomment:�����ַ���������¼����¼�����::HI_SVR_PLAYER_SUB_CODETYPE_S */
    HI_SVR_PLAYER_EVENT_DOWNLOAD_PROGRESS,   /**< Current download progress of the player. The event is reported every 300 ms. The parameter is ::HI_SVR_PLAYER_PROGRESS_S. *//**< CNcomment:��������ǰ���ؽ��ȣ�ÿ��300ms�ϱ�һ�θ��¼�������ֵΪ::HI_SVR_PLAYER_PROGRESS_S */
    HI_SVR_PLAYER_EVENT_BUFFER,        /**< Reporting buffer status. The parameter type is ::HI_SVR_PLAYER_BUFFER_S. *//**< CNcomment:����״̬�ϱ�,��������Ϊ::HI_SVR_PLAYER_BUFFER_S */
    HI_SVR_PLAYER_EVENT_FIRST_FRAME_TIME,    /**< The display time of the first frame from setting the setMedia. The parameter is ::HI_U32, in the unit of ms. *//**< CNcomment:������ý��setMedia��ʼ��һ֡��ʾʱ��,����Ϊ::HI_U32,��λΪms */

    HI_SVR_PLAYER_EVENT_ERROR,               /**< Event of indicating that an error occurs in the player. The parameter is ::HI_SVR_PLAYER_ERROR_E.*//**< CNcomment:������������Ϣ�¼�������Ϊ::HI_SVR_PLAYER_ERROR_E */
    HI_SVR_PLAYER_EVNET_NETWORK,       /**< Report the network status. The parameter is ::HI_FORMAT_NET_STATUS_S. *//**< CNcomment:����״̬�ϱ�, ����Ϊ::HI_FORMAT_NET_STATUS_S */
    HI_SVR_PLAYER_EVENT_BUTT
} HI_SVR_PLAYER_EVENT_E;

/** Information of playing progress */
/** CNcomment:�����¼���Ϣ���� */
typedef struct hiSVR_PLAYER_PROGRESS_S
{
    HI_U32 u32Progress;    /**< Progress of current playing. The value ranges from 0 to 100. *//**< CNcomment:����ֵ, ֵ0-100 */
    HI_S64 s64Duration;    /**< Duration (in the unit of ms) of current playing *//**< CNcomment:��ǰ����ʱ��,��λΪms */
    HI_S64 s64BufSize;     /**< Data size (in the unit of byte) in the buffer *//**< CNcomment:�������ݴ�С,��λΪbytes */
} HI_SVR_PLAYER_PROGRESS_S;

/** Information of the buffer event */
/** CNcomment:HI_SVR_PLAYER_EVENT_BUFFER �����¼����� */
typedef struct hiSVR_PLAYER_BUFFER_S
{
    HI_SVR_PLAYER_BUFFER_E eType;        /**< Buffer type *//**< CNcomment:��������::HI_SVR_PLAYER_BUFFER_E */
    HI_FORMAT_BUFFER_STATUS_S stBufStat; /**< Buffer status *//**< CNcomment:����״̬::HI_FORMAT_BUFFER_STATUS_S */
} HI_SVR_PLAYER_BUFFER_S;

/** Player event callback parameters */
/** CNcomment:�������¼��ص����� */
typedef struct hiSVR_PLAYER_EVENT_S
{
    HI_SVR_PLAYER_EVENT_E  eEvent;    /**< Event type *//**< CNcomment:�¼����� */
    HI_U32  u32Len;                   /**< Event parameter length, in the unit of byte. *//**< CNcomment:�¼��������ȣ��ֽ�Ϊ��λ */
    HI_U8   *pu8Data;                 /**< Start address of event parameter data *//**< CNcomment:�¼�����������ʼ��ַ */
} HI_SVR_PLAYER_EVENT_S;

/************************************Player Event End************************************/

/** Type of the registered dynamic link library (DLL) */
/** CNcomment:ע��Ķ�̬������ */
typedef enum hiSVR_PLAYER_DLLTYPE_E
{
    HI_SVR_PLAYER_DLL_PARSER = 0x0,    /**< DLLs for parsing audio and video media files and subtitle files *//**< CNcomment:����Ƶý���ļ���������Ļ������̬�� */
    HI_SVR_PLAYER_DLL_BUTT
} HI_SVR_PLAYER_DLLTYPE_E;

/** Playing speed flag */
/** CNcomment:�����ٶȱ�ʶ */
typedef enum hiSVR_PLAYER_PLAY_SPEED_E
{
    HI_SVR_PLAYER_PLAY_SPEED_2X_FAST_FORWARD   = 2 * HI_SVR_PLAYER_PLAY_SPEED_NORMAL,      /**< 2 x speed fast forward *//**< CNcomment:2���ٿ�� */
    HI_SVR_PLAYER_PLAY_SPEED_4X_FAST_FORWARD   = 4 * HI_SVR_PLAYER_PLAY_SPEED_NORMAL,      /**< 4 x speed fast forward *//**< CNcomment:4���ٿ�� */
    HI_SVR_PLAYER_PLAY_SPEED_8X_FAST_FORWARD   = 8 * HI_SVR_PLAYER_PLAY_SPEED_NORMAL,      /**< 8 x speed fast forward *//**< CNcomment:8���ٿ�� */
    HI_SVR_PLAYER_PLAY_SPEED_16X_FAST_FORWARD  = 16 * HI_SVR_PLAYER_PLAY_SPEED_NORMAL,     /**< 16 x speed fast forward *//**< CNcomment:16���ٿ�� */
    HI_SVR_PLAYER_PLAY_SPEED_32X_FAST_FORWARD  = 32 * HI_SVR_PLAYER_PLAY_SPEED_NORMAL,     /**< 32 x speed fast forward *//**< CNcomment:32���ٿ�� */
    HI_SVR_PLAYER_PLAY_SPEED_64X_FAST_FORWARD  = 64 * HI_SVR_PLAYER_PLAY_SPEED_NORMAL,     /**< 64 x speed fast forward *//**< CNcomment:64���ٿ�� */
    HI_SVR_PLAYER_PLAY_SPEED_2X_FAST_BACKWARD  = -2 * HI_SVR_PLAYER_PLAY_SPEED_NORMAL,     /**< 2 x speed rewind *//**< CNcomment:2���ٿ��� */
    HI_SVR_PLAYER_PLAY_SPEED_4X_FAST_BACKWARD  = -4 * HI_SVR_PLAYER_PLAY_SPEED_NORMAL,     /**< 4 x speed rewind *//**< CNcomment:4���ٿ��� */
    HI_SVR_PLAYER_PLAY_SPEED_8X_FAST_BACKWARD  = -8 * HI_SVR_PLAYER_PLAY_SPEED_NORMAL,     /**< 8 x speed rewind *//**< CNcomment:8���ٿ��� */
    HI_SVR_PLAYER_PLAY_SPEED_16X_FAST_BACKWARD = -16 * HI_SVR_PLAYER_PLAY_SPEED_NORMAL,    /**< 16 x speed rewind *//**< CNcomment:16���ٿ��� */
    HI_SVR_PLAYER_PLAY_SPEED_32X_FAST_BACKWARD = -32 * HI_SVR_PLAYER_PLAY_SPEED_NORMAL,    /**< 32 x speed rewind *//**< CNcomment:32���ٿ��� */
    HI_SVR_PLAYER_PLAY_SPEED_64X_FAST_BACKWARD = -64 * HI_SVR_PLAYER_PLAY_SPEED_NORMAL,    /**< 64 x speed rewind *//**< CNcomment:64���ٿ��� */
    HI_SVR_PLAYER_PLAY_SPEED_BUTT                                                          /**< Invalid speed value *//**< CNcomment:��Ч���ٶ�ֵ */
} HI_SVR_PLAYER_PLAY_SPEED_E;

/** The parameters need to be specified when the player is created. */
/** CNcomment:����������ʱ��ָ���Ĳ��� */
typedef struct hiSVR_PLAYER_PARAM_S
{
    HI_U32  u32DmxId;                 /**< Dmx ID. This parameter is mandatory when transport streams (TSs) are played. *//**< CNcomment:dmx id������ts��ʱ�����øò��� */
    HI_U32  u32PortId;                /**< Port ID. This parameter is mandatory when TS streams are played. *//**< CNcomment:port id������ts��ʱ�����øò��� */
    HI_U32  x, y, w, h;               /**< Coordinate, width, and height of the video output window. This parameter is invalid in the case of hAVPlayer!=HI_SVR_PLAYER_INVALID_HDL. *//**< CNcomment:��Ƶ����������꼰��ߣ�hAVPlayer!=HI_SVR_PLAYER_INVALID_HDL���ò�����Ч */
    HI_U32  u32MixHeight;             /**< Audio output mix weight. The value ranges from 0 to 100. This parameter is invalid in the case of hAVPlayer!=HI_SVR_PLAYER_INVALID_HDL. *//**< CNcomment:��Ƶ�������Ȩ��0-100��hAVPlayer!=HI_SVR_PLAYER_INVALID_HDL���ò�����Ч */
    HI_HANDLE hAVPlayer;              /**< AVPlay created externally. A window device and a sound device are bound to the AVPlay and the window size and the sound volume are set.
                                                                    The AVPlay handle can be transferred to the HiPlayer, which will use the AVPlay for playing. If this parameter is set to
                                                                    HI_SVR_PLAYER_INVALID_HDL, the HiPlayer internally creates the AVPlay and window automatically. */
                                      /**< CNcomment:�ⲿ�Ѿ�������avplay������window,sound�������ú�window�Ĵ�Сλ�ã�sound��������
                                                                    ���Խ�avplay�������HiPlayer��HiPlayer����ʹ�ø�avplay���š�����ò�������Ϊ
                                                                    HI_SVR_PLAYER_INVALID_HDL��HiPlayer�ڲ����Զ�����avplayer��window */
    HI_U32   u32SndPort;             /**< Specified audio port number, 0: master audio, 1: slave audio  *//**< CNcomment:ָ����Ƶ�˿ںţ�0����������1������*/
    HI_HANDLE hDRMClient;            /**< DRM client created externally. */
} HI_SVR_PLAYER_PARAM_S;

/** Input media file */
/** CNcomment:�����ý���ļ� */
typedef struct hiSVR_PLAYER_MEDIA_S
{
    HI_CHAR  aszUrl[HI_FORMAT_MAX_URL_LEN];          /**< File path, absolute file path, such as /mnt/filename.ts. *//**< CNcomment:�ļ�·��������·������/mnt/filename.ts */
    HI_S32   s32PlayMode;                            /**< Set the mode of the player. The parameter is ::HI_SVR_PLAYER_PLAY_MODE_E *//**< CNcomment:���ò���ģʽ,����::HI_SVR_PLAYER_PLAY_MODE_E */
    HI_U32   u32SubTitileNum;                        /**< Number of subtitle files *//**< CNcomment:��Ļ�ļ����� */
    HI_CHAR  aszSubTitle[HI_FORMAT_MAX_LANG_NUM][HI_FORMAT_MAX_URL_LEN];  /**< Absolute path of a subtitle file, such as /mnt/filename.ts. *//**< CNcomment:��Ļ�ļ�·��������·������/mnt/filename.ts */
    HI_U32   u32UserData;                            /**< Create a handle by calling the fmt_open function, send the user data to the DEMUX by calling the fmt_invoke, and then call the fmt_find_stream function. */
                                                     /**< CNcomment:�û����ݣ�HiPlayer����͸�������ý�����fmt_open֮��ͨ��fmt_invoke�ӿڴ��ݸ�������(HI_FORMAT_INVOKE_SET_USERDATA)���ٵ���fmt_find_stream�ӿ� */
} HI_SVR_PLAYER_MEDIA_S;

/** IDs of the AV and subtitle streams to be switched. */
/** CNcomment:Ҫ�л���������Ƶ����Ļ��ID  */
typedef struct hiSVR_PLAYER_STREAMID_S
{
    HI_U32 u32ProgramId;   /**< Program ID. The value is the subscript of the astProgramInfo array in the ::HI_FORMAT_FILE_INFO_S structure and ranges from 0 to (u32ProgramNum - 1). */
                           /**< CNcomment:��Ŀid��ֵΪ::HI_FORMAT_FILE_INFO_S�ṹastProgramInfo�����±꣬0-(u32ProgramNum - 1) */
    HI_U32 u32VStreamId;   /**< Video stream ID. The value equals the value of u32ProgramId. *//**< CNcomment:��Ƶ��id��ֵ��u32ProgramId��� */
    HI_U32 u32AStreamId;   /**< Audio stream ID. The value is the subscript of the astAudStream array in the ::HI_FORMAT_PROGRAM_INFO_S structure and ranges from 0 to (u32AudStreamNum - 1).*/
                           /**< CNcomment:��Ƶ��id��ֵΪ::HI_FORMAT_PROGRAM_INFO_S�ṹastAudStream�����±꣬0-(u32AudStreamNum - 1)*/
    HI_U32 u32SubTitleId;  /**< Subtitle ID. The value is the subscript of the astSubTitle array in the ::HI_FORMAT_PROGRAM_INFO_S structure and ranges from 0 to (u32SubTitleNum - 1). */
                           /**< CNcomment:��Ļid��ֵΪ::HI_FORMAT_PROGRAM_INFO_S�ṹastSubTitle�����±꣬0-(u32SubTitleNum - 1) */
} HI_SVR_PLAYER_STREAMID_S;

/** Set the offset of the stream PTS */
/** ��������Ƶ����Ļʱ��ƫ�� */
typedef struct hiSVR_PLAYER_SYNC_ATTR_S
{
    HI_S32 s32VFrameOffset;    /**< offset (in the unit of ms) of the video stream. Add this offset to the PTS of the video stream. *//**< ��Ƶ��ƫ��ֵ������Ƶ��ʱ������������Ӹ�offset����λms */
    HI_S32 s32AFrameOffset;    /**< offset (in the unit of ms) of the audio stream. Add this offset to the PTS of the video stream. *//**< ��Ƶ��ƫ��ֵ������Ƶ��ʱ������������Ӹ�offset����λms */
    HI_S32 s32SubTitleOffset;  /**< offset (in the unit of ms) of the subtitle stream. Add this offset to the PTS of the video stream. *//**< ��Ļ��ƫ��ֵ������Ƶ��ʱ������������Ӹ�offset����λms */
} HI_SVR_PLAYER_SYNC_ATTR_S;

/** Player information */
/** CNcomment:��������Ϣ */
typedef struct hiSVR_PLAYER_INFO_S
{
    HI_U32  u32Progress;                 /**< Playing progress. The value ranges from 0 to 100. *//**< CNcomment:���Űٷֱȣ�0-100 */
    HI_U64  u64TimePlayed;               /**< Elapsed time, in the unit of ms. *//**< CNcomment:�Ѳ���ʱ�䣬��λms */
    HI_S32  s32Speed;                    /**< Playing speed *//**< CNcomment:�������� */
    HI_SVR_PLAYER_STATE_E  eStatus;      /**< Playing status *//**< CNcomment:����״̬ */
} HI_SVR_PLAYER_INFO_S;

/** The result of setting the subtitle transcoding type */
/** Invoke����������Ļ�������͵��¼��ص����� */
typedef struct hiSVR_PLAYER_SUB_CODETYPE_S
{
    HI_U32 u32SubId;                    /**< Subtitle ID. For details, see ::HI_FORMAT_SUBTITLE_INFO_S:s32StreamIndex *//**< ��ĻID���ο�::HI_FORMAT_SUBTITLE_INFO_S:s32StreamIndex���� */
    HI_U32 u32Result;                   /**< Result of setting the subtitle transcoding type *//**< invoke�����ֵ */
} HI_SVR_PLAYER_SUB_CODETYPE_S;

/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API declaration *****************************/
/** \addtogroup      H_1_1 */
/** @{ */  /** <!-- [HiPlayer] CNcomment:��Hiplayer�� */

/**
\brief Player event callback function. The ::HI_SVR_PLAYER_RegCallback interface can be called to register the callback function. CNcomment:�������¼��ص�����������::HI_SVR_PLAYER_RegCallback�ӿ�ע��ûص�����
\attention \n
None.
\param[out] hPlayer player handle. CNcomment:���������
\param[out] pstruEvent event parameter. CNcomment: �¼�����

\retval ::HI_SUCCESS

\see \n
None.
*/
typedef HI_S32 (*HI_SVR_PLAYER_EVENT_FN)(HI_HANDLE hPlayer, HI_SVR_PLAYER_EVENT_S *pstruEvent);

/**
\brief Initialize the player. CNcomment:��ʼ��Player
\attention \n
The HI_S32 HI_SVR_PLAYER_Init interface can be called to return a success message after the initialization is successful. Multiple processes are not supported
. This interface must be called prior to other interfaces.
CNcomment:��ʼ���ɹ����ٵ��øýӿڷ��سɹ�����֧�ֶ���̣����������ӿ�ǰ�����ȵ��øýӿ�
\param None. CNcomment:��

\retval ::HI_SUCCESS The initialization is successful. CNcomment:��ʼ���ɹ�
\retval ::HI_FAILURE The initialization fails. CNcomment:��ʼ��ʧ��

\see \n
None.
*/
HI_S32 HI_SVR_PLAYER_Init(HI_VOID);

/**
\brief Deinitialize the player module by calling the HI_S32 HI_SVR_PLAYER_Deinit interface. The player module is not used any more. CNcomment:ȥ��ʼ��playerģ�飬����ʹ��playerģ�飬���øýӿ�ȥ��ʼ��playerģ��
\attention \n
The ::HI_SVR_PLAYER_Destroy interface must be called to release the created player first. Otherwise, a failure is returned. The Deinit interface does not release the player resource. \n
This interface can be called to return a success message after the deinitialization is successful.
CNcomment:�����ȵ���::HI_SVR_PLAYER_Destroy�ӿ��ͷŵ������Ĳ��������ٵ��øýӿڣ�����᷵��ʧ�ܣ�Deinit�������ͷ�\n
��������Դ��ȥ��ʼ���ɹ����ٵ��øýӿڷ��سɹ�
\param None. CNcomment:��

\retval ::HI_SUCCESS The deinitialization is successful. CNcomment:ȥ��ʼ���ɹ�
\retval ::HI_FAILURE The deinitialization fails and the created player is not released. CNcomment:ȥ��ʼ��ʧ�ܣ�û���ͷŵ������Ĳ�����

\see \n
None.
*/
HI_S32 HI_SVR_PLAYER_Deinit(HI_VOID);

/**
\brief Create a player. CNcomment:����һ��������
\attention \n
This interface must be called after initialization. Only one player can be created. \n
CNcomment:�ýӿڱ����ڳ�ʼ������ã�֧����ഴ��һ��������\n
\param[in] pstruParam player initialization attribute. The available Dmx ID and port ID must be specified for TS streams to be played. CNcomment:��������ʼ�����ԣ�����ts��������ָ�����õ�dmx id��port id
\param[out] phPlayer handle of the created player. CNcomment:�����Ĳ��������

\retval ::HI_SUCCESS A player is created successfully and the player handle is valid. CNcomment:�����ɹ��������������Ч
\retval ::HI_FAILURE A player fails to be created. The parameters are invalid or resources are insufficient. CNcomment:����ʧ�ܣ������Ƿ�����Դ����

\see \n
None.
*/
HI_S32 HI_SVR_PLAYER_Create(const HI_SVR_PLAYER_PARAM_S *pstruParam, HI_HANDLE *phPlayer);

/**
\brief Set the URL of the media file to be played. The URL format is absolute path + media file name. CNcomment:����Ҫ���ŵ�ý���ļ�url��ַ��url��ʽΪ:����·��+ý���ļ���
\attention \n
The HI_SVR_PLAYER_SetMedia interface is a synchronous interface. It must be called after the ::HI_SVR_PLAYER_Create interface is called or after the ::
HI_SVR_PLAYER_Stop interface is called to stop the player.
This interface performs the following operations:
1. Queries the media file set by the DEMUX during parsing.
2. Queries a subtitle file if no subtitle file is specified.
3. Performs the following steps if no AVPlay is specified:
   Creates an AVPlay and sets the audio and video attributes based on the parsed file attributes such as the AV encoding type.
   Creates a window, set the window size, and bind the window to the AVPlay.
   Bind HI_UNF_SND_0 to the AVPlay.
The window display location and mixheight set for binding HI_UNF_SND_0 to the AVPlay are specified when the ::HI_SVR_PLAYER_Create interface is called.
The window display location and mixheight are invalid if the AVPlay is specified externally. The player creates a TS buffer based on the Dmx ID and port ID
if TS streams need to be played.
CNcomment:ͬ���ӿڣ��ýӿڱ����ڵ���::HI_SVR_PLAYER_Create֮����ã����ߵ���::HI_SVR_PLAYER_Stop�ӿ�ֹͣ�����������
�ý���Ҫִ�����´���:
1�����ҽ������������õ�ý���ļ�
2�����û��ָ����Ļ�ļ����������Ļ�ļ�
3�����û��ָ��avplay��
   ����avplay�����ݽ��������ļ�������������Ƶ���ԣ�������Ƶ��������
   ����window������window����λ�ã���window�󶨵�avplay
   ��HI_UNF_SND_0�豸�󶨵�avplay
window��ʾλ�ã�HI_UNF_SND_0�󶨵�avplay���õ�mixheight(���avplay���ⲿָ��������ʾλ�ú�mixheight������Ч)��
�ڵ���::HI_SVR_PLAYER_Create�ӿ�ʱָ��������ǲ���ts���������������ָ����dmx id,port id����ts buffer

\param[in] hPlayer handle of the player is created by calling the ::HI_SVR_PLAYER_Create interface. CNcomment:ͨ������::HI_SVR_PLAYER_Create�ӿڴ����Ĳ��������
\param[in] eType media file. If only AV media files are specified, set this parameter to HI_SVR_PLAYER_MEDIA_STREAMFILE.\n
           If a subtitle file is specified, set this parameter to HI_SVR_PLAYER_MEDIA_STREAMFILE | HI_SVR_PLAYER_MEDIA_SUBTITLE.
           CNcomment:ý���ļ���ָֻ������Ƶý���ļ�����ò�������ΪHI_SVR_PLAYER_MEDIA_STREAMFILE��\n
            �����ָ������Ļ�ļ�������(HI_SVR_PLAYER_MEDIA_STREAMFILE | HI_SVR_PLAYER_MEDIA_SUBTITLE)
\param[in] pstruMedia media file information. Only absolute path is supported. If no subtitle file is set, \n
           the player searches for a subtitle file whose name is the same as the name of the audio and video media files in the same directory as the audio and video media files automatically.
            CNcomment:ý���ļ���Ϣ��ֻ֧�־���·�������û��������Ļ�ļ������������Զ�������Ƶý\n
            ���ļ�����Ŀ¼�²���������Ƶý���ļ�����ͬ����Ļ�ļ�

\retval ::HI_SUCCESS The media file is set successfully. The ::HI_SVR_PLAYER_Play interface can be called to start playing the file. CNcomment:ý���ļ����óɹ�����ʱ����::HI_SVR_PLAYER_Play�ӿڿ��Կ�ʼ����
\retval ::HI_FAILURE The media file fails to be set. CNcomment:ý���ļ�����ʧ��
\retval ::HI_ERRNO_NOT_SUPPORT_FORMAT The file format is not supported. CNcomment:��֧�ֵ��ļ���ʽ
\retval ::HI_ERRNO_NOT_SUPPORT_PROTOCOL The protocol is not supported. CNcomment:��֧�ֵ�Э��

\see \n
None.
*/
HI_S32 HI_SVR_PLAYER_SetMedia(HI_HANDLE hPlayer, HI_U32 eType, HI_SVR_PLAYER_MEDIA_S *pstruMedia);

/**
\brief Destroy a player instance. CNcomment:����һ��������ʵ��
\attention \n
The HI_S32 HI_SVR_PLAYER_Destroy interface is called to destroy the player resource after the ::HI_SVR_PLAYER_Create interface is called to create a player.
CNcomment:����::HI_SVR_PLAYER_Create�����������󣬵��øýӿ����ٲ�������Դ
\param[in] hPlayer player handle. CNcomment:���������

\retval ::HI_SUCCESS The player is released successfully. CNcomment:�������ͷųɹ�
\retval ::HI_FAILURE The operation is invalid. CNcomment:�Ƿ�����

\see \n
None.
*/
HI_S32 HI_SVR_PLAYER_Destroy(HI_HANDLE hPlayer);

/**
\brief Set player parameters. CNcomment:���ò���������
\attention \n
The following operations are supported:
The ::HI_SVR_PLAYER_ATTR_STREAMID interface can be used to set the ID of the stream to be played. It can be called to set the audio streams to be played for
single-video multi-audio files. \n
Calling the HI_SVR_PLAYER_SetParam interface is an asynchronous operation during playing. Nevertheless, calling this interface is a synchronous operation
after the SetMedia operation is performed prior to playing. Values returned by the interface cannot be used to check whether the operation is successful.
The player notifies the application (APP) of the stream ID setting status by using the ::HI_SVR_PLAYER_EVENT_STREAMID_CHG event. The event parameter is ::
HI_SVR_PLAYER_STREAMID_S.
CNcomment:�ýӿڱ����ڵ���::HI_SVR_PLAYER_SetMedia�ӿں����
CNcomment:֧�ֵĲ�������
::HI_SVR_PLAYER_ATTR_STREAMID : ���ò��ŵ���id�����ڵ���Ƶ������Ƶ�ļ�������ͨ���ò�������Ҫ���ŵ���Ƶ��\n
���Ź����е��øýӿ�Ϊ�첽����������ǰ��SetMedia����øýӿ�Ϊͬ���������첽��������ͨ���ӿڷ���ֵ���ж�\n
��id�Ƿ����óɹ�����������ͨ��::HI_SVR_PLAYER_EVENT_STREAMID_CHG�¼�֪ͨapp���¼�����Ϊ::HI_SVR_PLAYER_STREAMID_S��\n
::HI_SVR_PLAYER_ATTR_SYNC : ��������Ƶ����Ļ��ʱ���ƫ�ƣ�HiPlayer��ȡ����Ƶ֡��ʱ����������õ�ƫ��ֵ������ͬ��

\param[in] hPlayer player handle. CNcomment:���������
\param[in] eAttrId ID of the player parameter to be set. CNcomment:Ҫ���õĲ���������ID
\param[in] pArg player parameter to be set. CNcomment:Ҫ���õĲ���������

\retval ::HI_SUCCESS Parameters are set successfully. CNcomment:�������óɹ�
\retval ::HI_FAILURE The operation fails. CNcomment:����ʧ��

\see \n
None.
*/
HI_S32 HI_SVR_PLAYER_SetParam(HI_HANDLE hPlayer, HI_SVR_PLAYER_ATTR_E eAttrId, const HI_VOID *pArg);

/**
\brief Obtain player parameters. CNcomment:��ȡ����������
\attention \n
This interface must be called after the ::HI_SVR_PLAYER_SetMedia interface is called.
The following operations are supported:
::HI_SVR_PLAYER_ATTR_STREAMID: Obtain the ID of the stream that is played currently.
::HI_SVR_PLAYER_ATTR_WINDOW_HDL: Obtain the window handle created by the player.
::HI_SVR_PLAYER_ATTR_AVPLAYER_HDL: Obtain the AVPlay handle created by the player.
::HI_SVR_PLAYER_ATTR_SO_HDL: Obtain the SO module handle created by the player.

CNcomment:�ýӿڱ����ڵ���::HI_SVR_PLAYER_SetMedia�ӿں����
֧�ֵĲ�������
::HI_SVR_PLAYER_ATTR_STREAMID : CNcomment:��ȡ��ǰ���ŵ���id
::HI_SVR_PLAYER_ATTR_WINDOW_HDL : CNcomment:��ȡ������������window���
::HI_SVR_PLAYER_ATTR_AVPLAYER_HDL : CNcomment:��ȡ������������avplay���
::HI_SVR_PLAYER_ATTR_SO_HDL : CNcomment:��ȡ��������������Ļ���ģ��(so)���

\param[in] hPlayer player handle. CNcomment:���������
\param[in] eAttrId player parameter ID. CNcomment:����������ID
\param[out] pArg obtained player parameters. CNcomment:��ȡ�Ĳ���������

\retval ::HI_SUCCESS Parameters are obtained successfully. CNcomment:��ȡ�����ɹ�
\retval ::HI_FAILURE The operation fails. CNcomment:����ʧ��

\see \n
None.
*/
HI_S32 HI_SVR_PLAYER_GetParam(HI_HANDLE hPlayer, HI_SVR_PLAYER_ATTR_E eAttrId, HI_VOID *pArg);

/**
\brief Register a player event callback function. CNcomment:ע�Ქ�����¼��ص�����
\attention \n
This interface must be called after the ::HI_SVR_PLAYER_Create interface is called. This interface is unrelated to the player status. This function cannot be called in any player callback event.
CNcomment:�ú��������ڵ���::HI_SVR_PLAYER_Create�ӿں���ã��ýӿ��벥����״̬�޹أ��ú���������player�κλص��¼��е���
\param[in] hPlayer player handle. CNcomment:���������
\param[in] pfnCallback player event callback function. The callback function definition is ::HI_SVR_PLAYER_EVENT_FN. CNcomment:�������¼��ص��������ص���������::HI_SVR_PLAYER_EVENT_FN

\retval ::HI_SUCCESS The registration is successful. CNcomment:ע��ɹ�
\retval ::HI_FAILURE The registration fails. CNcomment:����ʧ��

\see \n
None.
*/
HI_S32 HI_SVR_PLAYER_RegCallback(HI_HANDLE hPlayer, HI_SVR_PLAYER_EVENT_FN pfnCallback);

/**
\brief Start playing. CNcomment:��ʼ����
\attention \n
This interface is an asynchronous interface. It must be called after the ::HI_SVR_PLAYER_SetMedia interface is called. If this interface is called after successful playing, HI_FAILURE is returned. \n
Values returned by this interface cannot be used to check whether the playing is successful. The player notifies the APP of playing success or failure by using the ::HI_SVR_PLAYER_EVENT_STATE event. \n
The event parameter value is ::HI_SVR_PLAYER_STATE_PLAY.This interface can be called to restart playing after the playing stops.

CNcomment:�첽�ӿڣ��ýӿڱ����ڵ���::HI_SVR_PLAYER_SetMedia�ӿں���ã����ųɹ����ٵ��øýӿڷ���HI_FAILURE��\n
����ͨ���ýӿڷ���ֵ���жϲ������Ƿ񲥷ųɹ�����������ͨ��::HI_SVR_PLAYER_EVENT_STATE�¼�֪ͨ\n
app���ųɹ����¼�����ֵΪ::HI_SVR_PLAYER_STATE_PLAY��ֹͣ���ź󣬿��Ե��øýӿ����²���

\param[in] hPlayer player handle. CNcomment:���������

\retval ::HI_SUCCESS The operation is valid. CNcomment:�Ϸ�����
\retval ::HI_FAILURE The operation is invalid. CNcomment:�Ƿ�����

\see \n
None.
*/
HI_S32 HI_SVR_PLAYER_Play(HI_HANDLE hPlayer);

/**
\brief Stop playing. CNcomment:ֹͣ����
\attention \n
This interface is an asynchronous interface. It can be called to stop playing during playing, fast forward, rewind, and pause. Values returned by this interface cannot be used to check whether playing is stopped successfully. \n
The player notifies the APP of stop success or failure by using the ::HI_SVR_PLAYER_EVENT_STATE event. The event parameter value is ::HI_SVR_PLAYER_STATE_STOP. \n
The ::HI_SVR_PLAYER_Play interface can be called to restart playing after playing is stopped.

CNcomment:�첽�ӿڣ����š���������ˡ���ͣ�����ж����Ե��øýӿ�ֹͣ���ţ�����ͨ���ýӿڷ���ֵ���жϲ�����\n
�Ƿ�ֹͣ�ɹ�����������ͨ��::HI_SVR_PLAYER_EVENT_STATE�¼�֪ͨappֹͣ�ɹ����¼�����ֵΪ::HI_SVR_PLAYER_STATE_STOP��\n
ֹͣ���ź󣬿��Ե���::HI_SVR_PLAYER_Play�ӿ����²���

\param[in] hPlayer player handle. CNcomment:���������

\retval ::HI_SUCCESS The operation is valid. CNcomment:�Ϸ�����
\retval ::HI_FAILURE The operation is invalid. CNcomment:�Ƿ�����

\see \n
None.
*/
HI_S32 HI_SVR_PLAYER_Stop(HI_HANDLE hPlayer);

/**
\brief Pause playing.  CNcomment:��ͣ����
\attention \n
This interface is an asynchronous interface. It can be called to pause playing but cannot be called during fast forward and rewind. Values returned by this interface cannot be used to check whether playing is paused successfully. \n
The player notifies the APP of pause success or failure by using the ::HI_SVR_PLAYER_EVENT_STATE event. The event parameter value is ::HI_SVR_PLAYER_STATE_PAUSE. \n
The ::HI_SVR_PLAYER_Resume interface can be called to resume playing after a successful pause.
CNcomment:�첽�ӿڣ����Ź����п��Ե��øýӿ���ͣ���ţ����������״̬�²��ܵ��øýӿڣ�����ͨ���ýӿڷ���ֵ���жϲ�����\n
�Ƿ���ͣ�ɹ�����������ͨ��::HI_SVR_PLAYER_EVENT_STATE�¼�֪ͨappֹͣ�ɹ����¼�����ֵΪ::HI_SVR_PLAYER_STATE_PAUSE��\n
��ͣ�ɹ��󣬿��Ե���::HI_SVR_PLAYER_Resume��������

\param[in] hPlayer player handle. CNcomment:���������

\retval ::HI_SUCCESS The operation is valid. CNcomment:�Ϸ�����
\retval ::HI_FAILURE The operation is invalid. CNcomment:�Ƿ�����

\see \n
None.
*/
HI_S32 HI_SVR_PLAYER_Pause(HI_HANDLE hPlayer);

/**
\brief Resume playing. CNcomment:�ָ�����
\attention \n
This interface is an asynchronous interface. It can be called to resume playing during pause, fast forward, and rewind. Values returned by this interface cannot be used to check whether playing is resumed successfully. \n
The player notifies the APP of resumption success or failure by using the ::HI_SVR_PLAYER_EVENT_STATE event. The event parameter value is ::HI_SVR_PLAYER_STATE_PLAY.

CNcomment:�첽�ӿڣ���ͣ�����������״̬�£����øýӿڻָ��������ţ�����ͨ���ýӿڷ���ֵ���жϲ������Ƿ�ָ�����\n
��������ͨ��::HI_SVR_PLAYER_EVENT_STATE�¼�֪ͨapp�ָ��ɹ����¼�����ֵΪ::HI_SVR_PLAYER_STATE_PLAY��

\param[in] hPlayer player handle. CNcomment:���������

\retval ::HI_SUCCESS The operation is valid. CNcomment:�Ϸ�����
\retval ::HI_FAILURE The operation is invalid. CNcomment:�Ƿ�����

\see \n
None.
*/
HI_S32 HI_SVR_PLAYER_Resume(HI_HANDLE hPlayer);

/**
\brief Fast forward and rewind. CNcomment:���������
\attention \n
This interface is an asynchronous interface. It must be called in play, pause or tplay status. The negative value of s32Speed indicates rewind and the
positive value of s32Speed indicates fast forward. Values returned by the interface cannot be used to check whether the fast forward or rewind is successful.
\n
The player notifies the APP of the fast forward or rewind status by using the ::HI_SVR_PLAYER_EVENT_STATE event. \n
The event parameter value is ::HI_SVR_PLAYER_STATE_FORWARD or ::HI_SVR_PLAYER_STATE_BACKWARD. The ::HI_SVR_PLAYER_Resume interface can be called to \n
resume normal playing after fast forward or rewind.
CNcomment:�첽�ӿڣ��ú������벥�š���ͣ����/����״̬�µ��ã�s32SpeedΪ����ʾ���ˣ�Ϊ����ʾ���������ͨ���ýӿ�\n
����ֵ���ж��Ƿ�������˳ɹ�����������ͨ��::HI_SVR_PLAYER_EVENT_STATE�¼�֪ͨapp���¼�����ֵΪ\n
::HI_SVR_PLAYER_STATE_FORWARD��::HI_SVR_PLAYER_STATE_BACKWARD����������˺�ͨ������::HI_SVR_PLAYER_Resume\n
�ӿڻָ��������š�

\param[in] hPlayer player handle. CNcomment:���������
\param[in] s32Speed playing speed. The value is HI_SVR_PLAYER_PLAY_SPEED_E. CNcomment:���ű���,ֵΪHI_SVR_PLAYER_PLAY_SPEED_E

\retval ::HI_SUCCESS The operation is valid. CNcomment:�Ϸ�����
\retval ::HI_FAILURE The operation is invalid. CNcomment:�Ƿ�����

\see \n
None.
*/
HI_S32 HI_SVR_PLAYER_TPlay(HI_HANDLE hPlayer, HI_S32 s32Speed);

/**
\brief Seek to a specified location for playing. CNcomment:����ָ��λ�ò���
\attention \n
This interface is an asynchronous interface. It must be called after the ::HI_SVR_PLAYER_SetMedia interface is called. It can be called to jump to a specified time point for playing.
CNcomment:�첽�ӿڣ��ýӿڱ����ڵ���::HI_SVR_PLAYER_SetMedia�ӿں���ã����š�ֹͣ״̬�£����øýӿ�����ָ��ʱ��㲥��

\param[in] hPlayer player handle. CNcomment:���������
\param[in] s64TimeInMs seeking time. The unit is ms. CNcomment:seekʱ�䣬��λms

\retval ::HI_SUCCESS The operation is successful. CNcomment:Seek�ɹ�
\retval ::HI_FAILURE The operation is invalid. CNcomment:�Ƿ�����

\see \n
None.
*/
HI_S32 HI_SVR_PLAYER_Seek(HI_HANDLE hPlayer, HI_S64 s64TimeInMs);

/**
\brief Seek to a specified location for playing. CNcomment:����ָ��λ�ò���
\attention \n
This interface is an asynchronous interface. It must be called after the ::HI_SVR_PLAYER_SetMedia interface is called. It can be called to jump to a specified time point for playing.
CNcomment:�첽�ӿڣ��ýӿڱ����ڵ���::HI_SVR_PLAYER_SetMedia�ӿں���ã����š�ֹͣ״̬�£����øýӿ�����ָ��λ�ò���

\param[in] hPlayer player handle. CNcomment:���������
\param[in] s64Offset offset(in bytes) of the starting position. CNcomment:����ļ���ʼλ�õ��ֽ�ƫ��

\retval ::HI_SUCCESS The operation is successful. CNcomment:Seek�ɹ�
\retval ::HI_FAILURE The operation is invalid. CNcomment:�Ƿ�����

\see \n
None.
*/
HI_S32 HI_SVR_PLAYER_SeekPos(HI_HANDLE hPlayer, HI_S64 s64Offset);

/**
\brief Invoke operation. Application expand operation. The player only transparently transmits the data. CNcomment:invoke������app��չʹ�ã�player����͸��
\attention \n
It must be called after the ::HI_SVR_PLAYER_SetMedia interface is called.
CNcomment:�ýӿڱ����ڵ���::HI_SVR_PLAYER_SetMedia�ӿں����

\param[in] hPlayer player handle. CNcomment:���������
\param[in] u32InvokeId Operation ID of application expanding. The value cannot be within the range of HI_FORMAT_INVOKE_ID_E. CNcomment:app��չ�Ĳ���id��ֵ������HI_FORMAT_INVOKE_ID_Eö�ٷ�Χ��
\param[in/out] pArg param of the invoking operation. CNcomment:��������

\retval ::HI_SUCCESS The operation is successful. CNcomment:�����ɹ�
\retval ::HI_FAILURE The operation is invalid. CNcomment:�Ƿ�����

\see \n
None.
*/
HI_S32 HI_SVR_PLAYER_Invoke(HI_HANDLE hPlayer, HI_U32 u32InvokeId, HI_VOID *pArg);

/**
\brief Obtain information about the current open file, such as the file size, playing duration, file bit rate, video width, video height, coding format, frame rate, video bit rate, audio encoding, and audio bit rate.
CNcomment:��ȡ��ǰ���ļ���Ϣ�����ļ���С���ļ�����ʱ�������ʵȣ���Ƶ���ߣ������ʽ��֡�ʡ����ʣ���Ƶ���롢���ʵ�
\attention \n
This interface must be called after the ::HI_SVR_PLAYER_SetMedia interface is called.
CNcomment:�ú��������ڵ���::HI_SVR_PLAYER_SetMedia�ӿں����

\param[in] hPlayer player handle. CNcomment:���������
\param[out] pstruInfo obtained file information. CNcomment:��ȡ���ļ���Ϣ

\retval ::HI_SUCCESS The file information is obtained successfully. CNcomment:��ȡ���ļ���Ϣ
\retval ::HI_FAILURE The file information fails to be obtained. CNcomment:��ȡ�ļ���Ϣʧ��

\see \n
None.
*/
HI_S32 HI_SVR_PLAYER_GetFileInfo(HI_HANDLE hPlayer, HI_FORMAT_FILE_INFO_S *pstruInfo);

/**
\brief Obtain the player information, such as the current playing status, playing progress, and elapsed time. CNcomment:��ȡ��������Ϣ���統ǰ����״̬�����Ž��ȡ��Ѳ���ʱ���
\attention \n
This interface must be called after the ::HI_SVR_PLAYER_Create interface is called. The playing progress and elapsed time are valid only after the ::HI_SVR_PLAYER_Play interface is called.
CNcomment:�ú��������ڵ���::HI_SVR_PLAYER_Create�ӿں���ã����Ž��ȡ��Ѳ���ʱ��ֻ���ڵ���::HI_SVR_PLAYER_Play�ӿں���Ч
::HI_SVR_PLAYER_SetMediaδ����ǰ��������øýӿ�

\param[in] hPlayer player handle. CNcomment:���������
\param[out] pstruInfo player information. CNcomment:��������Ϣ

\retval ::HI_SUCCESS The player information is obtained successfully. CNcomment:��������Ϣ��ȡ�ɹ�
\retval ::HI_FAILURE The player information fails to be obtained. CNcomment:��������Ϣ��ȡʧ��

\see \n
None.
*/
HI_S32 HI_SVR_PLAYER_GetPlayerInfo(HI_HANDLE hPlayer, HI_SVR_PLAYER_INFO_S *pstruInfo);

/**
\brief Register a file and subtitle DEMUX. The DEMUX must be a DLL and is implemented based on the specifications of the hi_svr_format.h header file.  CNcomment:ע���ļ���������Ļ�������������������Ƕ�̬�⣬�Ұ�hi_svr_format.hͷ�ļ��淶ʵ��
\attention \n
This interface must be called after the ::HI_SVR_PLAYER_Init interface is called.
DDLs such as libformat.so are stored in /usr/lib. The interface is called as follows:
HI_SVR_PLAYER_RegisterDynamic(HI_SVR_PLAYER_DLL_PARSER, "libformat.so")
CNcomment:�ýӿڱ����ڵ���::HI_SVR_PLAYER_Init�ӿں����
��̬�����/usr/libĿ¼�£���libformat.so������øýӿڷ�ʽΪ:
HI_SVR_PLAYER_RegisterDynamic(HI_SVR_PLAYER_DLL_PARSER, "libformat.so")

\param[in] eDllType DDL type. CNcomment:��̬������
\param[in] dllName DDL name. CNcomment:��̬������

\retval ::HI_SUCCESS The DDL is registered successfully.  CNcomment:��̬��ע��ɹ�
\retval ::HI_FAILURE The DDL fails to be registered. CNcomment:ע��ʧ��

\see \n
None.
*/
HI_S32 HI_SVR_PLAYER_RegisterDynamic(HI_SVR_PLAYER_DLLTYPE_E eDllType, const HI_CHAR *dllName);

/**
\brief Remove all DDLs for parsing files and subtitles. CNcomment:ж�������ļ���������Ļ������̬��
\attention \n
This interface must be called after the ::HI_SVR_PLAYER_Init interface is called.
CNcomment:�ýӿڱ����ڵ���::HI_SVR_PLAYER_Init�ӿں����

\param[in] eDllType DDL type. CNcomment:��̬������
\param[in] dllName DDL name. It is reserved. CNcomment:��̬�����ƣ������������ݲ�ʹ��

\retval ::HI_SUCCESS The DDLs are removed successfully. CNcomment:ж�سɹ�
\retval ::HI_FAILURE The DDLs fail to be removed. CNcomment:ж��ʧ��

\see \n
None.
*/
HI_S32 HI_SVR_PLAYER_UnRegisterDynamic(HI_SVR_PLAYER_DLLTYPE_E eDllType, const HI_CHAR *dllName);

/**
\brief Enable or disable the player dbg. CNcomment:��/�ر�player dbg��Ϣ
\attention \n
This interface can be called in any state.
CNcomment:�ýӿڿ������κ�״̬����

\param[in] bEnable enable value. HI_TRUE: enables the player log. HI_FALSE: disables the player log. CNcomment:ʹ��ֵ��HI_TRUE:��player��־��Ϣ��HI_FALSE:�ر���־��Ϣ

\retval :: None.

\see \n
None.
*/
HI_VOID HI_SVR_PLAYER_EnableDbg(HI_BOOL bEnable);

/**
\brief Obtain the player version. CNcomment:��ȡ�������汾��
\attention \n
This interface can be called in any state.
CNcomment:�ýӿڿ������κ�״̬����

\param[out] pstVersion player version. CNcomment:�������汾��

\retval :: None.

\see \n
None.
*/
HI_VOID HI_SVR_PLAYER_GetVersion(HI_FORMAT_LIB_VERSION_S *pstVersion);


/**
\brief Create a player for meta retriever. CNcomment:����һ������������ý��Ԫ���ݻ�ȡ
\attention \n
This interface must be called after initialization. Only one player can be created. \n
CNcomment:�ýӿڱ����ڳ�ʼ������ã�֧����ഴ��һ��������\n
\param[in] pstruParam player initialization attribute. The available Dmx ID and port ID must be specified for TS streams to be played. CNcomment:��������ʼ�����ԣ�����ts��������ָ�����õ�dmx id��port id
\param[in] pstruMedia media file information. Only absolute path is supported.\n
\param[out] phPlayer handle of the created player. CNcomment:�����Ĳ��������

\retval ::HI_SUCCESS A player is created successfully and the player handle is valid. CNcomment:�����ɹ��������������Ч
\retval ::HI_FAILURE A player fails to be created. The parameters are invalid or resources are insufficient. CNcomment:����ʧ�ܣ������Ƿ�����Դ����

\see \n
None.
*/
HI_S32 HI_SVR_PLAYER_CreateMetaRetriever(const HI_SVR_PLAYER_PARAM_S *pstruParam,
                                            HI_SVR_PLAYER_MEDIA_S *pstruMedia, HI_HANDLE *phPlayer);

/**
\brief Destroy a player instance for meta retriever. CNcomment:����һ������ý��Ԫ���ݻ�ȡ�Ĳ�����ʵ��
\attention \n
The HI_S32 HI_SVR_PLAYER_Destroy interface is called to destroy the player resource after the ::HI_SVR_PLAYER_CreateMetaRetriever interface is called to create a player.
CNcomment:����::HI_SVR_PLAYER_CreateMetaRetriever�����������󣬵��øýӿ����ٲ�������Դ
\param[in] hPlayer player handle. CNcomment:���������

\retval ::HI_SUCCESS The player is released successfully. CNcomment:�������ͷųɹ�
\retval ::HI_FAILURE The operation is invalid. CNcomment:�Ƿ�����

\see \n
None.
*/
HI_S32 HI_SVR_PLAYER_DestroyMetaRetriever(HI_HANDLE hPlayer);

/** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_SVR_PLAYER_H__ */

