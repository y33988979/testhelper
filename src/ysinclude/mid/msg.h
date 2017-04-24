/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : msg.h

    Description : ��Ϣ����

    Others:      

    History:  1. yzb creat 2005-10-18
			  2. xyan Modify 2011-11-16

*******************************************************************************/
#ifndef MSG_H_
#define MSG_H_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "stbtype.h"
#include "driver.h"
#include "app.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#define NULL_MSG				0x00000000	/*invalid message code*/
#define MASK_DRIVER				0x01000000	/*the message caused by low level driver �ɵײ������������Ϣ*/
#define MASK_APP				0x02000000	/*the message send by application controller ����Ӧ�ù���������Ϣ*/
#define MASK_CA					0x03000000	/*the message send by CA ����CA����Ϣ*/
#define MASK_DVB				0x04000000	/*the message send by DVB ����DVB����Ϣ*/
#define MASK_DBC				0x05000000	/*the message send by DBC ����DBC����Ϣ*/
#define MASK_USER				0x06000000	/*the message send by User Define �����û��Զ������Ϣ*/
#define MASK_KEY				0x08000000	/*the message caused by key action ���ڰ��������������Ϣ*/
#define MASK_ERROR				0x09000000	/*the message send by Error ����Ӧ���ϱ��Ĵ�����Ϣ*/

/*��������ϢMsgCode����*/
#define DRV_MSGCODE_BASE		MASK_DRIVER	/*��������ϢMsgCode*/
#define DRV_CARRIER_DATA_LOCK	(MASK_DRIVER+0x01)/*������סƵ��Ϣ*/
#define DRV_CARRIER_DATA_LOST	(MASK_DRIVER+0x02)/*����ʧ����Ϣ*/
#define DRV_CHANGE_TO_PAL		(MASK_DRIVER+0x03)/*����ת��ΪPAL����Ϣ*/
#define DRV_CHANGE_TO_NTSC		(MASK_DRIVER+0x04)/*����ת��ΪNTSC����Ϣ*/
#define DRV_ETH_LINK			(MASK_DRIVER+0x05)/*�������߲����Ϣ*/
#define DRV_ETH_UNLINK			(MASK_DRIVER+0x06)/*�������߰γ���Ϣ*/ 
#define DRV_SMARTCARD_INSERT	(MASK_DRIVER+0x07)/*�������ܿ�������Ϣ*/
#define DRV_SMARTCARD_REMOVE	(MASK_DRIVER+0x08)/*�������ܿ��γ���Ϣ*/ 

/*Ӧ�ü���ϢMsgCode����*/
#define APP_MSGCODE_BASE		MASK_APP		/*Ӧ������ϢMsgCode*/
#define APP_QUERY				(MASK_APP+0x01)	/*ѯ��Ӧ��״̬��Ϣ*/
#define APP_RESPONSE			(MASK_APP+0x02)	/*Ӧ��ѯ�ʻظ���Ϣ*/
#define APP_START 				(MASK_APP+0x03)	/*����Ӧ����Ϣ*/
#define APP_QUIT				(MASK_APP+0x04)	/*�˳�Ӧ����Ϣ*/
#define APP_SUSPEND				(MASK_APP+0x05)	/*����Ӧ����Ϣ*/
#define APP_RESUME				(MASK_APP+0x06)	/*�ָ�Ӧ����Ϣ*/
#define APP_SHOW				(MASK_APP+0x07)	/*��ʾӦ����Ϣ*/
#define APP_HIDE				(MASK_APP+0x08)	/*����Ӧ����Ϣ*/
#define APP_WINFOCUS			(MASK_APP+0x09)	/*Ӧ�û�ý�����Ϣ*/
#define APP_LOSEFOCUS			(MASK_APP+0x0a)	/*Ӧ��ʧȥ������Ϣ*/
#define MSGBOX_CLOSE			(MASK_APP+0x0b) /*close message box*/
#define APP_MSGMNG_OK			(MASK_APP+0x0c) /*Msg�����������������Ϣ*/
#define APP_EXIT				(MASK_APP+0x0d) /*Ӧ��ɾ����Ϣ*/
#define APP_DTV_START			(MASK_APP+0x0e) /*DTV������Ϣ*/
#define APP_DTV_SUSPEND			(MASK_APP+0x0f) /*DTV������Ϣ*/
#define APP_START_APP			(MASK_APP+0x10) /*Ӧ�ÿ�ʼ������Ϣ*/

/*CA����ϢMsgCode����*/
#define CA_MSGCODE_BASE			MASK_CA		/*CA����ϢMsgCode*/

/*DVB����ϢMsgCode����*/
#define DVB_MSGCODE_BASE		MASK_DVB	/*DVB����ϢMsgCode*/

/*DBC����ϢMsgCode����*/
#define DBC_MSGCODE_BASE		MASK_DBC	/*DBC����ϢMsgCode*/

/*��������ϢMsgCode����*/
#define BUTTON_PRESS			MASK_KEY		/*button pressed*/
#define ID_OK					(MASK_KEY+0x01)	/*OK*/ 
#define ID_MUTE					(MASK_KEY+0x02)	/*shut down the audio*/
#define ID_ONOFF				(MASK_KEY+0x03)	/*power on or power off*/
#define ID_DOUBLE_DIGIT 		(MASK_KEY+0x04)	/*switching between single or double*/
#define ID_LAST_CHANNEL	    	(MASK_KEY+0x05)	/*last channel */
#define ID_CHNSET      			(MASK_KEY+0x06)	/* Interactive TV */
#define ID_RECORD		    	(MASK_KEY+0x07)	/*record */
#define ID_UP					(MASK_KEY+0x08)	/*Move Up or to the previous one*/
#define ID_DOWN					(MASK_KEY+0x09)	/*Move down or to the next*/
#define ID_LEFT					(MASK_KEY+0x0a)	/*Move to the left or the parent*/
#define ID_RIGHT				(MASK_KEY+0x0b)	/*Move to the right or the son*/
#define ID_MENU					(MASK_KEY+0x0c)	/*Menu*/
#define ID_EXIT					(MASK_KEY+0x0d)	/*Exit the application*/
#define ID_INFO					(MASK_KEY+0x0e)	/*Show more information about the current program*/

#define ID_UP_REPEAT	    	(MASK_KEY+0x20)
#define ID_DOWN_REPEAT			(MASK_KEY+0x21)
#define ID_LEFT_REPEAT			(MASK_KEY+0x22)
#define ID_RIGHT_REPEAT 		(MASK_KEY+0x23)
#define ID_BROWSER_EXIT 		(MASK_KEY+0x24)
#define ID_F3					(MASK_KEY+0x25)
#define ID_F4					(MASK_KEY+0x26)
#define ID_F5					(MASK_KEY+0x27)
#define ID_F6					(MASK_KEY+0x28)
#define ID_F7					(MASK_KEY+0x29)
#define ID_F8					(MASK_KEY+0x2a)
#define ID_F9					(MASK_KEY+0x2b)
#define ID_F10					(MASK_KEY+0x2c)

#define ID_0					(MASK_KEY+0x30)	/*0*/
#define ID_1					(MASK_KEY+0x31)	/*1*/
#define ID_2					(MASK_KEY+0x32)	/*2*/
#define ID_3					(MASK_KEY+0x33)	/*3*/
#define ID_4					(MASK_KEY+0x34)	/*4*/
#define ID_5					(MASK_KEY+0x35)	/*5*/
#define ID_6					(MASK_KEY+0x36)	/*6*/
#define ID_7					(MASK_KEY+0x37)	/*7*/
#define ID_8					(MASK_KEY+0x38)	/*8*/
#define ID_9					(MASK_KEY+0x39)	/*9*/

/*���̻�����ɫ���ܼ�*/
#define ID_RED					(MASK_KEY+0x50)	/*RED*/
#define ID_GREEN				(MASK_KEY+0x51)	/*GREEN*/
#define ID_YELLOW	    		(MASK_KEY+0x52)	/*YELLOW*/
#define ID_BLUE     			(MASK_KEY+0x53)	/*BLUE*/

#define ID_SAVE					(MASK_KEY+0x60)	/*Save*/
#define ID_CLEAR				(MASK_KEY+0x61)	/*Clear*/
#define ID_LANGUAGE				(MASK_KEY+0x62)	/*Switch language*/
#define	ID_SYS					(MASK_KEY+0x63)	/*Switch to the system setting interface*/
#define ID_ZOOM					(MASK_KEY+0x64)	/*zoom the view of the display*/
#define ID_SEND					(MASK_KEY+0x65)	/*send the data*/
#define ID_BACK					(MASK_KEY+0x66)	/* Back to last channel */
#define ID_TAB					(MASK_KEY+0x67)	/*Switch to the next in sequence*/
#define ID_MENUEXIT 			(MASK_KEY+0x68)	/*Panel Menu Button*/
#define ID_EPG 					(MASK_KEY+0x69)
#define ID_HELP 				(MASK_KEY+0x6a)
#define ID_EMAIL 				(MASK_KEY+0x6b)
#define ID_PAUSE 				(MASK_KEY+0x6c)
#define ID_STOCK 				(MASK_KEY+0x6d)
#define ID_CHNLLIST 			(MASK_KEY+0x6e)
#define ID_NVOD 				(MASK_KEY+0x6f)
#define ID_DBC 					(MASK_KEY+0x70) 
#define ID_PREV 				(MASK_KEY+0x71)
#define ID_NEXT 				(MASK_KEY+0x72)
#define ID_TV_RADIO 			(MASK_KEY+0x73)
#define ID_FAVRIT 				(MASK_KEY+0x74)
#define ID_WEB 					(MASK_KEY+0x75)
#define ID_GAME 				(MASK_KEY+0x76)

#define ID_CHNL_UP				ID_UP			/*Move up the channel selection*/
#define ID_CHNL_DOWN			ID_DOWN			/*Move down the channel selection*/
#define ID_VOL_UP				ID_RIGHT		/*raise the vol*/
#define ID_VOL_DOWN				ID_LEFT			/*lower the col*/
#define ID_CHNL_UP_REPEAT		ID_UP_REPEAT
#define ID_CHNL_DOWN_REPEAT		ID_DOWN_REPEAT
#define ID_VOL_UP_REPEAT		ID_RIGHT_REPEAT
#define ID_VOL_DOWN_REPEAT		ID_LEFT_REPEAT

#define ID_PAGEUP				(MASK_KEY+0x100)/*page up*/
#define ID_PAGEDOWN				(MASK_KEY+0x101)/*page down*/
#define ID_PAGEUP_REPEAT		(MASK_KEY+0x102)
#define ID_PAGEDOWN_REPEAT		(MASK_KEY+0x103)
#define ID_CHNL_INC				(MASK_KEY+0x104)
#define ID_CHNL_DEC				(MASK_KEY+0x105)
#define ID_VOL_INC				(MASK_KEY+0x106)
#define ID_VOL_DEC				(MASK_KEY+0x107)

#define ID_INVALID				(MASK_KEY+0x0ffffffe)
#define ID_CANCEL				(MASK_KEY+0x0fffffff)

/*�û�����ϢMsgCode����*/
#define USER_MSGCODE_BASE		MASK_USER		/*USER����ϢMsgCode*/
#define FOCUS_MOVE				(MASK_USER+0x01)/*Switch focus among controls in same app*/
#define USER_DIALOG_SHOW		(MASK_USER+0x02)/*��ʾ�û�Dlg��Ϣ*/
#define USER_DIALOG_RETURN		(MASK_USER+0x03)/*�û�Dlg�ظ���Ϣ*/
#define USER_APP_START	    	(MASK_USER+0x04)/*�û�������Ӧ����Ϣ*/
#define USER_TIME_UPDATE	 	(MASK_USER+0x05)/*�û���ʱ������Ϣ*/


//�����ģ��ʹ�õ��û�����ϢMsgCode��ʼֵ
#define DTV_USER_BASE			(USER_MSGCODE_BASE+0x010000)/*DTVģ���û��Զ�����Ϣ��ʼֵ*/
#define CA_USER_BASE			(USER_MSGCODE_BASE+0x020000)/*CAģ���û��Զ�����Ϣ��ʼֵ*/
#define DVB_USER_BASE			(USER_MSGCODE_BASE+0x030000)/*DVBģ���û��Զ�����Ϣ��ʼֵ*/
#define DBC_USER_BASE			(USER_MSGCODE_BASE+0x040000)/*DBCģ���û��Զ�����Ϣ��ʼֵ*/
#define USB_USER_BASE			(USER_MSGCODE_BASE+0x050000)/*USBģ���û��Զ�����Ϣ��ʼֵ*/
#define EPG_USER_BASE			(USER_MSGCODE_BASE+0x060000)/*EPGģ���û��Զ�����Ϣ��ʼֵ*/
#define NVOD_USER_BASE			(USER_MSGCODE_BASE+0x070000)/*NVODģ���û��Զ�����Ϣ��ʼֵ*/
#define PROGMNG_USER_BASE		(USER_MSGCODE_BASE+0x080000)/*Ƶ������ģ���û��Զ�����Ϣ��ʼֵ*/
#define STBSET_USER_BASE		(USER_MSGCODE_BASE+0x090000)/*ϵͳ����ģ���û��Զ�����Ϣ��ʼֵ*/
#define GAME_USER_BASE			(USER_MSGCODE_BASE+0x0a0000)/*��Ϸģ���û��Զ�����Ϣ��ʼֵ*/
#define PVR_USER_BASE			(USER_MSGCODE_BASE+0x0b0000)/*PVRģ���û��Զ�����Ϣ��ʼֵ*/
#define MOSAIC_USER_BASE		(USER_MSGCODE_BASE+0x0c0000)/*MOSAICģ���û��Զ�����Ϣ��ʼֵ*/
#define CHSRCH_USER_BASE		(USER_MSGCODE_BASE+0x0d0000)/*����ģ���û��Զ�����Ϣ��ʼֵ*/
#define FAVORITE_USER_BASE		(USER_MSGCODE_BASE+0x0e0000)/*ϲ�������û��Զ�����Ϣ��ʼֵ*/
#define BOOKMNG_USER_BASE		(USER_MSGCODE_BASE+0x0F0000)/*ԤԼ�����û��Զ�����Ϣ��ʼֵ*/

/*������ϢMsgCode����*/
#define ERROR_MSGCODE_BASE		MASK_ERROR					/*ERROR����ϢMsgCode*/
#define INVALID_QUEUE			(ERROR_MSGCODE_BASE+0x01)	/*invalid message queue*/
#define INVALID_MESSAGE			(ERROR_MSGCODE_BASE+0x02)	/*invalid message*/
#define EMPTY_QUEUE				(ERROR_MSGCODE_BASE+0x03)	/*message queue is empty */

//�����ģ��ʹ�õĴ�����ϢMsgCode��ʼֵ
#define DTV_ERROR_BASE			(ERROR_MSGCODE_BASE+0x010000)/*DTVģ���û��Զ�����Ϣ��ʼֵ*/
#define CA_ERROR_BASE			(ERROR_MSGCODE_BASE+0x020000)/*CAģ���û��Զ�����Ϣ��ʼֵ*/
#define DVB_ERROR_BASE			(ERROR_MSGCODE_BASE+0x030000)/*DVBģ���û��Զ�����Ϣ��ʼֵ*/
#define DBC_ERROR_BASE			(ERROR_MSGCODE_BASE+0x040000)/*DBCģ���û��Զ�����Ϣ��ʼֵ*/
#define USB_ERROR_BASE			(ERROR_MSGCODE_BASE+0x050000)/*USBģ���û��Զ�����Ϣ��ʼֵ*/
#define EPG_ERROR_BASE			(ERROR_MSGCODE_BASE+0x060000)/*EPGģ���û��Զ�����Ϣ��ʼֵ*/
#define NVOD_ERROR_BASE			(ERROR_MSGCODE_BASE+0x070000)/*NVODģ���û��Զ�����Ϣ��ʼֵ*/
#define PROGMNG_ERROR_BASE		(ERROR_MSGCODE_BASE+0x080000)/*Ƶ������ģ���û��Զ�����Ϣ��ʼֵ*/
#define STBSET_ERROR_BASE		(ERROR_MSGCODE_BASE+0x090000)/*ϵͳ����ģ���û��Զ�����Ϣ��ʼֵ*/
#define GAME_ERROR_BASE			(ERROR_MSGCODE_BASE+0x0a0000)/*��Ϸģ���û��Զ�����Ϣ��ʼֵ*/
#define PVR_ERROR_BASE			(ERROR_MSGCODE_BASE+0x0b0000)/*PVRģ���û��Զ�����Ϣ��ʼֵ*/
#define MOSAIC_ERROR_BASE		(ERROR_MSGCODE_BASE+0x0c0000)/*MOSAICģ���û��Զ�����Ϣ��ʼֵ*/
#define CHSRCH_ERROR_BASE		(ERROR_MSGCODE_BASE+0x0d0000)/*����ģ���û��Զ�����Ϣ��ʼֵ*/

/*----------------------------------------------------------------------------
 *	Enum & Macro 
 *----------------------------------------------------------------------------*/
typedef enum
{
	MSG_CLASS_INVALID = 0,			/*��Ч��Ϣ��*/
	MSG_CLASS_POWER = 0x01,			/*��������Ϣ��*/
	MSG_CLASS_DRV = 0x02,			/*��������Ϣ��*/
	MSG_CLASS_APP = 0x03,			/*Ӧ�ü���Ϣ��*/
	MSG_CLASS_CA = 0x04,			/*CA����Ϣ��*/
	MSG_CLASS_DVB = 0x05,			/*DVB����Ϣ��*/
	MSG_CLASS_DBC = 0x06,			/*DBC����Ϣ��*/
	MSG_CLASS_TIME = 0x07,			/*��ʱ����Ϣ��*/ 
	MSG_CLASS_KEY = 0x08,			/*��������Ϣ��*/
	MSG_CLASS_USER_HPP = 0x09,		/*���û�����Ϣ��,��MSG_CLASS_KEY��MSG_CLASS_USER����*/
	MSG_CLASS_USER = 0x0a,			/*�û�����Ϣ��*/
	MSG_CLASS_ERROR = 0x0b,			/*������Ϣ��*/
	MSG_CLASS_COUNT
}MSG_MsgClass_e;

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
typedef struct
{
	MSG_MsgClass_e Class;
	unsigned int MsgCode;
	unsigned int PayloadLength;
	void *Payload;
} MSG_MsgInfo_t;/*Ӧ�ü���Ϣ�ṹ*/

typedef enum
{
	TIMER_STATUS_IDLE = 0,
	TIMER_STATUS_RUNNING = 0x01,
	TIMER_STATUS_PAUSED = 0x02,
	TIMER_STATUS_ABORT = 0x04,
	TIMER_STATUS_DELETE = 0x08
}MSG_Timer_Status_s;

typedef enum
{
	TIMER_MODE_REPEAT = 1,	/* �ظ���ʱ��	*/
	TIMER_MODE_ONE_IDLE = 2,	/* һ���Զ�ʱ��	*/
	TIMER_MODE_ONE_DEL = 4	/* һ���Զ�ʱ��	*/
}MSG_Timer_Mode_s;

typedef struct MSG_Timer
{
	char Mode;
	char Status;
	char TimeoutError;
	char Reserved;
	//void (*CallbackFun)(void *);
	void (*CallbackFun)(void);
	//void (*CallbackTransferFun)(struct MSG_Timer *Timer);
	void *CallbackParam;

	DRVOS_TimeMS_t Timeout_MS;
	DRVOS_TimeMS_t TimeRef_MS;
}MSG_Timer_t;

/*----------------------------------------------------------------------------
 *	Global Function Prototype
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : ReleaseTimerList
  Description     : �ͷŶ�ʱ�������ź���
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned int ReleaseTimerList(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : MsgCopy
  Description     : ��Ϣ����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int MsgCopy(MSG_MsgInfo_t *MsgDst, MSG_MsgInfo_t *MsgSrc);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : MSG_MsgReset
  Description     : ������Ϣ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int MSG_MsgReset(MSG_MsgInfo_t *Msg);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : MSG_GetMsg_Wait
  Description     : ����Ϣ���͵���Ϣ������
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int MSG_GetMsg_Wait(DRVOS_MsgQueue_t *MsgQueue, MSG_MsgInfo_t *Msg);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : MSG_GetMsg_Timeout_MS
  Description     : ����Ϣ���͵���Ϣ������(��ʱ�ȴ�ģʽ)
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int MSG_GetMsg_Timeout_MS(DRVOS_MsgQueue_t *MsgQueue,MSG_MsgInfo_t *Msg,DRVOS_TimeMS_t MsNum);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : MSG_SendMsg_Wait
  Description     : ������Ϣ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int MSG_SendMsg_Wait(DRVOS_MsgQueue_t *MsgQueue, MSG_MsgInfo_t *Msg);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : MSG_SendMsg_Timeout_MS
  Description     : ������Ϣ(��ʱ�ȴ�ģʽ)
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int MSG_SendMsg_Timeout_MS(DRVOS_MsgQueue_t *MsgQueue,MSG_MsgInfo_t *Msg,DRVOS_TimeMS_t MsNum);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : MSG_SendHPPMsg
  Description     : ������Ϣ(HPPģʽ)
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int MSG_SendHPPMsg(DRVOS_MsgQueue_t *MsgQueue, MSG_MsgInfo_t *Msg);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : MSG_SendTmMsg_MS
  Description     : ���Ͷ�ʱ��Ϣ(��ʱ�ȴ���ʽ)
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void MSG_SendTmMsg_MS(DRVOS_TimeMS_t MsNum, MSG_MsgInfo_t *Msg);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : MSG_DelTmMsg
  Description     : ɾ����ʱ��Ϣ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void MSG_DelTmMsg(MSG_MsgInfo_t *Msg);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : MSG_DelAppTmMsg
  Description     : ɾ������APP�Ķ�ʱ��Ϣ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void MSG_DelAppTmMsg(APP_AppInfo_t *AppInfo);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : MSG_TimerCreat
  Description     : ������ʱ��
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
MSG_Timer_t *MSG_TimerCreat(char *Name);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : MSG_TimerStart_MS
  Description     : ������ʱ��
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
MSG_Timer_t *MSG_TimerStart_MS(MSG_Timer_t *Timer, DRVOS_TimeMS_t MsNum, void *CallbackFun, void *CallbackParam, int Mode);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : MSG_TimerPause_MS
  Description     : ��ͣ��ʱ��
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
MSG_Timer_t *MSG_TimerPause_MS(MSG_Timer_t *Timer, BOOL Pause);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : MSG_TimerStop_MS
  Description     : ֹͣ��ʱ��
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
MSG_Timer_t *MSG_TimerStop_MS(MSG_Timer_t *Timer, BOOL Restart);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : MSG_TimerDel
  Description     : ɾ����ʱ��
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void MSG_TimerDel(MSG_Timer_t *Timer);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : MSG_SendMsgMngMsg
  Description     : ��Msg������������Ϣ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void MSG_SendMsgMngMsg(MSG_MsgInfo_t *Msg);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : MSG_SendMsgMngMsg_Nowait
  Description     : ��Msg������������Ϣ(�����ȴ�)
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void MSG_SendMsgMngMsg_Nowait(MSG_MsgInfo_t *Msg);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : MSG_SendAppMngMsg
  Description     : ��App������������Ϣ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void MSG_SendAppMngMsg(MSG_MsgInfo_t *Msg);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : MSG_SendAppMsg
  Description     : ��Ӧ����������Ϣ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void MSG_SendAppMsg(APP_AppInfo_t *AppInfo, MSG_MsgInfo_t *Msg);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : MSG_SendCurrentAppMsg_NoSemwait
  Description     : ��ǰӦ�÷�����Ϣ(�����ȴ�)
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int MSG_SendCurrentAppMsg_NoSemwait(MSG_MsgInfo_t *Msg);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : MSG_SendCurrentAppMsg
  Description     : ��ǰӦ�÷�����Ϣ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int MSG_SendCurrentAppMsg(MSG_MsgInfo_t *Msg);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : MSG_ManagerTaskInit
  Description     : Msg���������ʼ��
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void MSG_ManagerTaskInit(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : Panel_DeliverAppMsg
  Description     : �����ϱ��ӿ�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int Panel_DeliverAppMsg (unsigned char ucKeyRead);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : MSG_DeliverSmartCardMsg
  Description     : Tuner��Ƶ��Ϣ�ϱ�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void MSG_DeliverSmartCardMsg(BOOL bInsert);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : MSG_DeliverTunerMsg
  Description     : Tuner��Ƶ��Ϣ�ϱ�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void MSG_DeliverTunerMsg(BOOL bLock);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : MSG_DeliverIPMsg
  Description     : ����״̬��Ϣ�ϱ�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void MSG_DeliverIPMsg(BOOL bIpLink);

#endif //MSG_H__

