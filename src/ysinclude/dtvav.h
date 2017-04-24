/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : dtvav.h

    Description : DTV���Ź���
 
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
  Description     : ��ȡDTV״̬
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
  Description     : ����DTV״̬
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
  Description     : ����DTV��ǰ����Ƶ������
  Input           : ��������ֵ[0,63]
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
  Description     : ����DTV��ǰ����Ƶ������
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
  Description     : ��ȡDTV��ǰ��Ƶģʽ(NTSC/PAL)
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
  Description     : ����DTV��ǰ��Ƶģʽ(NTSC/PAL)
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
  Description     : DTV����Service����Ƶ�ӿ�
  Input           : Ҫ���ŵ�Service
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
  Description     : DTV��ʼ���ŵ�ǰ��Ŀ
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
  Description     : DTVֹͣ���ŵ�ǰ��Ŀ
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
  Description     : DTV��ʾI֡
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
  Description     : DTV��ʾ��������
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
  Description     : DTV��ʾ�㲥���� 
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
  Description     : �ж�Ƶ���Ƿ����
  Input           : Ƶ����
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
  Description     : ����DTV��ǰƵ��
  Input           : Ƶ����
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
  Description     : DTV����ģʽ����̨����
  Input           : usChannelNumΪƵ����
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
  Description     : DTV�㲥ģʽ����̨����
  Input           : usChannelNumΪƵ����
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
  Description     : DTV����ָ����Ŀ
  Input           : 
  Outut           : TRUE:���ųɹ�;FALSE:����ʧ��
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
  Description     : DTV��������
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
  Description     : DTV�������Ѵ���
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
  Description     : ��ʽ�仯������
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_NewFrameToBeDispWithFrameRateChange(unsigned int FrameRate);

#endif

