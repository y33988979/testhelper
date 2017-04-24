/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : app.h

    Description : 应用管理

    Others:      

    History:  1. llhui creat 2005-10-18
			  2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef APP_H_
#define APP_H_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "driver.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
/*应用到可按键启动的菜单应用中（epg、help）,使应用管理器能区分应用启动的方式*/
#define APP_NOT_START_BUTTON  			-1/*非按键启动*/
#define APP_NOT_START_MENU  			-1/*非菜单项启动*/
#define APP_DERECT_EXIT_TO_DTV  ((APP_AppInitInfo_t*)0xFFFFFFFF	)/*代替DTV启动地址*/

#define USER_WS_SIZE            		2048 		//the application workspace size
#define NORMAL_TIMER_LIMIT      		15625       //the normal timer limit 
#define NORMAL_PART_SIZE        		65536       //the size of normal paration
#define NORMAL_MSG_SIZE         		20          //the size of normal message of application
#define NORMAL_MAX_MSGS         		20          //max numbers of normal message queue

#define APP_NAME_MAX_LENGTH  			10

#define APP_MANAGER_SIZE  				4096*4		//消息队列20*20,初始化任务队列32*20,运行任务队列48*5
#define APP_MGR_MSG_SIZE  				20         	//the size of each message of application manager is 20 bytes
#define APP_MGR_MAX_MSGS  				20         	//max numbers of a message queue of application manager
#define APP_MGR_QUEUE_SIZE  	(DRVOS_MESSAGE_QUEUE_MEMSIZE(APP_MGR_MSG_SIZE,APP_MGR_MAX_MSGS))
#define APP_NORMAL_PRIORITY 			3

/*----------------------------------------------------------------------------
 *	Enum & Macro
 *----------------------------------------------------------------------------*/
typedef enum
{
	APP_SUSPEND = -1,
	APP_READY = 0,
	APP_ACTIVE = 1
}APP_AppStatus_e;

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
struct APP_AppInfo;

typedef int (*APP_AppInitFun)(struct APP_AppInfo*, void*);

typedef struct APP_AppInitInfo
{
	char name[APP_NAME_MAX_LENGTH];
	APP_AppInitFun  AppInit;
	int ButtonCode;						/*启动应用的键码*/
	int AtMenuId;						/*启动应用的菜单项*/
	int MenuFocus;						/*应用所在菜单的焦点项*/
	struct APP_AppInitInfo *AtAppMenu;	/*应用所在菜单,返回时启动*/
	struct APP_AppInitInfo *ReturnApp;	/*返回时启动的应用,优于AtAppMenu*/
}APP_AppInitInfo_t;

typedef struct APP_AppInfo
{
	/*兼容APP_AppMngInfo_t结构，使用同一顺序*/
	DRVOS_Task_t *Task;
	DRVOS_Partition_t *Part;
	DRVOS_MsgQueue_t *MsgQueue;

	DRVOS_Partition_t *AppPart;
	unsigned int AppPartSize;
	void *AppPartMem;        
	unsigned int  AppUniID;			//the unique identity     
	APP_AppInitInfo_t *AppInitInfo;
	void *Stack;
	unsigned int StackSize;
	void *PartMem;
	unsigned int PartSize;
	void *MsgQueueMem;
	APP_AppStatus_e AppStatus;
	APP_AppInitInfo_t *AppExit;		/*应用退出后启动的应用*/
}APP_AppInfo_t;

typedef struct 
{
	DRVOS_Task_t *Task;
	DRVOS_Partition_t *Part;
	DRVOS_MsgQueue_t *MsgQueue;
}APP_AppMngInfo_t;

/*----------------------------------------------------------------------------
 *	Global Function Prototype
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : APP_GetAppMngMsgQueue
  Description     : 获取APP管理任务消息队列
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DRVOS_MsgQueue_t* APP_GetAppMngMsgQueue(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : APP_GetAppMngPart
  Description     : 获取APP管理任务内存块指针
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DRVOS_Partition_t* APP_GetAppMngPart(void);

/*****************************************************************************
  Date&Time    :    2004-03-29
  Author          :   xyan
  Function Name   : NewApplicationStart
  Description     : 停收表任务，设置应用启动标志。
  Input           : 
  Outut           :  
  Calls           : 
  Called by       : 搜索、数据广播等和收表任务有冲突的应用
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void NewApplicationStart(void);

/*****************************************************************************
  Date&Time    :    2004-03-29
  Author          :   xyan
  Function Name   : EndApplication
  Description     : 设置应用停止标志
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void EndApplication(void);

/*****************************************************************************
  Date&Time    :    2004-03-29
  Author          :   xyan
  Function Name   : IsApplicationRunning
  Description     :  判断是否启动应用
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL IsApplicationRunning(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : APP_IsExistRunApp
  Description     : 判断当前是否有应用
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL APP_IsExistRunApp(void);  

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : APP_TryVisitRunApp
  Description     : 锁注册应用链表信号量
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void APP_TryVisitRunApp(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : APP_AllowVisitRunApp
  Description     : 释放注册应用链表信号量
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void APP_AllowVisitRunApp(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : APP_AppButtonGetApp
  Description     : 通过按键获取注册应用
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
APP_AppInitInfo_t *APP_AppButtonGetApp(int button);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : APP_AppStartIdGetApp
  Description     : 通过注册菜单和ID获取注册应用
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
APP_AppInitInfo_t *APP_AppStartIdGetApp(APP_AppInitInfo_t *AtAppMenu, int id);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : APP_AppNameGetApp
  Description     : 通过名字获取注册应用
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
APP_AppInitInfo_t *APP_AppNameGetApp(char *AppName);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : APP_TaskInit
  Description     : 初始化应用任务
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int APP_TaskInit(void *Function,int Priority,int StackSize,int PartSize,int MaxMessageSize,unsigned int MaxMessages,int flags,APP_AppInfo_t *AppInfo);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : APP_StartAppSpecial
  Description     : 直接启动应用接口
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 退出当前应用，并起新的应用
*****************************************************************************/ 
int APP_StartAppSpecial(char *AppName);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : APP_DirectStartApp
  Description     : 直接启动应用任务接口(注意处理应用等待退出的可能)
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 发送APP_QUIT至当前应用,随后启动新的应用任务,
*****************************************************************************/ 
int APP_DirectStartApp(char *AppName);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : APP_OnExit
  Description     : APP任务退出后的处理(Linux操作系统使用)
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void APP_OnExit(APP_AppInfo_t *AppInfo);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : APP_OSOnExit
  Description     : APP任务退出后的处理(OS操作系统使用)
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void APP_OSOnExit(DRVOS_Task_t *Task,int Param);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : APP_AddApplication
  Description     : 添加注册应用至m_AppInitList
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void APP_AddApplication(char *name,APP_AppInitFun AppInit,int StartButton,int AtMenuOrder,char *AtMenuName);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : APP_GetRunApp
  Description     : 获取当前运行的应用任务
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
APP_AppInfo_t *APP_GetRunApp(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : APP_ManagerTaskInit
  Description     : APP管理任务初始化
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void APP_ManagerTaskInit(void);

#endif 

