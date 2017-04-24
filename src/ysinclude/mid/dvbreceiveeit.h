/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : dvbreceiveeit.h

    Description : EIT���ݹ���

    Others:      

    History:  1. llh creat 2004-3-29
			  2. xyan Modify 2011-11-14 

*******************************************************************************/
#ifndef _YSEITREC_
#define _YSEITREC_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "msg.h"
#include "stbdata.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Global Function Prototype
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_GetSectionInUseMaskNew
  Description     : ��ȡ��׼�����ģ���m_uiSectionInUseMaskNewֵ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned int DVB_GetSectionInUseMaskNew(void);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_EPGDataUpdataWait
  Description     : �ڶ�Service�е�EventList����ǰ�����ź���������
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DVB_EPGDataUpdataWait(void);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_EPGDataUpdataSignal
  Description     : �ڶ�Service�е�EventList�����󣬽���ź���������
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DVB_EPGDataUpdataSignal(void);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_GetReReceiveEITFlag
  Description     : ��ȡ������ȡEPG���Ʊ�ʶ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DVB_GetReReceiveEITFlag(void);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_SetReReceiveEITFlag
  Description     : ����������ȡEPG���Ʊ�ʶ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DVB_SetReReceiveEITFlag(BOOL Flag);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_ResetMemoryLackTag
  Description     : ��EPG�ڴ�ȱ����ʶ��λ(��ΪFALSE)
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DVB_ResetMemoryLackTag(void);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_GetMemoryLackTag
  Description     : ��ȡEPG�ڴ�ȱ����ʶ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DVB_GetMemoryLackTag(void);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_FreeEPGData
  Description     : �ͷ����е�EPG����
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DVB_FreeEPGData(void);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_ReceiveEPGForChangeChannel
  Description     : DTV��̨��EPG���ݽӿ�
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DVB_ReceiveEPGForChangeChannel(unsigned short usChnlNO);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_ReceiveEPGForNVOD
  Description     : NVODӦ������EPG���ݽӿ�
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DVB_ReceiveEPGForNVOD(STBDATAServiceInfo_t *pServiceInfo);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DVB_ReceiveEPGForUpdate
  Description     : ������ȡEPG����
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DVB_ReceiveEPGForUpdate(void);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_SendCommandToStandardTableReceiveTaskNew
  Description     : Ӧ�ø�EPG����������Cmd
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DVB_SendCommandToStandardTableReceiveTaskNew(unsigned int nCmd);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_SendMsgToStandardTableReceiveTaskNew
  Description     : Ӧ�ø�EPG������������Ϣ(Msg)
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DVB_SendMsgToStandardTableReceiveTaskNew(MessageInfo_t *pMsg);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_RequestStandardTableReceiveNew
  Description     : Ӧ����EPG�������񷢳�EPG���ݽ�������
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DVB_RequestStandardTableReceiveNew(unsigned int uiReceiveTableState,signed int siPriority,void *pData,DRVOS_Partition_t *pPtn,DvbDelivery_t stDelivery,
										unsigned short usTsID,unsigned short usOriginalNetworkID,unsigned short usServiceID,DRVOS_MsgQueue_t *pstMsgQueue);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_ReceiveStandardTableTaskInit
  Description     : ����EPG ���������ʼ��
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DVB_ReceiveEITTaskInit(void);

#endif
