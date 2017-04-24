/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : msg.h

    Description : 消息管理

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
#define MASK_DRIVER				0x01000000	/*the message caused by low level driver 由底层驱动引起的消息*/
#define MASK_APP				0x02000000	/*the message send by application controller 来自应用管理器的消息*/
#define MASK_CA					0x03000000	/*the message send by CA 来自CA的消息*/
#define MASK_DVB				0x04000000	/*the message send by DVB 来自DVB的消息*/
#define MASK_DBC				0x05000000	/*the message send by DBC 来自DBC的消息*/
#define MASK_USER				0x06000000	/*the message send by User Define 来自用户自定义的消息*/
#define MASK_KEY				0x08000000	/*the message caused by key action 由于按键操作引起的消息*/
#define MASK_ERROR				0x09000000	/*the message send by Error 来自应用上报的错误消息*/

/*驱动级消息MsgCode定义*/
#define DRV_MSGCODE_BASE		MASK_DRIVER	/*驱动首消息MsgCode*/
#define DRV_CARRIER_DATA_LOCK	(MASK_DRIVER+0x01)/*驱动锁住频消息*/
#define DRV_CARRIER_DATA_LOST	(MASK_DRIVER+0x02)/*驱动失锁消息*/
#define DRV_CHANGE_TO_PAL		(MASK_DRIVER+0x03)/*驱动转换为PAL制消息*/
#define DRV_CHANGE_TO_NTSC		(MASK_DRIVER+0x04)/*驱动转换为NTSC制消息*/
#define DRV_ETH_LINK			(MASK_DRIVER+0x05)/*驱动网线插好消息*/
#define DRV_ETH_UNLINK			(MASK_DRIVER+0x06)/*驱动网线拔出消息*/ 
#define DRV_SMARTCARD_INSERT	(MASK_DRIVER+0x07)/*驱动智能卡插入消息*/
#define DRV_SMARTCARD_REMOVE	(MASK_DRIVER+0x08)/*驱动智能卡拔出消息*/ 

/*应用级消息MsgCode定义*/
#define APP_MSGCODE_BASE		MASK_APP		/*应用首消息MsgCode*/
#define APP_QUERY				(MASK_APP+0x01)	/*询问应用状态消息*/
#define APP_RESPONSE			(MASK_APP+0x02)	/*应用询问回复消息*/
#define APP_START 				(MASK_APP+0x03)	/*启动应用消息*/
#define APP_QUIT				(MASK_APP+0x04)	/*退出应用消息*/
#define APP_SUSPEND				(MASK_APP+0x05)	/*挂起应用消息*/
#define APP_RESUME				(MASK_APP+0x06)	/*恢复应用消息*/
#define APP_SHOW				(MASK_APP+0x07)	/*显示应用消息*/
#define APP_HIDE				(MASK_APP+0x08)	/*隐藏应用消息*/
#define APP_WINFOCUS			(MASK_APP+0x09)	/*应用获得焦点消息*/
#define APP_LOSEFOCUS			(MASK_APP+0x0a)	/*应用失去焦点消息*/
#define MSGBOX_CLOSE			(MASK_APP+0x0b) /*close message box*/
#define APP_MSGMNG_OK			(MASK_APP+0x0c) /*Msg管理任务完成启动消息*/
#define APP_EXIT				(MASK_APP+0x0d) /*应用删除消息*/
#define APP_DTV_START			(MASK_APP+0x0e) /*DTV启动消息*/
#define APP_DTV_SUSPEND			(MASK_APP+0x0f) /*DTV挂起消息*/
#define APP_START_APP			(MASK_APP+0x10) /*应用开始启动消息*/

/*CA级消息MsgCode定义*/
#define CA_MSGCODE_BASE			MASK_CA		/*CA首消息MsgCode*/

/*DVB级消息MsgCode定义*/
#define DVB_MSGCODE_BASE		MASK_DVB	/*DVB首消息MsgCode*/

/*DBC级消息MsgCode定义*/
#define DBC_MSGCODE_BASE		MASK_DBC	/*DBC首消息MsgCode*/

/*按键级消息MsgCode定义*/
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

/*红绿黄蓝四色功能键*/
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

/*用户级消息MsgCode定义*/
#define USER_MSGCODE_BASE		MASK_USER		/*USER首消息MsgCode*/
#define FOCUS_MOVE				(MASK_USER+0x01)/*Switch focus among controls in same app*/
#define USER_DIALOG_SHOW		(MASK_USER+0x02)/*显示用户Dlg消息*/
#define USER_DIALOG_RETURN		(MASK_USER+0x03)/*用户Dlg回复消息*/
#define USER_APP_START	    	(MASK_USER+0x04)/*用户按键启应用消息*/
#define USER_TIME_UPDATE	 	(MASK_USER+0x05)/*用户定时更新消息*/


//定义各模块使用的用户级消息MsgCode起始值
#define DTV_USER_BASE			(USER_MSGCODE_BASE+0x010000)/*DTV模块用户自定义消息起始值*/
#define CA_USER_BASE			(USER_MSGCODE_BASE+0x020000)/*CA模块用户自定义消息起始值*/
#define DVB_USER_BASE			(USER_MSGCODE_BASE+0x030000)/*DVB模块用户自定义消息起始值*/
#define DBC_USER_BASE			(USER_MSGCODE_BASE+0x040000)/*DBC模块用户自定义消息起始值*/
#define USB_USER_BASE			(USER_MSGCODE_BASE+0x050000)/*USB模块用户自定义消息起始值*/
#define EPG_USER_BASE			(USER_MSGCODE_BASE+0x060000)/*EPG模块用户自定义消息起始值*/
#define NVOD_USER_BASE			(USER_MSGCODE_BASE+0x070000)/*NVOD模块用户自定义消息起始值*/
#define PROGMNG_USER_BASE		(USER_MSGCODE_BASE+0x080000)/*频道管理模块用户自定义消息起始值*/
#define STBSET_USER_BASE		(USER_MSGCODE_BASE+0x090000)/*系统设置模块用户自定义消息起始值*/
#define GAME_USER_BASE			(USER_MSGCODE_BASE+0x0a0000)/*游戏模块用户自定义消息起始值*/
#define PVR_USER_BASE			(USER_MSGCODE_BASE+0x0b0000)/*PVR模块用户自定义消息起始值*/
#define MOSAIC_USER_BASE		(USER_MSGCODE_BASE+0x0c0000)/*MOSAIC模块用户自定义消息起始值*/
#define CHSRCH_USER_BASE		(USER_MSGCODE_BASE+0x0d0000)/*搜索模块用户自定义消息起始值*/
#define FAVORITE_USER_BASE		(USER_MSGCODE_BASE+0x0e0000)/*喜爱管理用户自定义消息起始值*/
#define BOOKMNG_USER_BASE		(USER_MSGCODE_BASE+0x0F0000)/*预约管理用户自定义消息起始值*/

/*错误级消息MsgCode定义*/
#define ERROR_MSGCODE_BASE		MASK_ERROR					/*ERROR首消息MsgCode*/
#define INVALID_QUEUE			(ERROR_MSGCODE_BASE+0x01)	/*invalid message queue*/
#define INVALID_MESSAGE			(ERROR_MSGCODE_BASE+0x02)	/*invalid message*/
#define EMPTY_QUEUE				(ERROR_MSGCODE_BASE+0x03)	/*message queue is empty */

//定义各模块使用的错误级消息MsgCode起始值
#define DTV_ERROR_BASE			(ERROR_MSGCODE_BASE+0x010000)/*DTV模块用户自定义消息起始值*/
#define CA_ERROR_BASE			(ERROR_MSGCODE_BASE+0x020000)/*CA模块用户自定义消息起始值*/
#define DVB_ERROR_BASE			(ERROR_MSGCODE_BASE+0x030000)/*DVB模块用户自定义消息起始值*/
#define DBC_ERROR_BASE			(ERROR_MSGCODE_BASE+0x040000)/*DBC模块用户自定义消息起始值*/
#define USB_ERROR_BASE			(ERROR_MSGCODE_BASE+0x050000)/*USB模块用户自定义消息起始值*/
#define EPG_ERROR_BASE			(ERROR_MSGCODE_BASE+0x060000)/*EPG模块用户自定义消息起始值*/
#define NVOD_ERROR_BASE			(ERROR_MSGCODE_BASE+0x070000)/*NVOD模块用户自定义消息起始值*/
#define PROGMNG_ERROR_BASE		(ERROR_MSGCODE_BASE+0x080000)/*频道管理模块用户自定义消息起始值*/
#define STBSET_ERROR_BASE		(ERROR_MSGCODE_BASE+0x090000)/*系统设置模块用户自定义消息起始值*/
#define GAME_ERROR_BASE			(ERROR_MSGCODE_BASE+0x0a0000)/*游戏模块用户自定义消息起始值*/
#define PVR_ERROR_BASE			(ERROR_MSGCODE_BASE+0x0b0000)/*PVR模块用户自定义消息起始值*/
#define MOSAIC_ERROR_BASE		(ERROR_MSGCODE_BASE+0x0c0000)/*MOSAIC模块用户自定义消息起始值*/
#define CHSRCH_ERROR_BASE		(ERROR_MSGCODE_BASE+0x0d0000)/*搜索模块用户自定义消息起始值*/

/*----------------------------------------------------------------------------
 *	Enum & Macro 
 *----------------------------------------------------------------------------*/
typedef enum
{
	MSG_CLASS_INVALID = 0,			/*无效消息类*/
	MSG_CLASS_POWER = 0x01,			/*待机级消息类*/
	MSG_CLASS_DRV = 0x02,			/*驱动级消息类*/
	MSG_CLASS_APP = 0x03,			/*应用级消息类*/
	MSG_CLASS_CA = 0x04,			/*CA级消息类*/
	MSG_CLASS_DVB = 0x05,			/*DVB级消息类*/
	MSG_CLASS_DBC = 0x06,			/*DBC级消息类*/
	MSG_CLASS_TIME = 0x07,			/*定时级消息类*/ 
	MSG_CLASS_KEY = 0x08,			/*按键级消息类*/
	MSG_CLASS_USER_HPP = 0x09,		/*高用户级消息类,由MSG_CLASS_KEY或MSG_CLASS_USER衍生*/
	MSG_CLASS_USER = 0x0a,			/*用户级消息类*/
	MSG_CLASS_ERROR = 0x0b,			/*错误级消息类*/
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
} MSG_MsgInfo_t;/*应用级消息结构*/

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
	TIMER_MODE_REPEAT = 1,	/* 重复定时器	*/
	TIMER_MODE_ONE_IDLE = 2,	/* 一次性定时器	*/
	TIMER_MODE_ONE_DEL = 4	/* 一次性定时器	*/
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
  Description     : 释放定时器链表信号量
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
  Description     : 消息拷贝
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
  Description     : 重置消息
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
  Description     : 将消息发送到消息队列中
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
  Description     : 将消息发送到消息队列中(超时等待模式)
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
  Description     : 发送消息
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
  Description     : 发送消息(超时等待模式)
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
  Description     : 发送消息(HPP模式)
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
  Description     : 发送定时消息(超时等待方式)
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
  Description     : 删除定时消息
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
  Description     : 删除传入APP的定时消息
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
  Description     : 创建定时器
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
  Description     : 启动定时器
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
  Description     : 暂停定时器
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
  Description     : 停止定时器
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
  Description     : 删除定时器
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
  Description     : 向Msg管理任务发送消息
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
  Description     : 向Msg管理任务发送消息(不做等待)
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
  Description     : 向App管理任务发送消息
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
  Description     : 向应用任务发送消息
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
  Description     : 向当前应用发送消息(不做等待)
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
  Description     : 向当前应用发送消息
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
  Description     : Msg管理任务初始化
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
  Description     : 按键上报接口
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
  Description     : Tuner锁频消息上报
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
  Description     : Tuner锁频消息上报
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
  Description     : 网络状态消息上报
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void MSG_DeliverIPMsg(BOOL bIpLink);

#endif //MSG_H__

