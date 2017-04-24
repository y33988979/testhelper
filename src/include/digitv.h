/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : digitv.h

    Description : ���ֵ��ӵ�Ӧ��
 
    Others:      
    
    History: 	1. 2004-3-15 yzb Create
    			2. 2011-11-2 xyan Modify

*******************************************************************************/
#ifndef _DTV_H
#define _DTV_H

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "stbtype.h"        
#include "driver.h"     	
#include "midware.h"        
//#include "midcas.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
/*DTV��ʾʱ�䶨��*/
#define DTV_BANNER_SHOW_TIME    				5000   /*Banner��ʾʱ��*/
#define DTV_SHOW_CHNL_NUM_TIME  				5000   /*Ƶ������ʾʱ��*/
#define DTV_SHOW_VOLUME_TIME    				3000   /*��������ʾʱ��*/
#define DTV_RECOVER_VOLUME_TIME    				1000   /*DTV��̨�ָ�����ʱ��*/
#define TIME_UPDATE_TIME						5000   /*Ӧ�ø��¼��ʱ��*/

/*DTV��������*/
#define MIN_VOLUME_SET							63	//��Ӧ������С����
#ifdef _VOLUME_32_
#define DTV_MAX_VOLUME							32
#else
#define DTV_MAX_VOLUME							63
#endif

#define DTV_INVALID_TS         					0xFFFF
#define DTV_INVALID_CHANNEL    					0xFFFF 
#define DTV_SystemTimeAdjuested					TDTReceived

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
typedef enum 
{
	DTV_MSG_PROCESSED,      /*��Ϣ�ѱ�����*/
    DTV_MSG_NOT_PROCESSED	/*��Ϣδ������*/
}DTV_Msg_Process_Status;

typedef enum
{
	DTV_PLAY_DTV = 1,		/*���Ӳ���״̬*/
	DTV_PLAY_RADIO = 2,		/*�㲥����״̬*/
	DTV_PLAY_COUNT
}DTV_Play_state;

typedef enum
{
	DTV_BANNER_SHOW,		/*Banner��ʾ*/
	DTV_VOLUME_SHOW,		/*��������ʾ*/
	DTV_BANNER_HIDE,		/*Banner����*/
	DTV_BANNER_STATUS_COUNT
}DTV_BannerStatus_e;

typedef enum
{
	NO_AD,					/*�޹��*/
    BMP_AD,					/*BMP���*/
    GIF_AD,					/*GIF���*/
    JPG_AD					/*JPEG���*/
}DTV_BannerAdType_e;

typedef enum 
{
	DTV_CA_MSG_INVALID = CA_MSGCODE_BASE,
	DTV_CA_SHOW_BUY,		/*��ʾCA��ʾ*/
	DTV_CA_HIDE_BUY,		/*���CA��ʾ*/
	DTV_CA_SHOW_FINGER,		/*��ʾCAָ��*/
	DTV_CA_HIDE_FINGER,		/*���CAָ��*/
	DTV_CA_SHOW_TOPOSD,		/*��ʾCA�Ϲ���*/
	DTV_CA_HIDE_TOPOSD,		/*���CA�Ϲ���*/
	DTV_CA_SHOW_BOTOSD,		/*��ʾCA�¹���*/
	DTV_CA_HIDE_BOTOSD,		/*���CA�¹���*/
	DTV_CA_SHOW_MAIL,		/*��ʾCA�ʼ�*/
	DTV_CA_EXHAUST_MAIL,	/*��˸CA�ʼ�*/
	DTV_CA_HIDE_MAIL,		/*���CA�ʼ�*/
	DTV_CA_SHOW_DETITLE,	/*��ʾCA����Ȩ*/
	DTV_CA_EXHAUST_DETITLE,	/*��˸CA����Ȩ*/
	DTV_CA_HIDE_DETITLE,	/*���CA����Ȩ*/
	DTV_CA_SHOW_FEEDING,	/*��ʾCAι��*/
	DTV_CA_HIDE_FEEDING,	/*���CAι��*/
	DTV_CA_SHOW_IPPV,		/*��ʾCA IPPV����*/
	DTV_CA_HIDE_IPPV,		/*���CA IPPV����*/
	DTV_CA_SHOW_UPGRADE,	/*��ʾCA������Ϣ*/
	DTV_CA_HIDE_UPGRADE,	/*���CA������Ϣ*/
	DTV_CA_MSG_COUNT  
}DTV_CAMsg_Type;

typedef enum 
{
	DTV_USER_MSG_INVALID = DTV_USER_BASE,
	DTV_USER_CHANGE_CHNL,	/*��ͨ��̨��Ϣ*/
	DTV_USER_CHANGE_TO_TV,	/*�л���TVģʽ��Ϣ*/
	DTV_USER_CHANGE_TO_RADIO,/*�л���Radioģʽ��Ϣ*/
	DTV_USER_RESET_CAINFO,	/*�������õ�ǰ���Ž�ĿCA������Ϣ*/
	DTV_USER_REPLAY_AV,		/*����������ǰ��Ŀ������Ϣ*/
	DTV_USER_REPLAY,		/*���²��ŵ�ǰ��Ŀ��Ϣ*/
	DTV_USER_REFRESH_PLAY,	/*ˢ�µ�ǰƵ�������²�����Ϣ*/
	DTV_USER_CLEAR_BANNER,	/*���Banner��ʾ��Ϣ*/
	DTV_USER_CLEAR_CHNL_NUM,/*���Ƶ������ʾ��Ϣ*/
	DTV_USER_RECOVER_VOLUME,/*����������Ϣ*/
	DTV_USER_COUNT
}DTV_UserMsg_e;

typedef enum 
{  
	DTV_START_FROM_POWER_OFF = 0,	/*DTV�Ӵ���������*/
	DTV_START_FROM_APP = 0x01,		/*DTV��Ӧ���˳�������*/
	DTV_START_FROM_COUNT
}DTV_Start_From_e;

 /*DTVģ��Ĵ�����*/
typedef enum 
{
	DTV_SUCCESS,		/*DTV�����ɹ�*/
	DTV_FAILED,			/*DTV����ʧ��*/
	DTV_INVALID_CHNL = DTV_ERROR_BASE,	/*Ƶ������Ч*/
	DTV_CHANGE_TS_FAILED,/*��Ƶ��ʧ��*/
	DTV_PLAYAV_FAILED,	/*����ʧ��*/
	DTV_START_FAILED,	/*����DTVʧ��*/
	DTV_SUSPEND_FAILED,	/*����DTVʧ��*/
	DTV_ERROR_COUNT
}DTV_ErrorCode_e;

typedef struct 
{
	unsigned short usCurrentTS;			/*DTV��ǰ��Ŀ��TsNo*/
	unsigned short usCurrentChannel;	/*DTV��ǰ����Ƶ����*/	
	unsigned short usCurrentRadioChannel;/*DTV��ǰ�㲥Ƶ����*/	
	unsigned short usLastChannel;		/*DTV��һ������Ƶ����*/	
	unsigned short usLastRadioChannel;	/*DTV��һ���㲥Ƶ����*/	
	unsigned char ucCurrentVolume;		/*DTV��ǰ��Ŀ����ֵ*/
	unsigned char ucCurrentTrack;		/*DTV��ǰ��Ŀ����ֵ*/

    unsigned char ucDigKeyInputMode;	/*DTVƵ��������ģʽ*/
    unsigned char ucDigKeyInputedNum;	/*�����������*/
    unsigned char aucDigKeyInput[3];	/*�������������ֵ*/

	BOOL bMute;				/*DTV������ʾ*/
	BOOL bRegionShow;		/*DTVRegion��ʾ��ʾ*/
	BOOL bIdle;				/*DTV�Ƿ�����ʾ(TRUEΪ����,FALSEΪ�ǹ���)*/
	BOOL bDTVPlay;			/*DTV�Ƿ񲥷ű�ʾ*/
	BOOL bChnlNumShow;		/*DTVƵ������ʾ��ʾ*/
	BOOL bTvOrRadioRestart;	/*ǿ����̨��ʾ*/
	
	DTV_BannerStatus_e BannerStatus;	/*DTV Banner����ʾ״̬*/
} DTV_State_t;

/*----------------------------------------------------------------------------
 *	External variable
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Global functions
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_IsDTVStart
  Description     : ��ȡDTV�Ƿ�ʼ���б�ʾ
  Input           : 
  Outut           : TRUEΪDTV�Ѿ�����,FALSEΪDTV��δ����
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DTV_IsDTVStart(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_GetCurrentChannel
  Description     : ��ȡDTV��ǰƵ����
  Input           : 
  Outut           : DTV��ǰ���ŵ�Ƶ����
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned short DTV_GetCurrentChannel(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_GetLastChannel
  Description     : ��ȡDTVǰһ�����ŵ�Ƶ����
  Input           : 
  Outut           : DTVǰһ�����ŵ�Ƶ����
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned short DTV_GetLastChannel(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_GetCurrentVolume
  Description     : ��ȡDTV��ǰ����Ƶ������ֵ
  Input           : 
  Outut           : DTV��ǰ����Ƶ��������ֵ
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char DTV_GetCurrentVolume(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_SetCurrentVolume
  Description     : ����DTV��ǰ����Ƶ��������ֵ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_SetCurrentVolume(unsigned char ucVol);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_GetCurrentTrack
  Description     : ��ȡDTV��ǰ����Ƶ��������ֵ
  Input           : 
  Outut           : DTV��ǰƵ�����ŵ�����ֵ
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char DTV_GetCurrentTrack(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_SetCurrentTrack
  Description     : ����DTV��ǰ����Ƶ��������ֵ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_SetCurrentTrack(unsigned char ucTrack);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_GetCurrentTsNo
  Description     : ��ȡDTV��ǰ����Ƶ����TsNo
  Input           : 
  Outut           : DTV��ǰƵ����TsNo
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned short DTV_GetCurrentTsNo(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_SetCurrentTsNo
  Description     : ����DTV��ǰ����Ƶ����TsNo
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_SetCurrentTsNo(unsigned short usTsNo);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_GetDTVMute
  Description     : ��ȡDTV��ǰ������ʶ
  Input           : 
  Outut           : DTV��ǰ������ʶ
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DTV_GetDTVMute(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_SetDTVMute
  Description     : ����DTV��ǰ������ʶ
  Input           : ������ʶ
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_SetDTVMute(BOOL bMute);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_GetDTVSuspendFlag
  Description     : ��ȡDTV��ǰ�Ƿ�����ʾ
  Input           : 
  Outut           : DTV��ǰ�����ʾ
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DTV_GetDTVSuspendFlag(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_SetDTVSuspendFlag
  Description     : ����DTV��ǰ�Ƿ�����ʾ
  Input           : 
  Outut           : DTV��ǰ�����ʾ
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_SetDTVSuspendFlag(BOOL bFlag);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_GetModeToChange
  Description     : ��ȡDTV�����л���ģʽ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned int DTV_GetModeToChange(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_SetModeToChange
  Description     : ����DTV�����л���ģʽ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_SetModeToChange(unsigned int uiMode);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_GetChnlNumToChange
  Description     : ��ȡDTV�����л���Ƶ����
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned short DTV_GetChnlNumToChange(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_SetChnlNumToChange
  Description     : ����DTV�����л���Ƶ����
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_SetChnlNumToChange(unsigned short usChnlNum);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_SendDTVMsg
  Description     : ��DTV������Ϣ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned int DTV_SendDTVMsg (MSG_MsgInfo_t *pstMsgInfo);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_DelTimeMsg
  Description     : ɾ��DTV��ʱ��Ϣ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : DTV��ʱ��Ϣʹ��MSG_CLASS_TIME����
*****************************************************************************/ 
void DTV_DelTimeMsg(unsigned int uiMsgCode);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_AddTimeMsg
  Description     : ��DTV���Ͷ�ʱ��Ϣ
  Input           : uiMsgCodeΪ��Ϣ��,uiTimeΪ���뷢��ʱ��
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : DTV��ʱ��Ϣʹ��MSG_CLASS_TIME����
*****************************************************************************/ 
void DTV_AddTimeMsg(unsigned int uiMsgCode,unsigned int uiTime);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_SendStartDTVMsg
  Description     : ��DTV��������DTV����Ϣ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DTV_SendStartDTVMsg(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_SendSuspendDTVMsg
  Description     : ��DTV���͹���DTV����Ϣ
  Input           : Ҫ�౳��I֡������ָ������ݴ�С
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DTV_SendSuspendDTVMsg(unsigned char *pFrameData,unsigned int FrameSize);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_ChangeChannelMsg
  Description     : ��DTV������̨��Ϣ
  Input           : PlayStatusΪҪ�л���DTV״̬,ChnlNoΪҪ�е�Ƶ����
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned int DTV_ChangeChannelMsg(int PlayStatus,unsigned short ChnlNo);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_ChangeChannelTmMsg
  Description     : ��DTV���Ͷ�ʱ��̨��Ϣ
  Input           : PlayStatusΪҪ�л���DTV״̬,ChnlNoΪҪ�е�Ƶ����
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_ChangeChannelTmMsg(int PlayStatus,unsigned short ChnlNo);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_ChangeToTvMsg
  Description     : ��DTV�����л���TV����Ϣ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned int DTV_ChangeToTvMsg(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_ChangeToRadioMsg
  Description     : ��DTV�����л���Radio����Ϣ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned int DTV_ChangeToRadioMsg(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_ForceStartDTV
  Description     : ǿ������DTV
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_ForceStartDTV(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_ForceSuspendDTV
  Description     : ǿ�ƹ���DTV
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_ForceSuspendDTV(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_ManagerTaskInit
  Description     : DTV���������ʼ��
  Input           : 
  Outut           : TRUEΪ��ʼ���ɹ�;FALSEΪ��ʼ��ʧ��
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL DTV_ManagerTaskInit(void);

#endif 

