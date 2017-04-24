/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : midcas.h

    Description : CAģ�����ʹ�õĽӿڷ�װ����

    Others:      

    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef _MIDCAS_H__
#define _MIDCAS_H__

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "driver.h"
#include "appys.h"
#include "midware.h"
#include "ca_filter.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
/*---------- ECM_PID���õĲ������� ---------*/
#define MIDCAS_LIST_OK          0x00
#define MIDCAS_LIST_FIRST       0x01
#define MIDCAS_LIST_ADD         0x02

#define MIDCAS_SC_OUT 			1
#define MIDCAS_SC_IN 			2
#define MIDCAS_SC_INVALID 		3

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
/*��ߵ���λ��ʾ���汾�����ش��޸ĺ����м���λ��ʾ���汾��������¹��ܺ���
* �����λ��ʾ�޸ĺţ����޸�BUG  ����ÿ���޸ĳ������Ҫ�������°汾��
*
* ע��:  �������ط���Ҫ�޸ģ�һ�ǰ汾�ַ�������һ���ǰ汾��ֵ!!!
*/
typedef struct CAS_LibraryInfo_s
{
	unsigned char  Name[16];      	/* ģ������ */
	unsigned char  VerString[16];  	/* ģ��汾�ַ��� */
	unsigned char  Builder[32];    	/* �����߻����� */
	unsigned char  BuildDate[16];  	/* �������� */
	unsigned char  BuildTime[16];  	/* ����ʱ�� */
	int SoftVersion;    			/* ģ��汾 */
	int HardVersion;   				/* Ӳ���汾 */
	unsigned char  Reserved[24];   	/* �����ֶ� */
}CAS_LibraryInfo;    /* �ܳ���Ϊ128�ֽ� */

//��Ҫ����ͬ��CA��������ѶCA,���ECM��Ϣʱ�����ݽṹ
typedef struct _MIDCASServiceInfo{
    unsigned short  m_wEcmPid;   	/* ��Ŀ��Ӧ������Ϣ��PID */
    unsigned char   m_byServiceNum; /* ��ǰPID�µĽ�Ŀ���� */
    unsigned char   m_byReserved;   /* ���� */
    unsigned short  m_wServiceID[4];/* ��ǰPID�µĽ�ĿID�б� */
}SMIDCASServiceInfo;

/*CAϵͳ����ö�٣������µ�casʱ���ڴ���Ӷ�Ӧö��*/
typedef enum
{
	UNKNOW_CA = 0,
	CDCAS30_CA = 1,
	SMSX_CA = 2,
}CAType_t;
/*----------------------------------------------------------------------------
 *	Function Prototype
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_AdpInit
  Description     : ����CAS
  Input           : 
  Outut           : 1:�����ɹ�, 0:����ʧ��
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int midcas_AdpInit(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_Add_CAApp
  Description     : ע��CASӦ������
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void midcas_Add_CAApp(int StartButton,int AtMenuOrder,char *AtMenuName);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_Add_EmailApp
  Description     : ע��CAS�ʼ�����
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void midcas_Add_EmailApp(int StartButton, int AtMenuOrder, char *AtMenuName);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_SendOSDMSG
  Description     : ��CA_ADPTask����Ϣ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void midcas_SendOSDMSG(MSG_MsgInfo_t *MsgInfo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_ADPTranslateMsg
  Description     : ����Ϣ����CAS�ж��Ƿ���Ҫת��ΪCAS��Ϣ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void midcas_ADPTranslateMsg(MSG_MsgInfo_t *MsgInfo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_ADPTVCHChange
  Description     : ��Ŀ��̨����
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void midcas_ADPTVCHChange(STBDATAServiceInfo_t *pSTBServiceInfo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_ADPRecChange
  Description     : ¼�ƽ�Ŀ��̨����
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void midcas_ADPRecChange(STBDATAServiceInfo_t *pSTBServiceInfo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_OSDHideBuyMsg
  Description     : ���CAS��ʾ��Ϣ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void midcas_OSDHideBuyMsg(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_IsIntegCAID
  Description     : �ж��Ƿ�ͬ��CASϵͳID
  Input           : 
  Outut           : 0:����ͬ����SystemID,1:��ͬ����SystemID
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int midcas_IsIntegCAID(unsigned short usCasID);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_GetScStatus
  Description     : �ж����ܿ�״̬
  Input           : 
  Outut           : 0:�ɹ�,1:�޿�,2:���ڻ�������,3:����Ч
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
int midcas_GetScStatus(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_GetScCardID
  Description     : ��ȡ���ܿ�����
  Input           : 
  Outut           : 0:�ɹ�,1:��ȡ����ʧ��,2:����Ч,3:����Ĵ�ſռ䲻��
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
int midcas_GetScCardID(int iCardIDLength, char* pSmartCardID);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_StopPrivateData
  Description     : ͣCAS˽������
  Input           : 0:ͣECM,1:ͣEMM,2:ͬʱͣECM��EMM
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : ����TFCAS21PROEX��TFCAS21PRO֧��ͬʱͣECM��EMM,����CDCAS30ֻ֧��ͣECM
*****************************************************************************/
int midcas_StopPrivateData(int iStopType);  

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_StopPSIData
  Description     : ֹͣCA��PSI����
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
int midcas_StopPSIData(int iStopType);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_StartPSIData
  Description     : ����CA��PSI����
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
int midcas_StartPSIData(int iStartType);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_DeleteAllEmail
  Description     : ɾ��CAS�����ʼ�
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void midcas_DeleteAllEmail(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_SetOldCW
  Description     : ������ǰ��CW
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void midcas_SetOldCW(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_SetInvalidCW
  Description     : ������Ч��CW
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void midcas_SetInvalidCW(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_GetProductID
  Description     : �����Ȩ��ProductID�б�
  Input           : 
  Outut           : 0:��ȡʧ��,1:��ȡ�ɹ�,2:����Ĵ洢�ռ䲻��
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
int midcas_GetProductID(int MaxCount, int *ProductSum);
        
/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_GetOperatorIds
  Description     : �����Ӫ���б�
  Input           : 
  Outut           : 0:��ȡʧ��,1:��ȡ�ɹ�
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
unsigned short midcas_GetOperatorIds(int MaxOperatorCount, unsigned short* pusOperatorIds);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_GetACList
  Description     : �����Ӫ�̵�����ֵ
  Input           : 
  Outut           : 0:��ȡʧ��,1:��ȡ�ɹ�
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
unsigned short midcas_GetACList(unsigned short usTVSID, unsigned long* pACArrayInfo);

/*************************************************
* Function:       midcas_SetEcmPid
* 
* Input:            �����Type��midcas.h�ж���
*
* Description:    ����ECM PID
*
* Input:  
* Output:        
* Return:         ��
*
* Others:         �ӿڱ��24
*************************************************/
void  midcas_SetEcmPid(int Type, SMIDCASServiceInfo* pServiceInfo);

/*************************************************
* Function:       midcas_SetEmmPid
* 
* Input:           EMM PID
*
* Description:    ����EMM PID
*
* Input:  
* Output:        
* Return:         ��
*
* Others:         �ӿڱ��25
*************************************************/
void  midcas_SetEmmPid(unsigned short usEmmPid);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_PrintCAVersion
  Description     : ��ӡCA�汾��
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void midcas_PrintCAVersion(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_GetCAVersion
  Description     : ��ȡCAS�汾��
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
unsigned int midcas_GetCAVersion(void);

/*****************************************************************************
  Date&Time    	  : 2012-12-26
  Author          :   wwqing
  Function Name   : midcas_GetCASName
  Description     : ��ȡCAϵͳ����
  Input           : ucCasName �������ݵ��ڴ��ַ��CasNameLen �����ڱ������ݵĳ���
  Outut           : �ɹ�����1��ʧ�ܷ���0
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
unsigned char midcas_GetCASName(char *ucCasName, unsigned char ucCasNameLen);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_GetPlatformID
  Description     : ��ȡCAƽ̨���
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
unsigned short midcas_GetPlatformID(void);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          : wpeng
  Function Name   : midcas_TimeTransform
  Description     : ʱ��ת��
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void midcas_TimeTransform(unsigned short CATime, StbTm_t *TransTime);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          : wpeng
  Function Name   : midcas_WriteCASCUpInfoAddr
  Description     : д���ܿ������ɹ���Ϣ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void midcas_WriteCASCUpInfoAddr(unsigned char* pScUpInfo);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          : wpeng
  Function Name   : midcas_ReadCASCUpInfoAddr
  Description     : ��ȡ���ܿ�������Ϣ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void midcas_ReadCASCUpInfoAddr(unsigned char* pScUpInfo);

/*****************************************************************************
  Date&Time       : 2012-12-26
  Author          : wwqing
  Function Name   : midcas_GetCasInitStatus
  Description     : ��ȡCAϵͳ��ʼ��״̬
  Input           : 
  Outut           :  1����ʼ����ɣ�0��δ��ʹ��
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
unsigned char midcas_GetCasInitStatus(void);

/*****************************************************************************
  Date&Time       : 2013-01-10
  Author          : wwqing
  Function Name   : midcas_UpdateSmsxPdsdData
  Description     : ����������ѶNIT������������ʹ�õ�pdsd���ݣ����pdsd��e2p�еĲ�ͬ��
  					�����DVTSTBCA_GetNitValue()��֪ca
  Input           : uiPdsd
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void midcas_UpdateSmsxPdsdData(unsigned int uiPdsd);

/*****************************************************************************
  Date&Time       : 2013-01-10
  Author          : wwqing
  Function Name   : midcas_SetOSDRunTime
  Description     : ����smsx ca osd������ʾʱ��
  Input           : Time
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : ����caʹ��
*****************************************************************************/
void midcas_SetOSDRunTime(unsigned int Time);

/*****************************************************************************
  Date&Time       : 2013-01-10
  Author          : wwqing
  Function Name   : midcas_GetDescrambleEcmPid
  Description     : CA��ȡ������EcmPid
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : tfcaʹ��
*****************************************************************************/
unsigned short midcas_GetDescrambleEcmPid(DescrambleType_e DescrambleType);

/*****************************************************************************
  Date&Time       : 2013-01-10
  Author          : wwqing
  Function Name   : midcas_GetCASType
  Description     : ��ȡcasϵͳ������
  Input           : ���� CAType_t
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : ����ģ��ʹ�ô˽ӿڣ��Ա���ʹ��ca�꣬ʹ����ģ�������caģ��
*****************************************************************************/
CAType_t midcas_GetCASType(void);

#endif //end of _MIDCAS_H__


