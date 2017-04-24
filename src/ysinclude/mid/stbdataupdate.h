/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : stbdataupdate.h

    Description : ��̨���ݸ��¹���

    Others:      

    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef _YSstbdata_Standard_
#define _YSstbdata_Standard_
/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "driver.h"
#include "midware.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#define ManufacturerCode_JIUZHOU        202
#define TFCAS_CODEDOWNLOAD_DESCRIPTOR   0xA1
#define TFCAS_JZ_ID                     0x03df
#define DTVIA_TAG                       0x93
#define SMSX_NIT_LOCK_DESCRIPTOR        0x5f

#define UPDATE_POWEROFF_DLG_TIME	    60
#define RECEIVE_TDT_MAX_TIMES           3

/*���ú�̨��������(Ĭ�ϸ���TDT��EIT������������NIT,������Ա���ݾ�������޸�)*/
#define UPDATE_DEFAULT_DATA				31  

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
typedef struct NITDataSection
{
    unsigned char ucNitSection[1024];
    unsigned short usSectionLen;
    BOOL bInUsed;
}NITDataSection_t;

typedef enum UpdateDataType
{
	UPDATE_INVALID_DATA=0,	/*��̨����������*/
	UPDATE_TDT_DATA=1,      /*��̨����TDT����*/
	UPDATE_EIT_DATA=2,      /*��̨����EIT����*/
	UPDATE_NIT_DATA=4,      /*��̨����NIT����*/
	UPDATE_JZUPGRADE_DATA=8,/*��̨���¾���������Ϣ*/
	UPDATE_CAT_DATA=16,     /*��̨����CAT����*/
	UPDATE_BAT_DATA=32      /*��̨����BAT����*/
	/*ʣ�µı���*/
}UpdateDataType_e;

typedef enum DataChangeType
{
	NO_DATA_CHANGE = 0,
	CA_PARA_CHANGE = 1,
	AV_PARA_CHANGE = 2,
	PROG_CHANGE = 3
}DataChangeType_e;

/*----------------------------------------------------------------------------
 *	Global Function Prototype
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : StartReceiveNIT
  Description     : ��ʼ����NIT ����
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void StartReceiveNIT(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : FinishReceiveNIT
  Description     : ��������NIT ����
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void FinishReceiveNIT(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetNITReceiveStatus
  Description     : ��ȡ����NIT ����״̬
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL GetNITReceiveStatus(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : IsNITReceived
  Description     : ��ȡ����NIT ����״̬
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL IsNITReceived(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetNITDataLength
  Description     : ��ȡNIT ���ݳ���(����ͬ��CA������ƹ���)
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned short GetNITDataLength(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetNITData
  Description     : ��ȡNIT ����(ͬ��CAû���յ�NIT��ȡ������)
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char *GetNITData(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetRegionNetworkID
  Description     : ��ȡNetworkID
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned short GetRegionNetworkID(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : SetRegionNetworkID
  Description     : ����NetworkID
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void SetRegionNetworkID(unsigned short usNetworkID);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : NVM_WriteRegionNetworkID
  Description     : ��NetworkIDд��E2P
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void NVM_WriteRegionNetworkID(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : NVM_ReadRegionNetworkID
  Description     : ��E2P�ж�ȡNetworkID
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void NVM_ReadRegionNetworkID(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetNITVersionChangedTag
  Description     : ��ȡNIT�汾���Ƿ�ı��ʾ
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL GetNITVersionChangedTag(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : SetNITVersionChangedTag
  Description     : ����NIT�汾���Ƿ�ı��ʾ
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void SetNITVersionChangedTag(BOOL tag);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetNITVersion
  Description     : ��ȡNIT�汾��
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char GetNITVersion(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : SetNITVersion
  Description     : ����NIT�汾��
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void SetNITVersion(unsigned char version);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : NVM_WriteNITVersion
  Description     : ��NIT�汾��д��E2P
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void NVM_WriteNITVersion(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : NVM_ReadNITVersion
  Description     : ��E2P����NIT�汾��
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void NVM_ReadNITVersion(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UpdateNIT
  Description     : ��̨����NIT(���Դ�ϵ�)
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void UpdateNIT(DRVOS_Partition_t *pPtn,DRVOS_MsgQueue_t *pstMsgQueue);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UpdateNITForSTBStart
  Description     : ����ʱ����NIT (���ɴ�ϵ�)
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL UpdateNITForSTBStart(unsigned short StartRequestStatus);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UpdateEITForSTBStart
  Description     : ����ʱ��ȡEPG����
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void UpdateEITForSTBStart(unsigned short StartRequestStatus);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UpdateEIT
  Description     : ��̨����EIT
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void UpdateEIT( DRVOS_Partition_t *pPtn,DRVOS_MsgQueue_t *pstMsgQueue);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : EITReceiveComplete
  Description     : �ж�����ʱ��ȡEPG�����Ƿ����
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL EITReceiveComplete(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UpdateTDT
  Description     : ��̨����TDT
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void UpdateTDT(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UpdateTDTStart
  Description     : ����ʱ����TDT
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void UpdateTDTStart(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : TDTReceived
  Description     : �ж�TDT�Ƿ��յ�
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL TDTReceived(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UpdateBAT
  Description     : ��̨����BAT����(���ڸ���BAT������Ϣ)
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : ע��!�˺���ֻ�ܸ���BAT������Ϣ,����������BAT���յ�ʱ��,ֻ��
  					����Ϊ������Ϣ��BAT.
*****************************************************************************/ 
void UpdateBAT(DRVOS_Partition_t *pPtn,DRVOS_MsgQueue_t *pstMsgQueue);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UpdateCAT
  Description     : ��̨����PAT��PMT��CAT��SDT����
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void UpdateCAT(DRVOS_Partition_t *pPtn,DRVOS_MsgQueue_t *pstMsgQueue);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : SetSTBStandby
  Description     : �������״̬
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void SetSTBStandby(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UpdateSTBLedDisplayTime
  Description     : ���»����д���״̬�µ�ʱ����ʾ
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void UpdateSTBLedDisplayTime(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UpdatePowerOffProcess
  Description     : ��̨����Զ����ػ�
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void UpdatePowerOffProcess(void);

#endif

