/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : stbdataupgrade.h

    Description : ��������

    Others:          

    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef _YSSTBDATAUPGRADE_H
#define _YSSTBDATAUPGRADE_H

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/      
#include "driver.h"     
#include "midware.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#define SECTION_WAIT_TIMEOUT_UMT_MS   		2000
#define SECTION_WAIT_TIMEOUT_UDT_MS  		300000
#define UPGRADE_UDT_RECEIVE_TIMEOUT 		0X30000000
#define UPGRADE_UDT_RECEIVE_STEP 			0X30000001
#define UPGRADE_UDT_RECEIVE_COMPLETE 		0X30000002

#define UPGRADE_FAILED						(0x00840000 + 5)
#define UPGRADE_SUCCESSED					(0x00840000 + 6)

#define MAX_UPGRADE_INFO_NUMBER 			10
#define MAX_UPGRADE_FILE_NUMBER 			1

#define UPGRADE_INVALID_VERSION 			0XFF
#define UPGRADE_INVALID_FILE_NUMBER 		0X00
#define UPGRADE_INVALID_FILE_TYPE 			0XFF

#define APPLICATION_UPGRADE_DESC 			0x80
#define APPLICATION_TYPE_DESC 				0X81
#define STB_SERIAL_DESC 					0x82
#define UPGRADE_MAX_STEP_TO_RECEIVE 		4

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
typedef enum UpgradeType
{
    UPGRADE_USUALLY,  /*�ֶ�����*/
    UPGRADE_FORCE,
    UPGRADE_PERMIT,	
    UPGRADE_INVALID_UPGRADE_TYPE = 0xFF
}UpgradeType_e;

typedef struct UpgradeFile
{
    unsigned int	uiFileLength;  
    unsigned short 	usMaxDataBlockNumber;
    unsigned short 	usDataBlockReceivedNumber;
    unsigned short 	usDataBlockStepToReceivedNumber;
    unsigned char 	*pBlockReceived;
    unsigned char 	*pFileData;
    unsigned char 	ucReceiveTableID;
    unsigned char 	ucFileType;    
    BOOL 			bReceiveComplete;
    BOOL 			bInUsed;
}UpgradeFile_t;

typedef struct UpgradeInfo
{
    unsigned short 	usHardWareVersion;   
    unsigned short 	usSoftWareVersion;   
    STBDATADeliveryInfo_t UpgradeDelivery;
    unsigned short 	usUDTPID;
    char 			scFileNumber;
    char 			ucUpgradeType;
    UpgradeFile_t 	urUpgradeFile[MAX_UPGRADE_FILE_NUMBER];
    BOOL 			bInUsed;
    unsigned char 	ucTag;
    unsigned char 	ucAppid;
    BOOL 			bTypeMatch;
    BOOL 			bSerialMatch;
}UpgradeInfo_t;

/*----------------------------------------------------------------------------
 *	Global Function Prototype
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time       : 2004-03-16
  Author          :   xyan
  Function Name   : UPGRADE_SetAppMsgQueue
  Description     : �����������������������������ж��Ƿ�����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void UPGRADE_ReceiveUMTStart(void);

/*****************************************************************************
  Date&Time       : 2004-03-16
  Author          :   xyan
  Function Name   : UPGRADE_SetAppMsgQueue
  Description     : ����������Ӧ�õ���Ϣ����(ԭ��������Ӧ��ʹ��)
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void UPGRADE_SetAppMsgQueue( DRVOS_MsgQueue_t* pMsgApp);

/*****************************************************************************
  Date&Time       : 2004-03-16
  Author          :   xyan
  Function Name   : UPGRADE_SendMsgToApp
  Description     : ������Ϣ������Ӧ��(ԭ��������Ӧ��ʹ��)
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void UPGRADE_SendMsgToApp(unsigned int uiMsgCode);

/*****************************************************************************
  Date&Time       : 2004-03-16
  Author          :   xyan
  Function Name   : UPGRADE_GetUMTReceiveStatus
  Description     : ��ȡ��������UMT����״̬
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL UPGRADE_GetUMTReceiveStatus(void);

/*****************************************************************************
  Date&Time       : 2004-03-16
  Author          :   xyan
  Function Name   : UPGRADE_SetUMTReceiveStatus
  Description     : ���þ�������UMT����״̬
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void UPGRADE_SetUMTReceiveStatus(BOOL flag);

/*****************************************************************************
  Date&Time       : 2004-03-16
  Author          :   xyan
  Function Name   : UPGRADE_GetUMTRequestStatus
  Description     : ��ȡ��������UMT�������״̬
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL UPGRADE_GetUMTRequestStatus(void);

/*****************************************************************************
  Date&Time       : 2004-03-16
  Author          :   xyan
  Function Name   : UPGRADE_SetUMTRequestStatus
  Description     : ���þ�������UMT�������״̬
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void UPGRADE_SetUMTRequestStatus(BOOL flag);

/*****************************************************************************
  Date&Time       : 2004-03-16
  Author          :   xyan
  Function Name   : UPGRADE_IsForceUpgrade
  Description     : �ж��Ƿ�Ϊǿ������
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL UPGRADE_IsForceUpgrade(void);

/*****************************************************************************
  Date&Time       : 2004-03-16
  Author          :   xyan
  Function Name   : UPGRADE_SetForceUpgrade
  Description     : ����������ʽΪǿ������
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void UPGRADE_SetForceUpgrade(void);

/*****************************************************************************
  Date&Time       : 2004-03-16
  Author          :   xyan
  Function Name   : UPGRADE_SetUpgradeType
  Description     : ������������
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void UPGRADE_SetUpgradeType(unsigned char flag);

/*****************************************************************************
  Date&Time       : 2004-03-16
  Author          :   xyan
  Function Name   : UPGRADE_GetUpgradeType
  Description     : ��ȡ��������
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
unsigned char UPGRADE_GetUpgradeType(void);

/*****************************************************************************
  Date&Time       : 2004-03-16
  Author          :   xyan
  Function Name   : UPGRADE_UpdateUpgradeInfo
  Description     : ��̨���¼���������
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void UPGRADE_UpdateUpgradeInfo(void);

/*****************************************************************************
  Date&Time       : 2004-03-16
  Author          :   xyan
  Function Name   : UPGRADE_UpdateUpgradeInfoForGUITask
  Description     : �ֶ�������������Ϣ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : ע:��Ҫǿ�������������ڼ�⵽������Ϣ�������ҵ������ļ�(Loader���Ҫ��)
*****************************************************************************/
void UPGRADE_UpdateUpgradeInfoForGUITask(void);

/*****************************************************************************
  Date&Time       : 2004-03-16
  Author          :   xyan
  Function Name   : UPGRADE_SendMsg2UpgradeTask
  Description     : ������Ϣ����������
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void UPGRADE_SendMsg2UpgradeTask(U32 uiMsgCode);

/*****************************************************************************
  Date&Time       : 2004-03-16
  Author          :   xyan
  Function Name   : UPGRADE_UpgradeTaskInit
  Description     : ���������ʼ��
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL UPGRADE_UpgradeTaskInit(void);


#endif

