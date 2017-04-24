/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : dtvav.h

    Description : DTV播放管理
 
    Others:      
    
    History: 	1. 2004-3-15 yzb create
    			2. 2011-11-11 xyan Modify

*******************************************************************************/
#ifndef _DTVAV_H
#define _DTVAV_H

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "driver.h"
#include "digitv.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	External Function Prototype
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Global functions
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_GetPlayState
  Description     : 获取DTV状态
  Input           : 
  Outut           :
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int DTV_GetPlayState(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_SetPlayState
  Description     : 设置DTV状态
  Input           : 
  Outut           :
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_SetPlayState(int Status);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_SetAudioVolume
  Description     : 设置DTV当前播放频道音量
  Input           : 音量输入值[0,63]
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_SetAudioVolume(unsigned char ucVolume);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_SetAudioTrack
  Description     : 设置DTV当前播放频道声道
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_SetAudioTrack(unsigned char ucTrack);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_GetVideoMode
  Description     : 获取DTV当前视频模式(NTSC/PAL)
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int DTV_GetVideoMode(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_SetVideoMode
  Description     : 设置DTV当前视频模式(NTSC/PAL)
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_SetVideoMode(int Mode);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_PlayServiceAV
  Description     : DTV播放Service音视频接口
  Input           : 要播放的Service
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_PlayServiceAV(STBDATAServiceInfo_t *pSTBServiceInfo);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_EnablePlay
  Description     : DTV开始播放当前节目
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_EnablePlay(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_DisablePlay
  Description     : DTV停止播放当前节目
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_DisablePlay(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_ShowMpeg
  Description     : DTV显示I帧
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DTV_ShowMpeg(unsigned char *pData,unsigned int uiDataLength);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_ShowBootMpeg
  Description     : DTV显示开机画面
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_ShowBootMpeg(void); 

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_ShowGBMpeg
  Description     : DTV显示广播背景 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_ShowGBMpeg(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_IsChannelLock
  Description     : 判断频道是否加锁
  Input           : 频道号
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DTV_IsChannelLock(unsigned short usChannelNum);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_ChannelUnLock
  Description     : 解锁DTV当前频道
  Input           : 频道号
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_ChannelUnLock(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_ChangeTVChannel
  Description     : DTV电视模式下切台处理
  Input           : usChannelNum为频道号
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_ChangeTVChannel(unsigned short usChannelNum);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_ChangeRadioChannel
  Description     : DTV广播模式下切台处理
  Input           : usChannelNum为频道号
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_ChangeRadioChannel(unsigned short usChannelNum);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_PlaySpecifyProgramme
  Description     : DTV播放指定节目
  Input           : 
  Outut           : TRUE:播放成功;FALSE:播放失败
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DTV_PlaySpecifyProgramme(STBDATADeliveryInfo_t *pDelivery,STBDATAServiceInfo_t *pSTBServiceInfo);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_Standby
  Description     : DTV待机处理
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_Standby(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_UnStandby
  Description     : DTV待机唤醒处理
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_UnStandby(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_NewFrameToBeDispWithFrameRateChange
  Description     : 制式变化处理函数
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_NewFrameToBeDispWithFrameRateChange(unsigned int FrameRate);

#endif

