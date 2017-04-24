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
    MODE_RECORD_PLAY_AUDIO,//Ӳ����Ƶ����
    MODE_RECORD_PLAY_VIDEO,//Ӳ����Ƶ����
    MODE_TIME_SHIFT_PLAY,  //ʱ�Ʋ���
    MODE_PLAY_MAX_COUNT
}COM_PLAYER_MODE_e;

typedef enum
{    
    MP3_NONE,
    MP3_BEFORE,//��һ��
    MP3_NEXT,  //��һ��
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
   char playPrgName[MAX_PLAY_PATH_LEN];//��ʾ�ý�Ŀ����(������·�����ļ���)
   char playUnitName[MAX_PLAY_PATH_LEN];//�߼��ý�Ŀ����(������·�����ļ���)
   COM_PLAYER_MODE_e VMode;//����ģʽ(����¼�ƽ�Ŀ������ʱ�ƽ�Ŀ)   
   unsigned short volume;//��������   
   unsigned char  soundType;//��������   
   unsigned short unChanneNo;//Ƶ����
   BOOL IsMute;//�Ƿ���
}Player_Struct_t;

typedef struct
{
   char playPrgName[MAX_PLAY_PATH_LEN];//��ʾ�ý�Ŀ����(������·�����ļ���)
   char playUnitName[MAX_PLAY_PATH_LEN];//�߼��ý�Ŀ����(������·�����ļ���)
   COM_MP3_PLAY_MODE_e AMode;//����ģʽ(˳�򲥷�,�������)   
   unsigned short volume;//��������   
   unsigned char soundType;//��������   
   BOOL IsMute;//�Ƿ���
   BOOL IsList;//�Ƿ񲥷��б�
}MP3Player_Struct_t;

typedef struct
{
	BOOL			IsRunning;
	STOSD_Bitmap_t	*stBitmap;
	OSD_Region_t 	*hOSDHandle; 
	SizeRect_t		stMsgRect;
	unsigned int	width;//��Ϣ�ַ���bmp�ĳ��ȡ�������Ϊ��λ
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
  Description     : ��ȡ�������Ƿ���ʾ��ʾ
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
  Description     : ���ò������Ƿ���ʾ��ʾ
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
  Description     : ��ȡ�Ƿ�Ҫ��ʾ����ʱ���ʾ
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
  Description     : �����Ƿ�Ҫ��ʾ����ʱ���ʾ
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
  Description     : ����Ӱ���������Ĳ���,����ֵ��0ʱ���Ͳ��ܹ���������������
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
  Description     : �ͷ�Ӱ���������ṹ
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
  Description     : ���ò�������ʾAppRegion
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
  Description     :  ���ò�������ʾDtvRegion
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
  Description     : ����������
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
  Description     : ��ʾ����������
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          : �����������ֺ���,��Ҫȷ��player�Ƿ�����,Ҫ��ֻ�ǻ���һ��
*****************************************************************************/
void play_show_bg(void);

/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia  
  Function Name   : play_show_status
  Description     : ��ʾ����״̬���Լ��ٶ�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : �����������ֺ���,��Ҫȷ��player�Ƿ�����,Ҫ��ֻ�ǻ���һ��
*****************************************************************************/ 
void play_show_status(DRV_PlayRecPlaybackStatus_t palymode,S32_YS speed);
/*****************************************************************************
  Date&Time       : 2011-12-24
  Author          :  wpeng
  Function Name   : play_show_Mp3Process
  Description     : ��ʾmp3�����л�
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
  Description     : ��ʾ���������Ѳ���ʱ��(��ʽ hh:mm)
  Input           : startTime---��ʾ��ʼ����ʱϵͳʱ�䣬���1900�������
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : ���ڲ��Ž��ȵļ��㣬������Ҫ�޸�
*****************************************************************************/ 
void play_show_process(COM_PLAYER_MODE_e mode,int curTime,int totalTime);
/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  zcxia  
  Function Name   : play_show_wave
  Description     : MP3��������ʾ����
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
  Description     : Ӱ����������ʾ������Ϣ
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
  Description     : Ӱ�����������ذ�����Ϣ
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
  Description     : ��ʾ����
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
  Description     : ��ʾ����
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
  Description     : ��ʾEPG��Ϣ
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
  Description     : ��ʾӰ��������
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
  Description     : ��������ʾ���ŷ�ʽ
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
  Description     : ���ز�����
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
  Description     : ����Ӱ������������������
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
  Description     : ����MP3����������������
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
  Description     : ֹͣ�����ʾ����
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
  Description     : LRC�����ʾ�����ʼ��
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int comMP3Player_LRCScrollTaskInit(APP_AppInfo_t *comMP3Player_LRCScrollInfo);


#endif

