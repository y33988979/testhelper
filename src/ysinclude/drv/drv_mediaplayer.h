#ifndef _DRV_MEDIAPLAYER_H_
#define _DRV_MEDIAPLAYER_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <drv_os.h>
#include <drv_filter.h>
#include <drv_filterinj.h>
#include <drv_init.h>
#include <drv_tuner.h>
#include <drv_panel.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <drv_video.h>
#include <drv_audio.h>
#include <drv_display.h>

#include <signal.h>

typedef enum DRV_MediaPlayStatus_e
{
	DRV_MEDIAPLAYER_RUNNING,
	DRV_MEDIAPLAYER_STOPPED,
	DRV_MEDIAPLAYER_PAUSED,
	DRV_MEDIAPLAYER_BACKWARD,
	DRV_MEDIAPLAYER_FORWARD,
	DRV_MEDIAPLAYER_SLOW,
	DRV_MEDIAPLAYER_NONE
}DRV_MediaPlayStatus_t;

typedef enum MediaPlayer_Evt_s
{
	REACH_END_OF_PLAYBACK_EVT,
	REACH_BEGIN_OF_PLAYBACK_EVT
}MediaPlayer_Evt;

typedef void (*DRV_MediaPlayerCallback)(MediaPlayer_Evt event);

DRV_ErrCode DRV_MediaPlayerOpen(void);
DRV_ErrCode DRV_MediaPlayerClose(void);
DRV_ErrCode DRV_MediaPlayerPause(void);
DRV_ErrCode DRV_MediaPlayerResume(void);
DRV_ErrCode DRV_MediaPlayerStart(char * FilePath);
DRV_ErrCode DRV_MediaPlayerStop(void);
DRV_ErrCode DRV_MediaPlayerSetSpeed(DRV_S32 speed);
DRV_ErrCode DRV_MediaPlayerSeek(DRV_S32 TimeInms);
DRV_ErrCode DRV_MediaPlayerGetTime(DRV_U32* TatalTimeInms_p,DRV_U32* BeginTimeInms_p,DRV_U32* EndTimeInms_p,DRV_U32* CurrentTimeInms_p);
DRV_ErrCode DRV_MediaPlayerGetStatus(DRV_MediaPlayStatus_t* Status);
DRV_ErrCode DRV_MediaPlayerGetSpeed(DRV_S32* Speed);

#ifdef __cplusplus
}
#endif
#endif




