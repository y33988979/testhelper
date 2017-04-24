#ifndef _YSFFMPEG_H_
#define _YSFFMPEG_H_

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif
#include "ysav.h"

typedef enum DRV_HiPlayerStatus_e
{
	DRV_HIPLAYER_CLOSE,
	DRV_HIPLAYER_OPEN,
	DRV_HIPLAYER_START,
	DRV_HIPLAYER_STOP,
	DRV_HIPLAYER_PAUSE,
	DRV_HIPLAYER_SEEK,
	DRV_HIPLAYER_BW,
	DRV_HIPLAYER_FF
}DRV_HiPlayerStatus;

DRV_ErrCode DRV_ffmpeg_setup(void);
DRV_ErrCode DRV_ffmpeg_term(void);
DRV_ErrCode DRV_ffmpeg_start(U8* FileName);
DRV_ErrCode DRV_ffmpeg_stop();
DRV_ErrCode DRV_ffmpeg_seek(S64 TimeMs);
DRV_ErrCode DRV_ffmpeg_BackForward(U32 Speed);
DRV_ErrCode DRV_ffmpeg_SetSpeed(U32 Speed);
DRV_ErrCode DRV_ffmpeg_Pause(void);
DRV_ErrCode DRV_ffmpeg_Resume(void);
DRV_ErrCode DRV_ffmpeg_SetSync(DRV_AvSynMode SynMode);
DRV_ErrCode DRV_ffmpeg_Get_DurationTime(HI_U32 * Time);
DRV_ErrCode DRV_ffmpeg_Get_NowTime(HI_U64 *Time);
DRV_ErrCode DRV_ffmpeg_GetSpeed(U32 *Speed);
U32 DRV_ffmpeg_GetSpeedtime(U32 * ops,U32 *time);
DRV_ErrCode DRV_ffmpegEnableCallback(DRV_PlayRecCallback Callback_f);
DRV_ErrCode DRV_ffmpegDisableCallback(void);
void DRV_ffmpeg_SetUsbFlag(int flag);
#ifdef __cplusplus
}
#endif
#endif



