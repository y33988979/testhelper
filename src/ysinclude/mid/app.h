/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : app.h

    Description : Ӧ�ù���

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
/*Ӧ�õ��ɰ��������Ĳ˵�Ӧ���У�epg��help��,ʹӦ�ù�����������Ӧ�������ķ�ʽ*/
#define APP_NOT_START_BUTTON  			-1/*�ǰ�������*/
#define APP_NOT_START_MENU  			-1/*�ǲ˵�������*/
#define APP_DERECT_EXIT_TO_DTV  ((APP_AppInitInfo_t*)0xFFFFFFFF	)/*����DTV������ַ*/

#define USER_WS_SIZE            		2048 		//the application workspace size
#define NORMAL_TIMER_LIMIT      		15625       //the normal timer limit 
#define NORMAL_PART_SIZE        		65536       //the size of normal paration
#define NORMAL_MSG_SIZE         		20          //the size of normal message of application
#define NORMAL_MAX_MSGS         		20          //max numbers of normal message queue

#define APP_NAME_MAX_LENGTH  			10

#define APP_MANAGER_SIZE  				4096*4		//��Ϣ����20*20,��ʼ���������32*20,�����������48*5
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
	int ButtonCode;						/*����Ӧ�õļ���*/
	int AtMenuId;						/*����Ӧ�õĲ˵���*/
	int MenuFocus;						/*Ӧ�����ڲ˵��Ľ�����*/
	struct APP_AppInitInfo *AtAppMenu;	/*Ӧ�����ڲ˵�,����ʱ����*/
	struct APP_AppInitInfo *ReturnApp;	/*����ʱ������Ӧ��,����AtAppMenu*/
}APP_AppInitInfo_t;

typedef struct APP_AppInfo
{
	/*����APP_AppMngInfo_t�ṹ��ʹ��ͬһ˳��*/
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
	APP_AppInitInfo_t *AppExit;		/*Ӧ���˳���������Ӧ��*/
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
  Description     : ��ȡAPP����������Ϣ����
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
  Description     : ��ȡAPP���������ڴ��ָ��
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
  Description     : ͣ�ձ���������Ӧ��������־��
  Input           : 
  Outut           :  
  Calls           : 
  Called by       : ���������ݹ㲥�Ⱥ��ձ������г�ͻ��Ӧ��
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void NewApplicationStart(void);

/*****************************************************************************
  Date&Time    :    2004-03-29
  Author          :   xyan
  Function Name   : EndApplication
  Description     : ����Ӧ��ֹͣ��־
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
  Description     :  �ж��Ƿ�����Ӧ��
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
  Description     : �жϵ�ǰ�Ƿ���Ӧ��
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
  Description     : ��ע��Ӧ�������ź���
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
  Description     : �ͷ�ע��Ӧ�������ź���
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
  Description     : ͨ��������ȡע��Ӧ��
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
  Description     : ͨ��ע��˵���ID��ȡע��Ӧ��
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
  Description     : ͨ�����ֻ�ȡע��Ӧ��
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
  Description     : ��ʼ��Ӧ������
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
  Description     : ֱ������Ӧ�ýӿ�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : �˳���ǰӦ�ã������µ�Ӧ��
*****************************************************************************/ 
int APP_StartAppSpecial(char *AppName);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : APP_DirectStartApp
  Description     : ֱ������Ӧ������ӿ�(ע�⴦��Ӧ�õȴ��˳��Ŀ���)
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : ����APP_QUIT����ǰӦ��,��������µ�Ӧ������,
*****************************************************************************/ 
int APP_DirectStartApp(char *AppName);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : APP_OnExit
  Description     : APP�����˳���Ĵ���(Linux����ϵͳʹ��)
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
  Description     : APP�����˳���Ĵ���(OS����ϵͳʹ��)
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
  Description     : ���ע��Ӧ����m_AppInitList
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
  Description     : ��ȡ��ǰ���е�Ӧ������
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
  Description     : APP���������ʼ��
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void APP_ManagerTaskInit(void);

#endif 

