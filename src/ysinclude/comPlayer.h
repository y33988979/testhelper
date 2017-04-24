/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : complayer.h

    Description : 

    Others:      

    History:    1. llh creat 2004-3-29


*******************************************************************************/
#ifndef _COMPLAYER_H_
#define _COMPLAYER_H_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "osddrv.h"
#include "midware.h"
#include "driver.h"
#include "ysplayrec.h"
#include "ysvfs.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#define MAX_PLAY_PATH_LEN 				1024
#define REACH_PLAY_BEGIN                990
#define REACH_PLAY_END                  991
#define REACH_PLAY_READ_ERR             992
#define REACH_PLAY_WRITE_ERR            993
#define USB_PLUG_OUT_ERR                994
#define FRESH_PROCESS_TIME              1000
#define HIDE_PLAYER                     1002
#define LRC_MAX_CONTNET      			128

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
typedef enum DRV_PlayerStatus_e
{
	DRV_PLAYREC_RUNNING,
	DRV_PLAYREC_STOPPED,
	DRV_PLAYREC_PAUSED,
	DRV_PLAYREC_BACKWARD,
	DRV_PLAYREC_FORWARD,
	DRV_PLAYREC_NONE
}DRV_PlayRecStatus_t;

typedef enum
{    
    MODE_NONE,
    MODE_RECORD_PLAY_AUDIO,//硬盘音频播放
    MODE_RECORD_PLAY_VIDEO,//硬盘视频播放
    MODE_TIME_SHIFT_PLAY,  //时移播放
    MODE_PLAY_MAX_COUNT
}COM_PLAYER_MODE_e;

typedef enum
{    
    MP3_NONE,
    MP3_BEFORE,//上一首
    MP3_NEXT,  //下一首
    MP3_COUNT
}MP3_PROCESS_t;

typedef enum
{    
    LRC_CMD_START,
    LRC_CMD_RUN,
    LRC_CMD_STOP,
    LRC_CMD_COUNT
}COM_LRC_CMD_e;

typedef enum
{    
    USBAPP_PLAYMODE_ORDER=0,
    USBAPP_PLAYMODE_SINGLE=1,
    USBAPP_PLAYMODE_RAMDOM=2,
    USBAPP_PLAYMODE_CIRCLE=3,
    USBAPP_PLAYMODE_COUNT
}COM_MP3_PLAY_MODE_e;


typedef enum 
{ 
    PLAY_HALF,
    PLAY_NORMAL,
    PLAY_2,           
    PLAY_4,           
    PLAY_6,       
    PLAY_8,
    PLAY_10,
    PLAY_12,    
    PLAY_16,
    PLAY_BACK_HALF,
    PLAY_BACK_NORMAL,
    PLAY_BACK_2,
    PLAY_BACK_4,
    PLAY_BACK_6,
    PLAY_BACK_8,
    PLAY_BACK_10,
    PLAY_BACK_12,
    PLAY_BACK_16  
}Play_Speed;

typedef struct
{
   char playPrgName[MAX_PLAY_PATH_LEN];//显示用节目名称(可能是路径和文件名)
   char playUnitName[MAX_PLAY_PATH_LEN];//逻辑用节目名称(可能是路径和文件名)
   COM_PLAYER_MODE_e VMode;//播放模式(播放录制节目，播放时移节目)   
   unsigned short volume;//播放音量   
   unsigned char  soundType;//声道类型   
   unsigned short unChanneNo;//频道号
   BOOL IsMute;//是否静音
}Player_Struct_t;

typedef struct
{
   char playPrgName[MAX_PLAY_PATH_LEN];//显示用节目名称(可能是路径和文件名)
   char playUnitName[MAX_PLAY_PATH_LEN];//逻辑用节目名称(可能是路径和文件名)
   COM_MP3_PLAY_MODE_e AMode;//播放模式(顺序播放,随机播放)   
   unsigned short volume;//播放音量   
   unsigned char soundType;//声道类型   
   BOOL IsMute;//是否静音
   BOOL IsList;//是否播放列表
}MP3Player_Struct_t;

typedef struct
{
	BOOL			IsRunning;
	STOSD_Bitmap_t	*stBitmap;
	OSD_Region_t 	*hOSDHandle; 
	SizeRect_t		stMsgRect;
	unsigned int	width;//消息字符串bmp的长度。以象素为单位
	unsigned int	height;
	int				siStart;
    char			*content;
}comMP3Player_LRCScrollInfo_t;

typedef struct
{
    char musicName[256];
    char singer[256];
    char spical[256];
    char LRCEditor[256];
    int offset;
}comMP3Player_LRCInfo_t;

typedef struct
{
	int startTime;
	char content[LRC_MAX_CONTNET];
	char* pos;
	int lineNum; 
}comMp3Player_LRCLine_t;

/*----------------------------------------------------------------------------
 *	Global functions
 *----------------------------------------------------------------------------*/

/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia  
  Function Name   : comPlay_GetPlayState
  Description     : 获取播放器是否显示标示
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL comPlay_GetPlayState(void);

/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia  
  Function Name   : comPlay_SetPlayState
  Description     : 设置播放器是否显示标示
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void comPlay_SetPlayState(BOOL state);

/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia  
  Function Name   : comPlay_GetProcessState
  Description     : 获取是否要显示绝对时间标示
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL comPlay_GetProcessState(void);

/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia  
  Function Name   : comPlay_SetProcessState
  Description     : 设置是否要显示绝对时间标示
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void comPlay_SetProcessState(BOOL state);

/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia  
  Function Name   : comPlay_Set_PlayerParam
  Description     : 设置影音播放器的参数,返回值非0时，就不能够启动播放器任务
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int comPlay_Set_PlayerParam(COM_PLAYER_MODE_e mode,COM_MP3_PLAY_MODE_e amode,char* name,char* Unitname,unsigned  short volume,unsigned  char  soundType,BOOL IsMute,unsigned short unChannelNo,BOOL IsList);

/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia  
  Function Name   : comPlay_Free_PlayerParam
  Description     : 释放影音播放器结构
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void comPlay_Free_PlayerParam(COM_PLAYER_MODE_e mode);


/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia  
  Function Name   : play_init_Osd
  Description     : 设置播放器显示AppRegion
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void play_init_AppOsd (void);

/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia  
  Function Name   : play_init_DtvOsd
  Description     :  设置播放器显示DtvRegion
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void play_init_DtvOsd (void);
/*****************************************************************************
  Date&Time       : 2011-12-24
  Author          :  wpeng  
  Function Name   : play_show_mpegbg
  Description     : 播放器背景
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void play_show_mpegbg(void);
/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia
  Function Name   : play_show_bg
  Description     : 显示播放器背景
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          : 单独调用这种函数,需要确定player是否消隐,要不只是画出一块
*****************************************************************************/
void play_show_bg(void);

/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia  
  Function Name   : play_show_status
  Description     : 显示播放状态，以及速度
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 单独调用这种函数,需要确定player是否消隐,要不只是画出一块
*****************************************************************************/ 
void play_show_status(DRV_PlayRecPlaybackStatus_t palymode,S32_YS speed);
/*****************************************************************************
  Date&Time       : 2011-12-24
  Author          :  wpeng
  Function Name   : play_show_Mp3Process
  Description     : 显示mp3播放切换
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          : 
*****************************************************************************/
void play_show_Mp3Process(MP3_PROCESS_t mp3process);

/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia  
  Function Name   : play_show_process
  Description     : 显示进度条和已播放时长(格式 hh:mm)
  Input           : startTime---表示开始播放时系统时间，相对1900年的秒数
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 对于播放进度的计算，可能需要修改
*****************************************************************************/ 
void play_show_process(COM_PLAYER_MODE_e mode,int curTime,int totalTime);
/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia  
  Function Name   : play_show_wave
  Description     : MP3播放器显示波形
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void play_show_wave(void);

/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia  
  Function Name   : play_show_helpinfo
  Description     : 影音播放器显示帮助信息
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void play_show_helpinfo(COM_PLAYER_MODE_e mode);

/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia  
  Function Name   : play_hide_helpinfo
  Description     : 影音播放器隐藏帮助信息
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void play_hide_helpinfo(void);

/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia  
  Function Name   : play_VolumeShow
  Description     : 显示音量
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void play_VolumeShow(int Volume, BOOL Mute);

/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia  
  Function Name   : play_SoundmodeShow
  Description     : 显示声道
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void play_SoundmodeShow(char *StringDisplay);


/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  wpeng
  Function Name   : play_showPFinfo
  Description     : 显示EPG信息
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
void play_showPFinfo(STBDATAEvent_t *pSTBEventinfo);

/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia  
  Function Name   : play_show_player
  Description     : 显示影音播放器
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void play_show_player(COM_PLAYER_MODE_e mode,DRV_PlayRecPlaybackStatus_t palymode,unsigned short unChanneNo,char *programname,int starttime,int entime,                                             
                        unsigned int totallen,int nowpalytime,S32_YS speed,unsigned short volume,unsigned char IsMute,unsigned char soundMode,unsigned char AMode);

/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia  
  Function Name   : play_show_playmode
  Description     : 播放器显示播放方式
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void play_show_playmode(unsigned char play_Mode);

/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia  
  Function Name   : play_hide_player
  Description     : 隐藏播放器
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void play_hide_player(void);

/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia  
  Function Name   : comPlayer_SetAudio
  Description     : 设置影音播放器音量和声道
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void comPlayer_SetAudio(void);

/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia  
  Function Name   : comMP3Player_SetAudio
  Description     : 设置MP3播放器音量和声道
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void comMP3Player_SetAudio(void);

/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia  
  Function Name   : comMP3Player_StopLRCTask
  Description     : 停止歌词显示任务
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void comMP3Player_StopLRCTask(void);
/*****************************************************************************
  Date&Time    :    2011-12-24
  Author          :    wpeng
  Function Name   : comMP3Player_PauseLRCTask
  Description     : 
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void comMP3Player_PauseLRCTask(void);
/*****************************************************************************
  Date&Time    :    2011-12-24
  Author          :    wpeng
  Function Name   : comMP3Player_ReStartLRCTask
  Description     : 
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void comMP3Player_ReStartLRCTask(void);

/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia  
  Function Name   : comMP3Player_ReadNextLine
  Description     : 
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
char* comMP3Player_ReadNextLine(char* pCur,char* line);

/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia  
  Function Name   : comMP3Player_GetTextLineNum
  Description     : 
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int comMP3Player_GetTextLineNum(char* text);

/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia  
  Function Name   : comMP3Player_LRCScrollTaskInit
  Description     : LRC歌词显示任务初始化
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int comMP3Player_LRCScrollTaskInit(APP_AppInfo_t *comMP3Player_LRCScrollInfo);


#endif

