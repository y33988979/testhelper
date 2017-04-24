/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : stbdatasearch.h

    Description : STB��������

    Others:      

    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef _YSSTBDATASEARCH_
#define _YSSTBDATASEARCH_
/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "driver.h"
#include "midware.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Global Function Prototype
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSet_GetCasVerifyStatus
  Description     : ��ȡ��Ŀ��Ȩ���״̬
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBSet_GetCasVerifyStatus(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSet_SetCasVerifyStatus
  Description     : ���ý�Ŀ��Ȩ���״̬
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetCasVerifyStatus(BOOL Staus);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSet_GetCasVerifyResult
  Description     : ��ȡ��Ŀ��Ȩ�����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBSet_GetCasVerifyResult(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSet_SetCasVerifyResult
  Description     : ���ý�Ŀ��Ȩ�����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetCasVerifyResult(BOOL Staus);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSet_CheckCasAuthority
  Description     : ����Ŀ�Ƿ���Ȩ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_CheckCasAuthority(unsigned char ucMsgType);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSearch_SetAppMsgQueue
  Description     : ����Ӧ�õ���Ϣ����
  Input           : pMsgApp ��Ϣ����
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : ���ڽ�midware�����������״̬���ݸ�����Ӧ��
*****************************************************************************/
void STBSearch_SetAppMsgQueue( DRVOS_MsgQueue_t* pMsgApp);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSearch_SendFinishCahnnelSearchMsgToSearchTask
  Description     : �����ж�Ƶ��������Ϣ��midware����������
  Input           : 
  Outut           : true���ͳɹ���false����ʧ�� 
  Calls           : ����Ӧ��
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void STBSearch_SendFinishCahnnelSearchMsgToSearchTask(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSearch_SendChannelSearchFinishedMsgToApp
  Description     : ����Ƶ������������Ϣ��Ӧ��
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void STBSearch_SendChannelSearchFinishedMsgToApp(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSet_SendChannelSearchTSChangedMsgToApp
  Description     : ����Ƶ������TS�л���Ϣ��Ӧ��
  Input           : uiTsNO TS�ţ�uiChnlSearchMode ����ģʽ
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SendChannelSearchTSChangedMsgToApp(unsigned int uiTsNO, unsigned int uiChnlSearchMode);

/*****************************************************************************
  Date&Time       : 2009-04-24  
  Author          :   xyan 
  Function Name   : STBSet_SetSTBJZDBCInfo
  Description     : �������ݹ㲥������Ϣ
  Input           : pTS ��������TS���ݽṹ
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : ����aut֧��avit�������������ɲο��ɶ�5516�汾06-10-18�ļ�¼
*****************************************************************************/ 
void STBSet_SetSTBJZDBCInfo(void);

/*****************************************************************************
  Date&Time       : 2009-04-24 
  Author          :   xyan 
  Function Name   : STBSet_SetSTBBATInfo
  Description     : ����BAT �����������Ϣ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetSTBBATInfo(DvbSI_t *pDvbSi);

/*****************************************************************************
  Date&Time    	  : 2010-06-01
  Author          :   xyan 
  Function Name   : STBSet_SetSTBChannelInfoForUpdate
  Description     : ����һ����˳���Ƶ����������(�ṩ����̨�������ɾ��Ƶ��ʹ��)
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetSTBChannelInfoForUpdate(void);

/*****************************************************************************
  Date&Time       : 2009-04-24 
  Author          :   xyan
  Function Name   : STBSet_StartChannelSearching
  Description     : ��ʼƵ������
  Input           : 
  Outut           : true �����ɹ���false����ʧ��
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBSet_StartChannelSearching(void);

/*****************************************************************************
  Date&Time       : 2009-04-24 
  Author          :   xyan
  Function Name   : STBSet_SetSeachingResult
  Description     : �����������
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetSeachingResult(void);

/*****************************************************************************
  Date&Time       : 2009-04-24 
  Author          :   xyan
  Function Name   : STBSet_CancelSeachingResult
  Description     : ���������������ݣ��ָ���ǰ������
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_CancelSeachingResult(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSet_SetChannelSearchedNUM
  Description     : �������������ӽ�Ŀ���㲥��Ŀ��NVOD�����ݹ㲥��Ŀ����Ŀ
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetChannelSearchedNUM(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSet_SetChannelSearchedNUMForSubSectionSearch
  Description     : �����ֶ��ֶ����������ӽ�Ŀ���㲥��Ŀ��NVOD�����ݹ㲥��Ŀ����Ŀ
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetChannelSearchedNUMForSubSectionSearch(signed short ssTsNO);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSet_GetTVChannelSearchedNUM
  Description     : ��ȡ�������ĵ��ӽ�Ŀ��
  Input           : 
  Outut           : ���ӽ�Ŀ��
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBSet_GetTVChannelSearchedNUM(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSet_GetRadioChannelSearchedNUM
  Description     : ��ȡ�������Ĺ㲥��Ŀ��
  Input           :
  Outut           : �㲥��Ŀ��
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBSet_GetRadioChannelSearchedNUM(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSet_GetNVODChannelSearchedNUM
  Description     : ��ȡ��������NVOD��Ŀ��
  Input           :
  Outut           : NVOD��Ŀ��
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBSet_GetNVODChannelSearchedNUM(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSet_GetDBCChannelSearchedNUM
  Description     : ��ȡ�����������ݹ㲥��Ŀ��  
  Input           :
  Outut           : ���ݹ㲥��Ŀ��
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBSet_GetDBCChannelSearchedNUM(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : STBSet_AllMemoryPartitionPrint
  Description     : ���������ڴ���Ƭ����
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_AllMemoryPartitionPrint(void);

#endif

