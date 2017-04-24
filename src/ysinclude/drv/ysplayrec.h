#ifndef _YSPLAYREC_H_
#define _YSPLAYREC_H_

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif
#include <drv_typedef.h>
#include "stddefs.h" 
#include "ysav.h"
#include "yshiplayer.h"
#include "hi_unf_pvr.h"
#define DRV_PLAYREC_MAX_LENGTH_NAME         (255)                      /* Maximum length size of the filename       */
#define DRV_PLAYREC_MAX_PIDS				(16)

typedef enum
{
    SOURCE_FROM_FILE,
    SOURCE_FROM_TUNER,
    SOURCE_FROM_NET
}PlayRec_SourceType;

/* Stream types */
/* ------------ */
typedef enum 
{
    DRV_PLAYREC_STREAMTYPE_INVALID =  0, /* None     : Invalid type                         */
    DRV_PLAYREC_STREAMTYPE_MP1V    =  1, /* Video    : MPEG1                                */
    DRV_PLAYREC_STREAMTYPE_MP2V    =  2, /* Video    : MPEG2                                */
    DRV_PLAYREC_STREAMTYPE_MP4V    =  3, /* Video    : MPEG4                                */
    DRV_PLAYREC_STREAMTYPE_H264V   =  4, /* Video    : H264                                 */
    DRV_PLAYREC_STREAMTYPE_VC1     =  5, /* Video    : Decode Simple/Main/Advanced profile  */
    DRV_PLAYREC_STREAMTYPE_MMV     =  6, /* Video    : Multimedia content                   */
    DRV_PLAYREC_STREAMTYPE_AVS     =  7, /* Video    : AVS Video format                     */
    DRV_PLAYREC_STREAMTYPE_MP1A    = 30, /* Audio    : MPEG 1 Layer I                       */
    DRV_PLAYREC_STREAMTYPE_MP2A    = 31, /* Audio    : MPEG 1 Layer II                      */
    DRV_PLAYREC_STREAMTYPE_MP3A    = 32, /* Audio    : Multimedia content                   */
    DRV_PLAYREC_STREAMTYPE_MP4A    = 33, /* Audio    : like HEAAC,Decoder LOAS / LATM - AAC */
    DRV_PLAYREC_STREAMTYPE_AC3     = 34, /* Audio    : AC3                                  */
    DRV_PLAYREC_STREAMTYPE_AAC     = 35, /* Audio    : AAC                                  */
    DRV_PLAYREC_STREAMTYPE_HE_AAC  = 36, /* Audio    : HE_AAC                                  */
    DRV_PLAYREC_STREAMTYPE_PMT     = 60, /* pmt      : pmt pid                         */
    DRV_PLAYREC_STREAMTYPE_PCR     = 61, /* Synchro  : PCR pid                              */
    DRV_PLAYREC_STREAMTYPE_OTHER   = 62  /* Misc     : Non identified pid                   */
} DRV_PlayRecStreamType_t;

typedef enum DRV_PlayRecPlaybackStatus_e
{
	DRV_PLAYREC_PLAYBACK_RUNNING,
	DRV_PLAYREC_PLAYBACK_STOPPED,
	DRV_PLAYREC_PLAYBACK_PAUSED,
	DRV_PLAYREC_PLAYBACK_BACKWARD,/*快退*/
	DRV_PLAYREC_PLAYBACK_FORWARD,/*快进*/
	DRV_PLAYREC_PLAYBACK_SLOWFORWARD,/*慢进*/
	DRV_PLAYREC_PLAYBACK_NONE
}DRV_PlayRecPlaybackStatus_t;

typedef enum DRV_PlayRecRecordStatus_e
{
	DRV_PLAYREC_RECORD_RECORDING,
	DRV_PLAYREC_RECORD_STOPPED,
	DRV_PLAYREC_RECORD_PAUSED,
	DRV_PLAYREC_RECORD_NONE
}DRV_PlayRecRecordStatus_t;

typedef enum DRV_PlayRecMode_e
{
	DRV_PLAYREC_MODE_LOCALPLAY,/*hiplayer*/
	DRV_PLAYREC_MODE_PVRREC,/*pvr的录制*/
	DRV_PLAYREC_MODE_PVRPLAY,/*播放pvr的录制的流*/
	DRV_PLAYREC_MODE_PVRTIMESHIFT,/*pvr的时移*/
	DRV_PLAYREC_MODE_NONE
}DRV_PlayRecMode_t;

typedef struct
{
    DRV_PlayRecStreamType_t Type;
    U32                  Pid;
} DRV_PlayRecStreamData_t;


typedef struct DRV_PlayRecStartPlayParams_s
{
	U8                    		PidsNum;                                   /* Number of pids [1,2,3]	*/
	DRV_PlayRecStreamData_t 	Pids[DRV_PLAYREC_MAX_PIDS];                /* Pid list				*/

    char*						SrcPath;    
    DRV_PlayRecCallback 		Callback;
    void*						CallbackData;
}DRV_PlayRecStartPlayParams_t;


typedef struct DRV_PlayRecStartRecordParams_s
{
	U8                    		PidsNum;                                   /* Number of pids [1,2,3]	*/
	DRV_PlayRecStreamData_t 	Pids[DRV_PLAYREC_MAX_PIDS];                    /* Pid list				*/

    DRV_PlayRecCallback 		Callback;
    void*						CallbackData;

    char*						SrcPath;
    char*						DstFilePath;
    U32                			TimeLimitInMinute;         /* Minute:0 means no limit */
    U32                         SizeLimitInMbyte;          /* Mbyte:0 means no limit*/
}DRV_PlayRecStartRecordParams_t;


/*PLAYBACK*/
DRV_ErrCode DRV_PlayRecStartPlay(U32 PlaybackId,DRV_PlayRecStartPlayParams_t *Params_p);
DRV_ErrCode DRV_PlayRecStopPlay(U32 PlaybackId);
DRV_ErrCode DRV_PlayRecPausePlay(U32 PlaybackId);
DRV_ErrCode DRV_PlayRecResumePlay(U32 PlaybackId);
DRV_ErrCode DRV_PlayRecSetPlaySpeed(U32 PlaybackId,S32 Speed);
DRV_ErrCode DRV_PlayRecGetPlaySpeed(U32 PlaybackId,S32* Speed);
DRV_ErrCode DRV_PlayRecSeekPlay(U32 PlaybackId,S32 TimeInms);
DRV_ErrCode DRV_PlayRecGetPlayTime(U32 PlaybackId,U32* TatalTimeInms_p,U32* BeginTimeInms_p,U32* EndTimeInms_p,U32* CurrentTimeInms_p);
DRV_ErrCode DRV_PlayRecGetPlayStatus(U32 PlaybackId,DRV_PlayRecPlaybackStatus_t* Status);
DRV_ErrCode DRV_PlayRecPlaybackEnableCallBack(DRV_PlayRecCallback Callback_f);
DRV_ErrCode DRV_PlayRecPlaybackDisableCallBack(void);

/*RECORD*/
DRV_ErrCode DRV_PlayRecStartRecord(U32 RecordId,DRV_PlayRecStartRecordParams_t *Params_p);
DRV_ErrCode DRV_PlayRecStopRecord(U32 RecordId);
DRV_ErrCode DRV_PlayRecGetRecordTime(U32 RecordId,U32* TimeInms_p);
DRV_ErrCode DRV_PlayRecGetRecordStatus(U32 RecordId,DRV_PlayRecRecordStatus_t* Status);
DRV_ErrCode DRV_PlayRecPauseRecord(U32 RecordId);
DRV_ErrCode DRV_PlayRecResumeRecord(U32 RecordId);
DRV_ErrCode DRV_PlayRecRecordEnableCallBack(U32 RecordId);

/* ========================================================================
   Name:        DRV_PlayRecPausePlay2Record
   Description: Pause a live program and start a record
   ======================================================================== */
DRV_ErrCode DRV_PlayRecPausePlay2Record(U32 PlaybackId,U32 RecordId,DRV_PlayRecStartRecordParams_t* Params_p,DRV_PlayRecCallback	PlaybackCallback);


#ifdef __cplusplus
}
#endif
#endif










